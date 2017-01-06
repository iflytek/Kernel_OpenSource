#include <common.h>
#include <android_bootimg.h>
#include "loader_common.h"
#include <malloc.h>
#include <ext_common.h>
#include <ext4fs.h>
#include <linux/sizes.h>
#include "dl_operate.h"
#include "sprd_common_rw.h"

#include <secureboot/sprdsec_header.h>
#include <secureboot/sec_common.h>
#include <otp_helper.h>
#include <stdio_dev.h>

#ifdef CONFIG_ARM7_RAM_ACTIVE
    extern void pmic_arm7_RAM_active(void);
#endif

#ifdef CONFIG_MINI_TRUSTZONE
#include "trustzone_def.h"
#endif

#ifdef CONFIG_SECURE_BOOT
#include "secure_boot.h"
#include "secure_verify.h"
#include <linux/types.h>
#endif

#ifdef CONFIG_OF_LIBFDT
#include <libfdt.h>
#include "dev_tree.h"
#endif
extern int lcd_line_length;

extern int fdt_fixup_memory_region(void *fdt, int *auto_mem_num_p);
extern int get_dt_end_addr(void *fdt, uchar **addr);

#define MODEM_MAGIC		"SCI1"
#define MODEM_HDR_SIZE		(12 * 512) //size of a block
#define SCI_TYPE_MODEM_BIN	1
#define SCI_TYPE_PARSING_LIB	2
#define MODEM_LAST_HDR		0x100
#define MODEM_SHA1_HDR		0x400
#define MODEM_SHA1_SIZE		20

#define FREE_RAM_SPACE_ADDR		0x9d000000 /* 0x9d000000 ~9f000000  30M Bytes Free*/

typedef struct {
	uint32_t type_flags;
	uint32_t offset;
	uint32_t length;
} data_block_header_t;

typedef struct _BOOT_IMG_OFFSET {
	uint64_t kernel_offset;
	uint64_t ramdisk_offset;
	uint64_t dt_offset;
}BOOT_IMG_OFFSET;
static BOOT_IMG_OFFSET boot_img_offset;

#ifdef CONFIG_SUPPORT_TDLTE
static boot_image_required_t const s_boot_image_tl_table[] = {
#if !defined( CONFIG_KERNEL_BOOT_CP )
	{"tl_fixnv1", "tl_fixnv2", LTE_FIXNV_SIZE, LTE_FIXNV_ADDR},
	{"tl_runtimenv1", "tl_runtimenv2", LTE_RUNNV_SIZE, LTE_RUNNV_ADDR},
	{"tl_modem", NULL, LTE_MODEM_SIZE, LTE_MODEM_ADDR},
	{"tl_ldsp", NULL, LTE_LDSP_SIZE, LTE_LDSP_ADDR},	//ltedsp
	{"tl_tgdsp", NULL, LTE_GDSP_SIZE, LTE_GDSP_ADDR},
#endif
	{NULL, NULL, 0, 0}
};
#endif

#ifdef CONFIG_SUPPORT_WLTE
static boot_image_required_t const s_boot_image_wl_table[] = {
#if !defined( CONFIG_KERNEL_BOOT_CP )
	{"wl_fixnv1", "wl_fixnv2", LTE_FIXNV_SIZE, LTE_FIXNV_ADDR},
	{"wl_runtimenv1", "wl_runtimenv2", LTE_RUNNV_SIZE, LTE_RUNNV_ADDR},
	{"wl_modem", NULL, LTE_MODEM_SIZE, LTE_MODEM_ADDR},
	{"wl_ldsp", NULL, LTE_LDSP_SIZE, LTE_LDSP_ADDR},
	{"wl_gdsp", NULL, LTE_GDSP_SIZE, LTE_GDSP_ADDR},
	{"wl_warm", NULL, WL_WARM_SIZE, WL_WARM_ADDR},
#endif
	{NULL, NULL, 0, 0}
};
#endif

#ifdef CONFIG_SUPPORT_GSM
static boot_image_required_t const s_boot_image_gsm_table[] = {
#if !defined( CONFIG_KERNEL_BOOT_CP )
	{"g_fixnv1", "g_fixnv2", GSM_FIXNV_SIZE, GSM_FIXNV_ADDR},
	{"g_runtimenv1", "g_runtimenv2", GSM_RUNNV_SIZE, GSM_RUNNV_ADDR},
	{"g_modem", NULL, GSM_MODEM_SIZE, GSM_MODEM_ADDR},
	{"g_dsp", NULL, GSM_DSP_SIZE, GSM_DSP_ADDR},
#endif
	{NULL, NULL, 0, 0}
};
#endif

