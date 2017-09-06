/*
 * Copyright (C) 2017 The Android Open Source Project
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

//#include <libavb_ab/libavb_ab.h>

/*add for avb ops*/
#include <common.h>
#include <android_bootimg.h>
#include "loader_common.h"
#include <malloc.h>
#include <ext_common.h>
#include <ext4fs.h>
#include <linux/sizes.h>
#include "dl_operate.h"
#include "sprd_common_rw.h"

#include <otp_helper.h>
#include <stdio_dev.h>
/*end of add for avb ops*/

//#include "sprd_imgversion.h"

#include <avb/uboot_avb_ops.h>
#include "avb_ops.h"
#include <avb/uboot_avb_util.h>

extern AvbOps g_avb_ops;
extern unsigned char *g_eMMCBuf;

AvbCheckType g_ulAvbCheckType = AVB_CHECK_BOOTING;
unsigned char g_sprd_vboot_key[SPRD_RSA4096PUBK_HASHLEN] __attribute__((aligned(4096))); /*must be PAGE ALIGNED*/
unsigned int g_sprd_vboot_key_len = 0;
unsigned char g_sprd_vboot_cmdline[SPRD_VBOOT_CMDLINE_MAXSIZE]__attribute__((aligned(4096))); /*must be PAGE ALIGNED*/

/*for skip check vbmeta when already checked*/
bool g_sprd_vbmeta_checked_flag = false;

/* This function hex dumps a buffer of characters */
int do_hex_dump(void * vaddr, int buflen)
{
	uint8_t *p;
	uint32_t bytes = buflen;
	uint32_t i;

	p = (uint8_t *)vaddr;
	for (i = 0; i < bytes; i++)
	{
		if(i%16 == 0)
		{
			printf("\ni:%d     ", i);
		}
		printf("  0x%02x", p[i]);
	}
	printf("\n");
}

AvbIOResult read_from_partition(AvbOps* ops,
                                       const char* partition_name,
                                       int64_t offset_from_partition,
                                       size_t num_bytes,
                                       void* buf,
                                       size_t* out_num_read) {
	int ret = 0;
    uint64_t partition_size;
#if 0
	disk_partition_t info;

	if (get_partition_info_by_name(dev, partition_name, &info)) {
		debug("verify get partition %s info failed!\n", partition_name);
		return AVB_IO_RESULT_ERROR_IO;
	}

	partition_size = info.size * EMMC_SECTOR_SIZE;
	debug("%s=%x  =%x\n", partition_name, info.size, partition_size);
#else
	extern int secure_get_partition_size(uchar * partition_name, uint64_t * size);
	secure_get_partition_size(partition_name, &partition_size);
#endif

  if (offset_from_partition < 0) {
    if ((-offset_from_partition) > partition_size) {
      debug("Offset outside range.\n");
      return AVB_IO_RESULT_ERROR_RANGE_OUTSIDE_PARTITION;
    }
    offset_from_partition = partition_size - (-offset_from_partition);
  }

  /* Check if num_bytes goes beyond partition end. If so, don't read beyond
   * this boundary -- do a partial I/O instead.
   */
  if (num_bytes > partition_size - offset_from_partition)
    *out_num_read = partition_size - offset_from_partition;
  else
    *out_num_read = num_bytes;

	if((strcmp(partition_name, "vbmeta")) && (g_ulAvbCheckType == AVB_CHECK_DOWNLOAD))
	{
		debug("buf:0x%x, g_eMMCBuf:0x%x, offset_from_partition:%d num_bytes:%d\n", buf, g_eMMCBuf, offset_from_partition, num_bytes);

		memcpy((char *)buf, g_eMMCBuf+offset_from_partition, num_bytes);

#ifdef CONFIG_VBOOT_DUMP
	    do_hex_dump(g_eMMCBuf, 512);
#endif
		*out_num_read = num_bytes;
	}
	else
	{
		ret = common_raw_read(partition_name, (uint64_t)num_bytes, (uint64_t)offset_from_partition, (char *)buf);
		if (0 != ret) {
			debug("PARTITION %s read error, ret:%d \n", partition_name, ret);
			return ret;
		}

		*out_num_read = num_bytes;
	}

	return ret;
}

AvbIOResult validate_vbmeta_public_key(
    AvbOps* ops,
    const uint8_t* public_key_data,
    size_t public_key_length,
    const uint8_t* public_key_metadata,
    size_t public_key_metadata_length,
    bool* out_key_is_trusted) {
    AvbIOResult ret = AVB_IO_RESULT_OK;
    unsigned char sprd_vboot_key_in_vbmeta[SPRD_RSA4096PUBK_HASHLEN];

    uint8_t* public_key_hash;
    size_t public_key_hash_length;

    avb_debug("Enter: implement validate_vbmeta_public_key().\n");
    (void)public_key_metadata;
    (void)public_key_metadata_length;

    if (!out_key_is_trusted ||!public_key_data || !public_key_length) {
	avb_errorv("invalid: para is null.\n", NULL);
	return AVB_SLOT_VERIFY_RESULT_ERROR_OOM;
    }

    /*get expected_public_key from user data*/
    public_key_hash = avb_ops_get_vboot_key(&public_key_hash_length);

#ifdef CONFIG_VBOOT_DUMP
    debug("dump public_key_hash: public_key_hash_length:%d. \n", public_key_hash_length);
    if(public_key_hash_length && public_key_hash) {
        do_hex_dump(public_key_hash, public_key_hash_length);
    }
#endif

    extern void cal_sha256(uint8_t * input, uint32_t bytes_num, uint8_t * output);
    cal_sha256((uint8_t *)public_key_data, (uint32_t)public_key_length, (uint8_t *)sprd_vboot_key_in_vbmeta);
#ifdef CONFIG_VBOOT_DUMP
    do_hex_dump((uint8_t *)sprd_vboot_key_in_vbmeta, SPRD_RSA4096PUBK_HASHLEN);
#endif

    if (public_key_hash_length != SPRD_RSA4096PUBK_HASHLEN ||
	avb_safe_memcmp(public_key_hash, sprd_vboot_key_in_vbmeta, SPRD_RSA4096PUBK_HASHLEN) != 0) {
	avb_debugv("expected_public_key is not matched.\n", NULL);
	avb_errorv("vbmeta",
		": Public key used to sign data does not match key in chain "
		"partition descriptor.\n",
		NULL);

	*out_key_is_trusted = false;
	ret = AVB_SLOT_VERIFY_RESULT_ERROR_PUBLIC_KEY_REJECTED;
    } else {
	avb_debugv("expected_public_key is matched.\n", NULL);
	*out_key_is_trusted = true;
    }

  return ret;
}

