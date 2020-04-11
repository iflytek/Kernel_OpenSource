/*
 * SPRD verify orca modem process in AP side.
 *
 * Copyright (C) 2020 Spreadtrum Co., Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 of
 * the License as published by the Free Software Foundation.
 *
 * This program is used to verify orca modem in AP side for
 * Spreadtrum SoCs.
 */
#include <linux/module.h>
#include <linux/of_device.h>
#include <linux/platform_device.h>
#include <linux/nvmem-consumer.h>
#include "sprd_verify.h"
#include "../crypto/sprd_crypto.h"
#include "../crypto/sprd_sha256.h"
#include <linux/kernel.h>
#include <linux/string.h>

#define TRUSTED_FIRMWARE 1
#define NON_TRUSTED_FIRMWARE 0

static struct nvmem_device *efuse;
static u32 img_rollback_version;

/* for dat modem */
#define MODEM_MAGIC           "SCI1"
#define MODEM_HDR_SIZE        12
#define SCI_TYPE_MODEM_BIN    1
#define SCI_TYPE_PARSING_LIB  2
#define MODEM_LAST_HDR        0x100
#define MODEM_SHA1_HDR        0x400
#define MODEM_SHA1_SIZE       20

struct data_block_header_t {
	u32 type_flags;
	u32 offset;
	u32 length;
};

void cal_sha256(u8 *input, u32 bytes_num, u8 *output)
{
	if (input && output)
		sha256_csum_wd(input, bytes_num, output, 0);
	else
		pr_err("\r\tthe pointer is error,pls check it\n");
}

int check_and_update_version(u32 block, u32 certversion)
{
	int ret;

	if (!efuse) {
		pr_err("get efuse handle error!\n");
		return -ENODEV;
	}
	nvmem_device_read(efuse, (block - 2) * 4, 4, &img_rollback_version);
	if (certversion < img_rollback_version) {
		pr_err("error:block%d rollback_ver = %d  cert_ver = %d\n",
			block, img_rollback_version, certversion);
		ret = -ERANGE;
	} else {
		if (certversion > img_rollback_version)
			nvmem_device_write(efuse,
			(block - 2) * 4, 1, &certversion);
		ret = 0;
	}
	return ret;
}

extern unsigned char mode_flag;
int sprd_anti_rollback(u32 certtype, u32 certversion)
{
	int ret;

	if (certtype != NON_TRUSTED_FIRMWARE)
		return -EBADF;

	switch (mode_flag) {
	case MODEM_BLOCK:
		mode_flag = 0;
		ret = check_and_update_version(MODEM_BLOCK, certversion);
		break;

	case V3PHY_BLOCK:
		mode_flag = 0;
		ret = check_and_update_version(V3PHY_BLOCK, certversion);
		break;

	case NRPHY_BLOCK:
		mode_flag = 0;
		ret = check_and_update_version(NRPHY_BLOCK, certversion);
		break;

	case NRDSP1_BLOCK:
		mode_flag = 0;
		ret = check_and_update_version(NRDSP1_BLOCK, certversion);
		break;

	default:
		mode_flag = 0;
		ret = -EINVAL;
		break;
	}
	return ret;
}