#ifdef CONFIG_SUPPORT_LTE
static boot_image_required_t const s_boot_image_lte_table[] = {
#if (!defined( CONFIG_KERNEL_BOOT_CP )) && (!defined(CONFIG_MEM_LAYOUT_DECOUPLING))
#ifdef CONFIG_ADVANCED_LTE
	{"l_fixnv1","l_fixnv2",LTE_FIXNV_SIZE,LTE_FIXNV_ADDR},
	{"l_runtimenv1","l_runtimenv2",LTE_RUNNV_SIZE,LTE_RUNNV_ADDR},
	{"l_modem",NULL,LTE_MODEM_SIZE,LTE_MODEM_ADDR},
	{"l_tgdsp",NULL,LTE_TGDSP_SIZE ,LTE_TGDSP_ADDR}, //tddsp
	{"l_ldsp",NULL,LTE_LDSP_SIZE ,LTE_LDSP_ADDR},
#else
	{"l_fixnv1", "l_fixnv2", LTE_FIXNV_SIZE, LTE_FIXNV_ADDR},
	{"l_runtimenv1", "l_runtimenv2", LTE_RUNNV_SIZE, LTE_RUNNV_ADDR},
	{"l_modem", NULL, LTE_MODEM_SIZE, LTE_MODEM_ADDR},
	{"l_ldsp", NULL, LTE_LDSP_SIZE, LTE_LDSP_ADDR},
	{"l_gdsp", NULL, LTE_GDSP_SIZE, LTE_GDSP_ADDR},
	{"l_warm", NULL, WL_WARM_SIZE, WL_WARM_ADDR},
#endif
#endif
#ifdef  CONFIG_ADVANCED_LTE
	{"l_agdsp",NULL,LTE_AGDSP_SIZE ,LTE_AGDSP_ADDR}, //agdsp now is boot by uboot
#endif
	{NULL, NULL, 0, 0}
};
#endif

#ifdef CONFIG_SUPPORT_TD
static boot_image_required_t const s_boot_image_TD_table[] = {
#if !defined( CONFIG_KERNEL_BOOT_CP )
	{"tdfixnv1", "tdfixnv2", FIXNV_SIZE, TDFIXNV_ADR},
	{"tdruntimenv1", "tdruntimenv2", RUNTIMENV_SIZE, TDRUNTIMENV_ADR},
	{"tdmodem", NULL, TDMODEM_SIZE, TDMODEM_ADR},
	{"tddsp", NULL, TDDSP_SIZE, TDDSP_ADR},
#endif
	{NULL, NULL, 0, 0}
};
#endif

#ifdef CONFIG_SUPPORT_W
static boot_image_required_t const s_boot_image_W_table[] = {
#if !defined( CONFIG_KERNEL_BOOT_CP )
	{"wfixnv1", "wfixnv2", FIXNV_SIZE, WFIXNV_ADR},
	{"wruntimenv1", "wruntimenv2", RUNTIMENV_SIZE, WRUNTIMENV_ADR},
	{"wmodem", NULL, WMODEM_SIZE, WMODEM_ADR},
	{"wdsp", NULL, WDSP_SIZE, WDSP_ADR},
#endif
	{NULL, NULL, 0, 0}
};
#endif

#ifdef CONFIG_SUPPORT_WIFI
static boot_image_required_t const s_boot_image_WIFI_table[] = {
	{"wcnfixnv1", "wcnfixnv2", FIXNV_SIZE, WCNFIXNV_ADR},
	{"wcnruntimenv1", "wcnruntimenv2", RUNTIMENV_SIZE, WCNRUNTIMENV_ADR},
	{"wcnmodem", NULL, WCNMODEM_SIZE, WCNMODEM_ADR},
	{NULL, NULL, 0, 0}
};
#endif

static boot_image_required_t const s_boot_image_COMMON_table[] = {
#ifdef CONFIG_SIMLOCK_ENABLE
	{"simlock", NULL, SIMLOCK_SIZE, SIMLOCK_ADR},
#endif
#ifdef CONFIG_DFS_ENABLE
#if !defined( CONFIG_KERNEL_BOOT_CP )
	{"pm_sys", NULL, DFS_SIZE, DFS_ADDR},
#endif
#endif
	{NULL, NULL, 0, 0}

};

#ifdef CONFIG_MINI_TRUSTZONE
static boot_image_required_t const s_boot_image_TZ_table[] = {
	{"sml", NULL, TRUSTRAM_SIZE, TRUSTRAM_ADR},
	{NULL, NULL, 0, 0}

};
#endif

static boot_image_required_t *const s_boot_image_table[] = {
#ifdef CONFIG_SUPPORT_TDLTE
	s_boot_image_tl_table,
#endif

#ifdef CONFIG_SUPPORT_WLTE
	s_boot_image_wl_table,
#endif

#ifdef CONFIG_SUPPORT_LTE
	s_boot_image_lte_table,
#endif

#ifdef CONFIG_SUPPORT_GSM
	s_boot_image_gsm_table,
#endif

#ifdef CONFIG_SUPPORT_TD
	s_boot_image_TD_table,
#endif

#ifdef CONFIG_SUPPORT_W
	s_boot_image_W_table,
#endif

#ifdef CONFIG_SUPPORT_WIFI
	s_boot_image_WIFI_table,
#endif
#ifdef CONFIG_MINI_TRUSTZONE
	s_boot_image_TZ_table,
#endif
	s_boot_image_COMMON_table,

	0
};

#ifdef CONFIG_SECURE_BOOT
uint8_t header_buf[SEC_HEADER_MAX_SIZE];
#endif

#ifdef CONFIG_SECURE_BOOT