static AvbIOResult read_rollback_index(AvbOps* ops,
                                       size_t rollback_index_slot,
                                       uint64_t* out_rollback_index) {
  uint32_t swVersion = 0;
  int ret = 0;

  //ret = sprd_get_imgversion((antirb_image_type)rollback_index_slot, &swVersion);
  debug(">>>>> rollback_index_slot:0x%llx, swVersion:%d, ret:%d.\n", rollback_index_slot, swVersion, ret);

  if((!ret) && (out_rollback_index != NULL)) {
    *out_rollback_index = (uint64_t)swVersion;
  } else {
     debug(">>>>> ERR:return err or para is invalid.\n");
     return AVB_IO_RESULT_ERROR_IO;
  }

  return AVB_IO_RESULT_OK;
}

AvbIOResult write_rollback_index(AvbOps* ops,
                                        size_t rollback_index_slot,
                                        uint64_t rollback_index) {
  /* For now this is a no-op. */
  avb_debug("TODO: implement write_rollback_index().\n");
  return AVB_IO_RESULT_OK;
}

AvbIOResult read_is_device_unlocked(AvbOps* ops, bool* out_is_unlocked) {
  /* For now we always return that the device is unlocked. */
  avb_debug("TODO: implement read_is_device_unlocked().\n");
  *out_is_unlocked = false; //true;
  return AVB_IO_RESULT_OK;
}

AvbIOResult get_unique_guid_for_partition(AvbOps* ops,
                                                 const char* partition,
                                                 char* guid_buf,
                                                 size_t guid_buf_size) {
  return AVB_IO_RESULT_OK;
}

int avb_ops_new_for_download(void)
{
  g_avb_ops.ab_ops = NULL;
  g_avb_ops.read_from_partition = read_from_partition;
  g_avb_ops.write_to_partition = NULL;
  g_avb_ops.validate_vbmeta_public_key = validate_vbmeta_public_key;
  g_avb_ops.read_rollback_index = read_rollback_index;
  g_avb_ops.write_rollback_index = write_rollback_index;
  g_avb_ops.read_is_device_unlocked = read_is_device_unlocked;
  g_avb_ops.get_unique_guid_for_partition = get_unique_guid_for_partition;
  g_avb_ops.user_data = 0x00;

  g_ulAvbCheckType = AVB_CHECK_DOWNLOAD;
   return 0;
}

int avb_ops_new(void)
{
  g_avb_ops.ab_ops = NULL;
  g_avb_ops.read_from_partition = read_from_partition;
  g_avb_ops.write_to_partition = NULL;
  g_avb_ops.validate_vbmeta_public_key = validate_vbmeta_public_key;
  g_avb_ops.read_rollback_index = read_rollback_index;
  g_avb_ops.write_rollback_index = write_rollback_index;
  g_avb_ops.read_is_device_unlocked = read_is_device_unlocked;
  g_avb_ops.get_unique_guid_for_partition = get_unique_guid_for_partition;
  g_avb_ops.user_data = 0x00;

  g_ulAvbCheckType = AVB_CHECK_BOOTING;
   return 0;
}

uint8_t* avb_ops_get_image_buffer(uint8_t avb_check_type)
{
	if(avb_check_type == AVB_CHECK_BOOTING) {
		return (uint8_t*)KERNEL_ADR;
	} else {
		return (uint8_t*)(g_eMMCBuf + 0x04000000); /*g_emmcbuf + 64MB*/
	}
}

/*for uboot*/
int avb_ops_set_vboot_key(uint8_t * avb_vboot_key, uint32_t avb_vboot_key_len)
{
	if(avb_vboot_key_len > sizeof(g_sprd_vboot_key)) {
		avb_debug("in avb_ops_set_vboot_key, para size is overflow.\n");
		return -1;
	}
	memset(g_sprd_vboot_key, 0, sizeof(g_sprd_vboot_key));
	memcpy(g_sprd_vboot_key, avb_vboot_key, avb_vboot_key_len);
	g_sprd_vboot_key_len = sizeof(g_sprd_vboot_key);
	return 0;
}

/*for avb ops*/
uint8_t* avb_ops_get_vboot_key(uint32_t * avb_vboot_key_len)
{
	*avb_vboot_key_len = g_sprd_vboot_key_len;
	return (uint8_t*)g_sprd_vboot_key;
}
#if 0
int avb_ops_vbmeta_image_verify(const uint8_t * data, size_t length, const uint8_t** out_public_key_data, size_t* out_public_key_length)
{
	int result = 0;

	result = avb_vbmeta_image_verify(data, length, out_public_key_data, out_public_key_length);
	return result;
}
#endif