int sprd_verify_cert(u8 *hash_key_precert, u8 *hash_data, u8 *certptr)
{
	int ret;
	u8 certtype = *certptr;
	u8 temphash_data[HASH_BYTE_LEN];
	sprd_crypto_err_t err = SPRD_CRYPTO_SUCCESS;
	sprd_rsa_pubkey_t pubkey;
	sprd_rsa_padding_t padding;
	int result;
	u8 certhash[HASH_BYTE_LEN];
	struct sprd_contentcert *curcertptr;

	padding.type = SPRD_RSASSA_PSS;
	padding.pad.rsassa_pss.type = SPRD_CRYPTO_HASH_SHA256;
	padding.pad.rsassa_pss.mgf1_hash_type = SPRD_CRYPTO_HASH_SHA256;
	padding.pad.rsassa_pss.salt_len = 32;

	if (certtype == CERTTYPE_CONTENT) {
		curcertptr = (struct sprd_contentcert *)certptr;
		cal_sha256((u8 *) &(curcertptr->pubkey), SPRD_RSAPUBKLEN,
			temphash_data);

		if (memcmp(hash_data, curcertptr->hash_data, HASH_BYTE_LEN)) {
			pr_err("<pss>cmp hash_data diffent\r\n");
			return -EFAULT;
		}
		if (memcmp(hash_key_precert, temphash_data, HASH_BYTE_LEN)) {
			pr_err("<pss>cmp hash key diffent\r\n");
			return -EFAULT;
		}
		pubkey.n = curcertptr->pubkey.mod;
		pubkey.n_len = (curcertptr->pubkey.keybit_len) >> 3;
		pubkey.e = (u8 *) &(curcertptr->pubkey.e);
		pubkey.e_len = 4;

		cal_sha256(curcertptr->hash_data, CNTCERT_HASH_LEN,
			certhash);
		err = sprd_rsa_verify((const sprd_rsa_pubkey_t *)(&pubkey),
			certhash, HASH_BYTE_LEN,
			(const u8 *)(curcertptr->signature),
			pubkey.n_len, padding, &result);

		if (err != SPRD_CRYPTO_SUCCESS) {
			pr_err("\n<pss>RSA verify err fail(%08x)\n", err);
			return -EFAULT;
		}

		if (result != SPRD_VERIFY_SUCCESS) {
			pr_err("\n<pss>RSA verify err result(%08x)\n", result);
			return -EFAULT;
		}

		ret = sprd_anti_rollback(curcertptr->type, curcertptr->version);
	} else {
		pr_err("<pss>invalid cert type %d!!", certtype);
		ret = -EINVAL;
	}

	return ret;
}

static bool is_packed_modem_image(u8 *data)
{
	if (memcmp(data, (u8 *)MODEM_MAGIC, sizeof(MODEM_MAGIC)))
		return false;
	return true;
}

static void get_modem_info(u8 *data, u32 *code_offset, u32 *code_len)
{
	u32 offset = 0, length = 0, hdr_offset = 0;
	u8 hdr_buf[MODEM_HDR_SIZE << 3];
	u8 read_len, result = 0;
	int found = 0;
	struct data_block_header_t *hdr_ptr;
	struct data_block_header_t *endp;

	read_len = sizeof(hdr_buf);
	memcpy(hdr_buf, data, read_len);

	do {
		if (!hdr_offset) {
			if (memcmp(hdr_buf, MODEM_MAGIC, sizeof(MODEM_MAGIC))) {
				result = OLD_IMG;
				pr_info("old image format!\n");
				break;
			}
			hdr_ptr = (struct data_block_header_t *)hdr_buf + 1;
			hdr_offset = MODEM_HDR_SIZE;
		} else {
			hdr_ptr = (struct data_block_header_t *)hdr_buf;
		}

		endp = (struct data_block_header_t *)(hdr_buf +
			sizeof(hdr_buf));
		while (hdr_ptr < endp) {
			u32 type = (hdr_ptr->type_flags & 0xff);

			if (type == SCI_TYPE_MODEM_BIN) {
				found = 1;
				break;
			}
			hdr_ptr++;
		}

		if (!found) {
			result = OLD_IMG;
			pr_err("no MODEM exe found in SCI header!");
		}
	} while (1);

	if (!result) {
		offset = hdr_ptr->offset;
		if (hdr_ptr->type_flags & MODEM_SHA1_HDR)
			offset += MODEM_SHA1_SIZE;
		length = hdr_ptr->length;
	}

	*code_offset = offset;
	*code_len = length;
}

u32 sprd_get_img_size(u8 *buf)
{
	struct sys_img_header *imghdr  = (struct sys_img_header *) buf;

	return imghdr->img_size;
}

u8 *sprd_get_sechdr_addr(u8 *buf)
{
	struct sys_img_header *imghdr = (struct sys_img_header *) buf;

	return buf + imghdr->img_size + sizeof(struct sys_img_header);
}

u8 *sprd_get_code_addr(u8 *buf)
{
	struct signed_img_header *sechdr_addr;
	u8 *code_addr;

	sechdr_addr = (struct signed_img_header *) sprd_get_sechdr_addr(buf);
	code_addr = buf + sechdr_addr->payload_offset;
	return code_addr;
}