int get_spl_hash(void *hash_data)
{
	NBLHeader *header;
	unsigned int len;
	uint8_t *spl_data;
	int ret = 0;
	int size = CONFIG_SPL_HASH_LEN;
	spl_data = malloc(size);
	if (!spl_data) {
		return ret;
	}

	if (0 != common_raw_read("splloader", (uint64_t)size, (uint64_t)0, (char *) spl_data)) {
		errorf("PARTITION_BOOT1 read error \n");
		return ret;
	}

	header = (NBLHeader *) ((uint8_t *) spl_data + BOOTLOADER_HEADER_OFFSET);
	len = header->mHashLen;
	/*clear header */
	memset(header, 0, sizeof(NBLHeader));
	header->mHashLen = len;
	debugf("cal spl hash len=%d\n", header->mHashLen * 4);
	ret = cal_sha1(spl_data, (header->mHashLen) << 2, hash_data);

	if (spl_data)
		free(spl_data);

	return ret;
}
#endif
void _boot_secure_check(void)
{
#ifdef SECURE_BOOT_ENABLE
	secure_check(DSP_ADR, 0, DSP_ADR + DSP_SIZE - VLR_INFO_OFF, CONFIG_SYS_NAND_U_BOOT_DST + CONFIG_SYS_NAND_U_BOOT_SIZE - KEY_INFO_SIZ - VLR_INFO_OFF);
	secure_check(MODEM_ADR, 0, MODEM_ADR + MODEM_SIZE - VLR_INFO_OFF,
		     CONFIG_SYS_NAND_U_BOOT_DST + CONFIG_SYS_NAND_U_BOOT_SIZE - KEY_INFO_SIZ - VLR_INFO_OFF);
#ifdef CONFIG_SIMLOCK
	secure_check(SIMLOCK_ADR, 0, SIMLOCK_ADR + SIMLOCK_SIZE - VLR_INFO_OFF,
		     CONFIG_SYS_NAND_U_BOOT_DST + CONFIG_SYS_NAND_U_BOOT_SIZE - KEY_INFO_SIZ - VLR_INFO_OFF);
#endif
#endif
	return;
}

int splash_screen_prepare(uchar *logo_part_name, u8 *addr)
{
	size_t size ;
	u32 bmp_header_size = 8192;

	size = lcd_get_size(&lcd_line_length) >> 1;
	size += bmp_header_size;
	if (0 != common_raw_read(logo_part_name, (uint64_t)size, (uint64_t)0, addr)) {
		errorf("failed to read logo partition:%s\n", logo_part_name);
		return -1;
	}
	return 0;
}

int32_t _boot_read_partition_with_backup(boot_image_required_t info)
{
	uchar header_buf[NV_HEADER_SIZE];
	uchar backup_header_buf[NV_HEADER_SIZE];
	nv_header_t *header_p = header_buf;
	nv_header_t *backup_header_p = backup_header_buf;
	uchar * backup_nv_buf = NULL;
	int status = ORIGIN_BACKUP_NV_OK;
	int ret = 0;

	backup_nv_buf = malloc(info.size);
	if (NULL == backup_nv_buf) {
		errorf("no enough space for backup nv buffer\n");
		return 0;
	}

	do {
		/*read origin image header */
		if (0 != common_raw_read(info.partition, NV_HEADER_SIZE, (uint64_t)0, header_buf)) {
			errorf("read origin <<<%s>>> image header failed\n", info.partition);
			status |= ORIGIN_NV_DAMAGED;
			break;
		}

		if (NV_HEAD_MAGIC != header_p->magic) {
			errorf("<<<%s>>> header magic error, wrong magic=0x%x\n", info.partition, header_p->magic);
			status |= ORIGIN_NV_DAMAGED;
			break;
		}

		/*read origin image */
		if (0 != common_raw_read(info.partition, (uint64_t)(info.size), NV_HEADER_SIZE, info.mem_addr)) {
			errorf("read origin <<<%s>>> image data failed\n", info.partition);
			status |= ORIGIN_NV_DAMAGED;
			break;
		}

		/*check crc */
		if (_chkNVEcc(info.mem_addr, info.size, header_p->checksum)) {
			debugf("read origin <<<%s>>> image success and crc correct\n", info.partition);
		} else {
			errorf("check origin <<<%s>>> image crc wrong\n", info.partition);
			status |= ORIGIN_NV_DAMAGED;
		}
	} while(0);

	do {
		/*read backup header */
		if (0 != common_raw_read(info.bak_partition, NV_HEADER_SIZE, (uint64_t)0, backup_header_buf)) {
			errorf("read backup <<<%s>>> image header failed\n", info.bak_partition);
			status |= BACKUP_NV_DAMAGED;
			break;
		}

		if (NV_HEAD_MAGIC != backup_header_p->magic) {
			errorf("backup <<<%s>>> header magic error, wrong magic=0x%x\n", info.bak_partition, backup_header_p->magic);
			status |= BACKUP_NV_DAMAGED;
			break;
		}

		/*read bakup image */
		if (0 != common_raw_read(info.bak_partition, (uint64_t)(info.size), NV_HEADER_SIZE, backup_nv_buf)) {
			errorf("read backup <<<%s>>> image failed\n", info.bak_partition);
			status |= BACKUP_NV_DAMAGED;
			break;
		}

		/*check crc */
		if (_chkNVEcc(backup_nv_buf, info.size, backup_header_p->checksum)) {
			debugf("read backup <<<%s>>> image success and crc correct\n", info.bak_partition);
		} else {
			errorf("check backup <<<%s>>> image crc wrong\n", info.bak_partition);
			status |= BACKUP_NV_DAMAGED;
		}
	} while(0);

	switch (status) {
	case ORIGIN_BACKUP_NV_OK :
		debugf("both org and bak nv partition are ok\n");
		ret = 1;
		break;
	case ORIGIN_NV_DAMAGED :
		memcpy(info.mem_addr, backup_nv_buf, info.size);
		if (0 != common_raw_write(info.partition, NV_HEADER_SIZE, (uint64_t)0, (uint64_t)0, backup_header_buf)) {
			errorf("reapire origin <<<%s>>> image header fail\n", info.partition);
			ret = 0;
			break;
		}

		if (0 != common_raw_write(info.partition, (uint64_t)(info.size), (uint64_t)0, NV_HEADER_SIZE, backup_nv_buf)) {
			errorf("reapire origin <<<%s>>> image data fail\n", info.partition);
			ret = 0;
			break;
		}

		ret = 1;
		break;
	case BACKUP_NV_DAMAGED :
		if (0 != common_raw_write(info.bak_partition, NV_HEADER_SIZE, (uint64_t)0, (uint64_t)0, header_buf)) {
			errorf("reapire backup <<<%s>>> image header fail\n", info.bak_partition);
			ret = 0;
			break;
		}

		if (0 != common_raw_write(info.bak_partition, (uint64_t)(info.size), (uint64_t)0, NV_HEADER_SIZE, info.mem_addr)) {
			errorf("reapire backup <<<%s>>> image data fail\n", info.bak_partition);
			ret = 0;
			break;
		}
		debugf("repair backup <<<%s>>> image OK\n", info.bak_partition);
		ret = 1;
		break;
	case ORIGIN_NV_DAMAGED | BACKUP_NV_DAMAGED :
		errorf("both org <<<%s>>> and bak <<<%s>>> partition are damaged!\n", info.partition, info.bak_partition);
		ret = 0;
		break;
	}

	free(backup_nv_buf);
	return ret;
}

uint32_t get_boot_offset(boot_image_required_t img_info, uint32_t secure_offset)
{
	uint32_t offset = 0;
	uint32_t hdr_offset = 0;
	data_block_header_t hdr_buf[8]  __attribute__ ((packed));
	uint32_t read_len = sizeof(hdr_buf);
	uint8_t result = 0; // 0:OK, 1:not find, 2:some error occur
	data_block_header_t* hdr_ptr = NULL;

	if(strstr(img_info.partition, "modem")) {
		do {
			if (0 != common_raw_read(img_info.partition, (uint64_t)read_len, (uint64_t)(hdr_offset + secure_offset), (char *)hdr_buf)) {
				result = 2;
				errorf("get modem boot hdr failed: partitionread failed !\n");
				break;
			}

			if(!hdr_offset) {
				if(memcmp((uint8_t*)hdr_buf, MODEM_MAGIC, strlen(MODEM_MAGIC))) {
					result = 2;
					debugf("old image format!\n");
					break;
				}

				hdr_ptr = hdr_buf + 1;
				hdr_offset = MODEM_HDR_SIZE;
			} else {
				hdr_ptr = hdr_buf;
			}

			size_t n = sizeof hdr_buf / sizeof(data_block_header_t);
			data_block_header_t* startp = hdr_ptr;
			data_block_header_t* endp = hdr_buf + n;
			int found = 0;

			while (hdr_ptr < endp) {
				uint32_t type = (hdr_ptr->type_flags & 0xff);
				if (SCI_TYPE_MODEM_BIN == type) {
					// MODEM image
					found = 1;
					break;
				}
				// To work with both the right SCI headers
				// and the wrong SCI headers, comment out
				// the following.
				// This is just a workaround rather than a
				// formal fix. After the MODEM SCI image
				// generator is fixed, uncomment it.
#if 0
				if(hdr_ptr->type_flags & MODEM_LAST_HDR) {
					// No MODEM image
					result = 2;
					debugf("no modem image, error image header!!!\n");
					break;
				}
#endif
				++hdr_ptr;
			}
			if (!found) {  // No MODEM image found
				if (hdr_ptr == endp) {  // Need to read more SCI headers
					hdr_offset += (hdr_ptr - startp);
					result = 1;
				} else {
					result = 2;
					debugf("no modem image, error image header!!!\n");
				}
			}

			if(result != 1) {
				break;
			}

		} while(1);

		if(!result) {
			offset = hdr_ptr->offset;
			if(hdr_ptr->type_flags & MODEM_SHA1_HDR) {
				offset += MODEM_SHA1_SIZE;
			}
		}
	}

	debugf("Modem offset Bytes: 0x%x!\n",offset);

	return offset;

}