u8 *sprd_get_cert_addr(u8 *buf)
{
	struct signed_img_header *sechdr_addr;
	u8 *cert_addr;

	sechdr_addr = (struct signed_img_header *) sprd_get_sechdr_addr(buf);
	cert_addr = buf + sechdr_addr->cert_offset;
	return cert_addr;
}

u8 *sprd_get_cert_addr_kbc(u8 *buf, u32 imgSize)
{
	u8 *cert_addr;

	cert_addr = buf + imgSize + sizeof(struct signed_img_header);
	return cert_addr;
}

int sprd_verify_img(u8 *hash_key_precert, u8 *imgbuf)
{
	struct signed_img_header *imghdr;
	u8 *code_addr, *curcertptr;
	u32 modem_offset = 0, modem_len = 0;
	u8 soft_hash_data[HASH_BYTE_LEN];
	u8 hash_key[HASH_BYTE_LEN];

	imghdr = (struct signed_img_header *) sprd_get_sechdr_addr(imgbuf);
	code_addr = sprd_get_code_addr(imgbuf);

	if (is_packed_modem_image(code_addr)) {
		get_modem_info(code_addr, &modem_offset, &modem_len);
		code_addr += modem_offset;
	}
	/* used to indicate verify spl image fastboot spl */
	memset(hash_key, 0xff, HASH_BYTE_LEN);
	cal_sha256(code_addr, imghdr->payload_size, soft_hash_data);
	curcertptr = sprd_get_cert_addr(imgbuf);
	return sprd_verify_cert(hash_key_precert, (u8 *)soft_hash_data,
		curcertptr);
}

int sprd_verify_img_kbc(u8 *hash_key_precert, u8 *imgbuf, u32 imgSize)
{
	u8 soft_hash_data[HASH_BYTE_LEN];
	u8 *curcertptr;

	memset(soft_hash_data, 0xff, HASH_BYTE_LEN);
	cal_sha256(imgbuf, imgSize, soft_hash_data);
	curcertptr = sprd_get_cert_addr_kbc(imgbuf, imgSize);
	return sprd_verify_cert(hash_key_precert, (u8 *)soft_hash_data,
		curcertptr);
}

void sprd_get_hash_key(u8 *load_buf, u8 *hash_key)
{
	struct sprd_keycert *certtype =
		(struct sprd_keycert *)sprd_get_cert_addr((u8 *)load_buf);

	memcpy(hash_key, certtype->hash_key, HASH_BYTE_LEN);
}

void sprd_get_pubk(u8 *load_buf, u8 *pubk)
{
	struct sprd_keycert *certtype =
		(struct sprd_keycert *)sprd_get_cert_addr((u8 *)load_buf);

	memcpy(pubk, (u8 *)&certtype->pubkey, sizeof(struct sprd_rsapubkey));
}

void sprd_secure_check(u8 *current_img_addr, u8 *data_header)
{
	u8 *hash_key_next = NULL;
	u8 hash_key[HASH_BYTE_LEN];

	memset(hash_key, 0, HASH_BYTE_LEN);
	sprd_get_hash_key(current_img_addr, hash_key);
	hash_key_next = hash_key;
	sprd_verify_img(hash_key_next, data_header);
}

static const struct of_device_id sprd_verify_of_match[] = {
	{ .compatible = "sprd,orca-verify", },
	{ }
};

static int sprd_verify_probe(struct platform_device *pdev)
{
	efuse = devm_nvmem_device_get(&pdev->dev, "ap-efuse");
	if (IS_ERR(efuse))
		return PTR_ERR(efuse);
	return 0;
}

static struct platform_driver sprd_verify_driver = {
	.probe = sprd_verify_probe,
	.driver = {
		.name = "sprd-verify",
		.of_match_table = sprd_verify_of_match,
	},
};

module_platform_driver(sprd_verify_driver);

MODULE_AUTHOR("gang.qie1 <gang.qie1@unisoc.com>");
MODULE_DESCRIPTION("modem verify driver");
MODULE_LICENSE("GPL v2");