/**
	Function for reading image which is needed when power on.
*/
int _boot_load_required_image(boot_image_required_t img_info)
{
	uint32_t secure_boot_offset = 0;
	uint32_t normal_boot_offset = 0;

	debugf("load %s to addr 0x%08x\n", img_info.partition, img_info.mem_addr);

	if (NULL != img_info.bak_partition) {
		debugf("load %s with backup img %s\n", img_info.partition, img_info.bak_partition);
		_boot_read_partition_with_backup(img_info);
	} else {
#if defined (CONFIG_SECURE_BOOT)
		if (!_boot_partition_read(dev, img_info.partition, 0, SEC_HEADER_MAX_SIZE, header_buf)) {
			debugf("%s:%s read error!\n", img_info.partition);
			return 0;
		}
		//if(header_parser(header_buf) )
		secure_boot_offset = get_code_offset(header_buf);
		normal_boot_offset = get_boot_offset(dev, img_info, secure_boot_offset);

		/*_boot_partition_read(dev, img_info.partition, normal_boot_offset + secure_boot_offset, img_info.size, (u8 *) img_info.mem_addr);*/
		_boot_partition_read(dev, img_info.partition, secure_boot_offset, img_info.size + (normal_boot_offset * EMMC_SECTOR_SIZE), (u8 *)FREE_RAM_SPACE_ADDR);

		/*secure_verify("uboot", header_buf, img_info.mem_addr);*/
		secure_verify("uboot", header_buf, (u8 *)FREE_RAM_SPACE_ADDR);
		memcpy((u8 *)img_info.mem_addr, (u8 *)FREE_RAM_SPACE_ADDR + (normal_boot_offset * EMMC_SECTOR_SIZE), img_info.size);
#elif defined (CONFIG_SECBOOT)
		/***secboot 2nd step***/
		secboot_secure_process_flow(img_info.partition,(img_info.size + CERT_SIZE),0,(char *)VERIFY_ADDR);
		memcpy(img_info.mem_addr,(VERIFY_ADDR + SYS_HEADER_SIZE),img_info.size);
#else
		normal_boot_offset = get_boot_offset(img_info, 0);
		if (0 != common_raw_read(img_info.partition, (uint64_t)(img_info.size), (uint64_t)normal_boot_offset, img_info.mem_addr))
			errorf("read %s partition fail\n", img_info.partition);

#endif
	}

	return 1;
}

uint64_t _get_kernel_ramdisk_dt_offset( boot_img_hdr * hdr, uchar *partition)
{
	uint64_t size;
	uint64_t offset = 0;
	uint32_t secure_boot_offset = 0;

#ifdef CONFIG_SECURE_BOOT
	if (0 != common_raw_read(partition, (uint64_t)512, (uint64_t)0,  (u8 *) hdr)) {
		errorf("%s read error!\n", partition);
		return 0;
	}
	secure_boot_offset = get_code_offset(hdr);
	offset += secure_boot_offset;
#endif
#ifdef CONFIG_SECBOOT
	offset = 512;/**header of image size is 512B**/
#endif
	if (0 != common_raw_read(partition, KERNL_PAGE_SIZE, (uint64_t)offset, (char *)hdr)) {
		errorf("read %s header error!\n", partition);
		return 0;
	}

	/*image header check */
	if (0 != memcmp(hdr->magic, BOOT_MAGIC, BOOT_MAGIC_SIZE)) {
		errorf("bad boot image header, give up boot!!!!\n");
		return 0;
	}
	/*kernel image */
	boot_img_offset.kernel_offset = offset + KERNL_PAGE_SIZE;
	size = PAD_SIZE(hdr->kernel_size, KERNL_PAGE_SIZE);
	if (0 == size) {
		errorf("kernel image should not be zero!\n");
		return 0;
	}

	/* ramdisk image */
	boot_img_offset.ramdisk_offset = boot_img_offset.kernel_offset + size;
	size = PAD_SIZE(hdr->ramdisk_size, KERNL_PAGE_SIZE);
	if (0 == size) {
		errorf("ramdisk image size should not be zero\n");
		return 0;
	}
	/* dt image */
	boot_img_offset.dt_offset = boot_img_offset.ramdisk_offset + size;
	return 1;
}

/**
	Function for do memory defragment and load dtb
*/
int load_fixup_dt_img(uchar *partition, uchar **dt_start_addr) {
	u8 *fdt_blob = NULL;
	struct dt_table table;
	char * header = NULL;
	struct dt_entry *dt_entry_ptr;
	uint64_t size = 0;
	uint64_t fdt_size = 0;
	int ret = -1;
	uchar *dt_end_addr;
	uchar *ramdisk_adr;
	uint32_t auto_mem_num = 0;

	size = sizeof(struct dt_table);
	if (0 != common_raw_read(partition, size, (uint64_t)(boot_img_offset.dt_offset), &table)) {
		errorf("read dt image table header fail\n");
		goto error;
	}

	/* Validate the device tree table header */
	if((table.magic != DEV_TREE_MAGIC) || (table.version != DEV_TREE_VERSION)) {
		errorf("Cannot validate Device Tree Table \n");
		goto error;
	}

	size = sizeof(struct dt_table) + sizeof(struct dt_entry) * table.num_entries;
	header = (char *)malloc(size);
	if (NULL == header) {
		errorf("malloc size %d for dt header fail\n", size);
		goto error;
	}
	if (0 != common_raw_read(partition, size,  (uint64_t)(boot_img_offset.dt_offset), header)) {
		errorf("read dt image table and entries header fail\n");
		goto error;
	}

	/* Calculate the offset of device tree within device tree table */
	dt_entry_ptr = dev_tree_get_entry_ptr((struct dt_table *)header);
	if(NULL == dt_entry_ptr) {
		errorf("Getting device tree address failed\n");
		goto error;
	}

	size = dt_entry_ptr->size;
	/* malloc reserve FDT_ADD_SIZE for fdt fixup */
	fdt_blob = malloc(size + FDT_ADD_SIZE);
	debugf("malloc size for fdt: 0x%lx\n", size + FDT_ADD_SIZE);
	if (0 != common_raw_read(partition, (uint64_t)size, (uint64_t)(boot_img_offset.dt_offset + dt_entry_ptr->offset), (char *)fdt_blob)) {
		errorf("dt entry size read error!\n");
		goto error;
	}

	if (fdt_check_header(fdt_blob) != 0) {
		errorf("image is not a fdt\n");
		goto error;
	}
	fdt_size = fdt_totalsize(fdt_blob);
	ret = fdt_open_into(fdt_blob, fdt_blob, fdt_size + FDT_ADD_SIZE);
	if (0 != ret) {
		errorf("libfdt fdt_open_into(): %s\n", fdt_strerror(ret));
		goto error;
	}
#ifdef CONFIG_X86
	fdt_fixup_all_x86(fdt_blob);
#else
	fdt_fixup_all(fdt_blob);
#endif
	ret = fdt_fixup_memory_region(fdt_blob, &auto_mem_num);
	if(ret < 0)
		goto error;
	fdt_size = fdt_totalsize(fdt_blob);
	size = PAD_SIZE(fdt_size, KERNL_PAGE_SIZE);
	if (0 == ret && 0 != auto_mem_num) {
		if(0 != get_dt_end_addr(fdt_blob , &dt_end_addr))
			goto end;
		*dt_start_addr = dt_end_addr - size;
	}
end:
	memcpy(*dt_start_addr, fdt_blob, size);
	ret = 0;
error:
	if (NULL != header)
		free(header);
	if (NULL != fdt_blob)
		free(fdt_blob);
	return ret;
}

int _boot_load_kernel_ramdisk_image(char *bootmode, boot_img_hdr * hdr, uchar **dt_addr )
{
	uchar *partition = NULL;
	uint64_t size;
	uchar *dt_img_adr;
	char  *boot_mode_type_str;
	uchar *ramdisk_addr = 0;
	uint64_t fdt_size = 0;
	int ret;

	if (0 == memcmp(bootmode, RECOVERY_PART, strlen(RECOVERY_PART))) {
		partition = "recovery";
		debugf("enter recovery mode!\n");
	} else {
		partition = "boot";
		debugf("enter boot mode!\n");
	}

	if(0 == _get_kernel_ramdisk_dt_offset(hdr, partition))
		return 0;
	/* get kernel image size */
	size = PAD_SIZE(hdr->kernel_size, KERNL_PAGE_SIZE);
	if (0 == size) {
		errorf("kernel image should not be zero!\n");
		return 0;
	}

	/* read kernel image */
	if (0 != common_raw_read(partition, size, (uint64_t)(boot_img_offset.kernel_offset) , (char *)KERNEL_ADR)) {
		errorf("%s kernel read error!\n", partition);
		return 0;
	}
	debugf("%s kernel read OK,size=0x%llx, locate to 0x%lx \n", partition, size, (ulong)KERNEL_ADR);

	ramdisk_addr = RAMDISK_ADR;
	/* get ramdisk image size */
	size = PAD_SIZE(hdr->ramdisk_size, KERNL_PAGE_SIZE);
	if (0 == size) {
		errorf("ramdisk image size should not be zero\n");
		return 0;
	}

/*read dt image*/
#if defined(CONFIG_SEPARATE_DT)
	if (!_boot_load_separate_dt())
		return 0;
	fdt_size = fdt_totalsize(DT_ADR);
	ret = fdt_open_into(DT_ADR, DT_ADR, fdt_size + FDT_ADD_SIZE);
	if (0 != ret)
		errorf("libfdt fdt_open_into(): %s\n", fdt_strerror(ret));
	fdt_fixup_all((uchar *)DT_ADR);
#else
	/* for memory defragment, dr_addr and ramdisk_adr is allocated dynamically*/
	if (0 == load_fixup_dt_img(partition, dt_addr)) {
		if (DT_ADR != *dt_addr) {
			debugf("allocate dt_addr:%lx \n", *dt_addr);
			/*allocate  ramdisk addr */
			ramdisk_addr = *dt_addr - size;
		} else {
			debugf(" use default value : DT_ADDR & RAMDISK_ADR\n");
		}
	} else {
		errorf("load dt error!\n");
		return 0;
	}
#endif
	fdt_initrd_norsvmem(*dt_addr, ramdisk_addr, ramdisk_addr + hdr->ramdisk_size, 1);
	if (0 == size) {
		errorf("ramdisk image size should not be zero\n");
		return 0;
	}
	/*read ramdisk image */
	if (0 != common_raw_read(partition, size, (uint64_t)(boot_img_offset.ramdisk_offset), ramdisk_addr)) {
		errorf("%s ramdisk read error!\n", partition);
		return 0;
	}
	debugf("%s ramdisk read OK,size=0x%llx, locate to 0x%lx \n", partition, size, ramdisk_addr);

	boot_mode_type_str = getenv("bootmode");
	if (NULL != boot_mode_type_str)
	{
		if(!strncmp(boot_mode_type_str, "sprdisk", 7)) {
			int ramdisk_size;
			ramdisk_size = boot_sprdisk(size, ramdisk_addr, 0);
			if (ramdisk_size > 0) {
				hdr->ramdisk_size = ramdisk_size;
				if (RAMDISK_ADR != ramdisk_addr) {
					/* get ramdisk size , calculate addr, so we can have enough room for sprdisk ramdisk */
					size = PAD_SIZE(hdr->ramdisk_size, KERNL_PAGE_SIZE);
					ramdisk_addr = *dt_addr - size;
					hdr->ramdisk_addr = ramdisk_addr;
				}
				fdt_initrd_norsvmem(*dt_addr, ramdisk_addr, ramdisk_addr+hdr->ramdisk_size , 1);
				boot_sprdisk(ramdisk_size, ramdisk_addr, 1);
			} else {
				errorf("%s, sprdisk mode failure!\n", __FUNCTION__);
			}
			return 1;
		}
	}


#ifdef CONFIG_SDRAMDISK
	{
		int sd_ramdisk_size = 0;
#ifdef WDSP_ADR
		size = WDSP_ADR - RAMDISK_ADR;
#else
		size = TDDSP_ADR - RAMDISK_ADR;
#endif
		if (size > 0)
			sd_ramdisk_size = load_sd_ramdisk((void *) RAMDISK_ADR, size);
		if (sd_ramdisk_size > 0)
			hdr->ramdisk_size = sd_ramdisk_size;
	}
#endif
	return 1;
}


#ifdef CONFIG_SEPARATE_DT
int _boot_load_separate_dt(void)
{
	struct dt_table table;
	char * header = NULL;
	struct dt_entry *dt_entry_ptr;
	uint32_t secure_boot_offset = 0;
	uint64_t offset = 0;
	uint64_t size = 0;

#ifdef CONFIG_SECURE_BOOT
	if (0 != common_raw_read("dt", EMMC_SECTOR_SIZE, (uint64_t)0, &table)) {
		errorf("dt read error!\n");
		free(table);
		return 0;
	}
	secure_boot_offset = get_code_offset(table);
	offset += secure_boot_offset;
#endif
#ifdef CONFIG_SECBOOT
	offset = 512;/**header of image size is 512B**/
#endif

	size = sizeof(struct dt_table);
	if (0 != common_raw_read(DT_PART, size, offset, &table)) {
		errorf("read dt image table header fail\n");
		goto fail;
	}

	/* Validate the device tree table header */
	if((table.magic != DEV_TREE_MAGIC) || (table.version != DEV_TREE_VERSION)) {
		errorf("Cannot validate Device Tree Table \n");
		goto fail;
	}

	size = sizeof(struct dt_table) + sizeof(struct dt_entry) * table.num_entries;
	header = (char *)malloc(size);
	if (NULL == header) {
		errorf("malloc size %d for dt header fail\n", size);
		goto fail;
	}
	if (0 != common_raw_read(DT_PART, size, offset, header)) {
		errorf("read dt image table and entries header fail\n");
		goto fail;
	}

	/* Calculate the offset of device tree within device tree table */
	dt_entry_ptr = dev_tree_get_entry_ptr((struct dt_table *)header);
	if(NULL == dt_entry_ptr) {
		errorf("Getting device tree address failed\n");
		goto fail;
	}

	size = dt_entry_ptr->size;
	offset += dt_entry_ptr->offset;
	if (0 != common_raw_read(DT_PART, size, offset, (char *)DT_ADR)) {
		errorf("dt entry size 0x%x offset 0x%x read error!\n", size, offset);
		goto fail;
	}

	debugf("load separate DT OK,size=0x%llx, locate to 0x%lx \n", size, (ulong)DT_ADR);
	free(header);
	return 1;

fail:
	if (NULL != header)
		free(header);
	return 0;
}
#endif

/*TODO*/
#ifdef CONFIG_SECURE_BOOT
int secure_verify_partition(block_dev_desc_t * dev, uchar * partition_name, void *ram_addr)
{
	int ret = 0;
	int size;
	disk_partition_t info;

	if (get_partition_info_by_name(dev, partition_name, &info)) {
		errorf("verify get partition %s info failed!\n", partition_name);
		ret = 1;
	}
	size = info.size * EMMC_SECTOR_SIZE;
	debugf("%s=%x  =%x\n", partition_name, info.size, size);
	_boot_partition_read(dev, partition_name, 0, size, (uint8_t *) ram_addr);
	secure_verify("uboot", (uint8_t *) ram_addr, 0);
	return ret;
}
#endif

#ifdef CONFIG_ZEBU
void vlx_nand_boot_zebu(char *kernel_pname, int backlight_set, int lcd_enable)
{
    struct boot_img_hdr *boot_hdr;
    ulong bzImage_size = 0;
    ulong initrd_size = 0;
    ulong dtb_size = 0;
    ulong dt_img_adr = 0;
	uint64_t fdt_size = 0;
	int ret;

    /* load bootimg to RAM  */
    boot_hdr = malloc(512*4);
    memcpy((void *)boot_hdr,(const void *)BOOTIMG_ADR,512*4);
    if (!memcmp(boot_hdr->magic, BOOT_MAGIC, BOOT_MAGIC_SIZE)) {
	bzImage_size = (boot_hdr->kernel_size + (KERNL_PAGE_SIZE-1))&(~(KERNL_PAGE_SIZE -1));
	initrd_size = (boot_hdr->ramdisk_size + (KERNL_PAGE_SIZE-1))&(~(KERNL_PAGE_SIZE -1));
	dtb_size = (boot_hdr->dt_size + (KERNL_PAGE_SIZE-1))&(~(KERNL_PAGE_SIZE -1));
	memcpy((void *)KERNEL_ADR,(const void *)BOOTIMG_ADR+512*4,bzImage_size);
	memcpy((void *)RAMDISK_ADR,(const void *)BOOTIMG_ADR+512*4+bzImage_size,initrd_size);
	memcpy((void *)DT_ADR,(const void *)BOOTIMG_ADR+512*4+bzImage_size+initrd_size,dtb_size);

	debugf("\nthe kernel is loading in the address:%x,size is %d\n",KERNEL_ADR,bzImage_size);
	debugf("\nthe ramdisk is loading in the address:%x,size is %d\n",RAMDISK_ADR,initrd_size);
	debugf("\nthe dtb is loading in the address:%x,size is %d\n",DT_ADR,dtb_size);
    }
    else {
	errorf("A wrong boot.img here,please check!\n");
    }
	free(boot_hdr);

	/* copy kernel/dt/ramdisk/sml from jointRAM to InterlaceRAM */
#ifdef CONFIG_INTERLACE_RAM_COPY
	memcpy(KERNEL_ADR,SRC_KERNEL_ADR,KERNEL_SIZE);
	memcpy(RAMDISK_ADR,SRC_RAMDISK_ADR,RAMDISK_SIZE);
#endif

#ifdef CONFIG_INTERLACE_RAM_COPY
	debugf("copy kernel to interlace RAM done\n");
#endif
	dt_img_adr = DT_ADR;
	if (load_dtb((uint64_t)DT_ADR,(void*)dt_img_adr)) {
	debugf("dt load error!\n");
	return;
	}

	fdt_size = fdt_totalsize(DT_ADR);
	ret = fdt_open_into(DT_ADR, DT_ADR, fdt_size + FDT_ADD_SIZE);
	if (0 != ret) {
		errorf("libfdt fdt_open_into(): %s\n", fdt_strerror(ret));
	}
	fdt_fixup_all((uchar *)DT_ADR);
	fdt_initrd_norsvmem((uchar *)DT_ADR,RAMDISK_ADR,RAMDISK_ADR+initrd_size, 1);

	vlx_entry(DT_ADR);

}
#endif

void vlx_nand_boot(char *kernel_pname, int backlight_set, int lcd_enable)
{
	boot_img_hdr *hdr = (void *)raw_header;
	char *mode_ptr = NULL;
	uchar *partition = NULL;
	int i = 0;
	int j = 0;
	int ret = 0;
	uchar *dt_adr = DT_ADR;

#ifdef CONFIG_SPLASH_SCREEN
	if(lcd_enable) {
		extern void lcd_enable(void);
		debug("[LCD] Drawing the logo...\n");
		drv_lcd_init();
		lcd_splash(LOGO_PART);
		lcd_enable();
	}
	set_backlight(backlight_set);
#endif
	set_vibrator(0);

#if defined(CONFIG_SECURE_BOOT)||defined(CONFIG_SECBOOT)
	if (0 == memcmp(kernel_pname, RECOVERY_PART, strlen(RECOVERY_PART))) {
		partition = "recovery";
	} else {
		partition = "boot";
	}
#if defined (CONFIG_SECURE_BOOT)
	secure_verify_partition(dev, partition, KERNEL_ADR);
#endif
#if defined (CONFIG_SECBOOT)
	/***secboot only 3 steps***/
	/***secboot 1st step***/
	secboot_init();
	/***secboot 2nd step***/
	secboot_secure_process_flow(partition,0,0,(char *)VERIFY_ADDR);
#endif
#endif
#ifdef OTA_BACKUP_MISC_RECOVERY
	ret = memcmp(kernel_pname, RECOVERY_PART, strlen(RECOVERY_PART));
	if ((ret != 0) || (boot_load_recovery_in_sd(hdr) != 0))
		if (!_boot_load_kernel_ramdisk_image(kernel_pname, hdr, &dt_adr))
			ret = -1;
#else
	/*loader kernel and ramdisk*/
	if (!_boot_load_kernel_ramdisk_image(kernel_pname, hdr, &dt_adr))
		ret = -1;
#endif

	if (-1 == ret)
		return;

	/*load required image which config in table */
	i = 0;
#ifdef CONFIG_ARM7_RAM_ACTIVE
			pmic_arm7_RAM_active();
#endif
	while (s_boot_image_table[i]) {
		j = 0;
		while (s_boot_image_table[i][j].partition) {
			_boot_load_required_image(s_boot_image_table[i][j]);
			j++;
		}
		i++;
	}

#if defined(CONFIG_KERNEL_BOOT_CP)
	extern unsigned int g_charger_mode;
	if(g_charger_mode) {
		boot_image_required_t pm_image = {"pm_sys", NULL, DFS_SIZE, DFS_ADDR};
		_boot_load_required_image(pm_image);
	}
#endif

#if !defined(CONFIG_KERNEL_BOOT_CP) && defined(CONFIG_MEM_LAYOUT_DECOUPLING)
extern boot_image_required_t *get_cp_load_table(void);
	do {
		boot_image_required_t *cp_load_table = NULL;
		cp_load_table = get_cp_load_table();
		printf("cp_load_table = 0x%p\n", cp_load_table);
		if (NULL != cp_load_table) {
			for(i = 0; cp_load_table[i].size > 0; i++) {
				_boot_load_required_image(cp_load_table[i]);
			}
		}
	} while(0);
#endif

#if defined (CONFIG_SECBOOT)

	/***if define CONFIG_KERNEL_BOOT_CP,update swversion is not here***/
#ifndef CONFIG_KERNEL_BOOT_CP
	//secboot_update_swVersion();
#endif
	/***secboot 3rd step***/
	secboot_terminal();

#endif
#ifdef CONFIG_TEE_FIREWALL
	fdt_reserved_mem_multimedia_parse(dt_adr);
#endif
	vlx_entry(dt_adr);
}
