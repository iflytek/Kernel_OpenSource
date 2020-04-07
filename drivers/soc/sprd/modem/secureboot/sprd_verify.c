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
#include <linux/cdev.h>

#define TRUSTED_FIRMWARE 1
#define NON_TRUSTED_FIRMWARE 0

static dev_t dev_num;
static struct cdev v_dev;
static struct class *verify_class;
static struct nvmem_device *efuse;
static u32 img_rollback_version;

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

u32 sprd_get_img_size(u8 *buf)
{
	struct sys_img_header *imghdr = (struct sys_img_header *) buf;

	return imghdr->img_size;
}

u32 sprd_get_modem_size(u8 *buf)
{
	struct sys_img_header *imghdr = (struct sys_img_header *) buf;

	return imghdr->firmware_size;
}

u8 *sprd_get_cert_addr_kbc(u8 *buf, u32 imgSize)
{
	u8 *cert_addr;

	cert_addr = buf + imgSize + sizeof(struct signed_img_header);
	return cert_addr;
}

int sprd_verify_modem(u8 *hash_key_precert, u8 *imgbuf, u32 imgsize)
{
	u8 *code_addr;
	u8 *curcertptr;
	u8 soft_hash_data[HASH_BYTE_LEN];

	code_addr = imgbuf + sizeof(struct sys_img_header);
	cal_sha256(code_addr, imgsize, soft_hash_data);
	curcertptr = imgbuf + sizeof(struct sys_img_header) + imgsize
		+ sizeof(struct signed_img_header);
	return sprd_verify_cert(hash_key_precert, (u8 *)soft_hash_data,
		curcertptr);
}

int sprd_verify_img_kbc(u8 *hash_key_precert, u8 *imgbuf, u32 imgsize)
{
	u8 soft_hash_data[HASH_BYTE_LEN];
	u8 *curcertptr;

	memset(soft_hash_data, 0xff, HASH_BYTE_LEN);
	cal_sha256(imgbuf, imgsize, soft_hash_data);
	curcertptr = sprd_get_cert_addr_kbc(imgbuf, imgsize);
	return sprd_verify_cert(hash_key_precert, (u8 *)soft_hash_data,
		curcertptr);
}

static const struct of_device_id sprd_verify_of_match[] = {
	{ .compatible = "sprd,orca-verify", },
	{ }
};

static int efuse_write_and_check(u32 block, u32 write_data)
{
	u32 read_data = 0, read_org = 0;

	nvmem_device_read(efuse, (block - 2) * 4, 4, &read_org);
	nvmem_device_write(efuse, (block - 2) * 4, 4, &write_data);
	nvmem_device_read(efuse, (block - 2) * 4, 4, &read_data);

	if ((read_org | write_data) != read_data)
		return -EFAULT;

	return 0;
}

static int write_rotpk_blocks(u32 start_id, u32 end_id, u32 *pwrite_data)
{
	int ret;
	u32 block_id;
	u32 *write_ptr = pwrite_data;

	for (block_id = start_id; block_id <= end_id; block_id++) {
		ret = efuse_write_and_check(block_id, *write_ptr);
		if (ret)
			return ret;
		write_ptr++;
	}
	return ret;
}

static int verify_open(struct inode *inode, struct file *filp)
{
	return 0;
}

static long verify_ioctl(struct file *pfile, unsigned int cmd,
	unsigned long arg)
{
	int ret, plcs;
	u32 block, temp_data[8], rotpk_data[8];

	if (!efuse) {
		pr_err("ioctl get efuse handle error!\n");
		return -ENODEV;
	}
	memcpy(rotpk_data, rotpk_hash, HASH_BYTE_LEN);

	if (_IOC_TYPE(cmd) != EFUSE_MAGIC)
		return -EINVAL;
	if (_IOC_NR(cmd) > EFUSE_MAGIC_MAXNR)
		return -EINVAL;

	switch (cmd) {
	case WR_EFUSE_ROTPK:
		ret = write_rotpk_blocks(ROTPK_START, ROTPK_END, rotpk_data);
		if (ret)
			return ret;
		break;

	case RD_EFUSE_LCS:
		for (block = ROTPK_START; block <= ROTPK_END; block++)
			nvmem_device_read(efuse, (block - 2) * 4, 4,
			&temp_data[block - ROTPK_START]);

		if (memcmp(temp_data, rotpk_data, HASH_BYTE_LEN))
			plcs = 1;
		else
			plcs = 5;
		if (copy_to_user((int *)arg, &plcs, sizeof(int))) {
			pr_err("error:copy_to_user fail...");
			return -EFAULT;
		}
		ret = 0;
		break;

	default:
		ret = -EINVAL;
		break;
	}
	return ret;
}

static const struct file_operations verify_fops = {
	.owner = THIS_MODULE,
	.open = verify_open,
	.unlocked_ioctl = verify_ioctl,
};

static int sprd_verify_probe(struct platform_device *pdev)
{
	int ret;
	struct device *verify_dev;

	efuse = devm_nvmem_device_get(&pdev->dev, "ap-efuse");
	if (IS_ERR(efuse))
		return PTR_ERR(efuse);

	ret = alloc_chrdev_region(&dev_num, 0, 1, DEV_NAME);
	if (ret < 0) {
		pr_err("orca dev_num can't be registered!");
		return ret;
	}

	cdev_init(&v_dev, &verify_fops);
	cdev_add(&v_dev, dev_num, 1);

	verify_class = class_create(THIS_MODULE, DEV_NAME);
	if (IS_ERR(verify_class)) {
		pr_err("error:create verify_class failed!");
		cdev_del(&v_dev);
		unregister_chrdev_region(dev_num, 1);
		return PTR_ERR(verify_class);
	}

	verify_dev = device_create(verify_class, NULL, dev_num, NULL, DEV_NAME);
	if (IS_ERR(verify_dev)) {
		pr_err("error:create device failed!");
		class_destroy(verify_class);
		cdev_del(&v_dev);
		unregister_chrdev_region(dev_num, 1);
		return PTR_ERR(verify_dev);
	}

	return 0;
}

static int sprd_verify_remove(struct platform_device *pdev)
{
	device_destroy(verify_class, dev_num);
	class_destroy(verify_class);
	cdev_del(&v_dev);
	unregister_chrdev_region(dev_num, 1);
	return 0;
}

static struct platform_driver sprd_verify_driver = {
	.probe = sprd_verify_probe,
	.remove = sprd_verify_remove,
	.driver = {
		.name = "sprd-verify",
		.of_match_table = sprd_verify_of_match,
	},
};

module_platform_driver(sprd_verify_driver);

MODULE_AUTHOR("gang.qie1 <gang.qie1@unisoc.com>");
MODULE_DESCRIPTION("modem verify driver");
MODULE_LICENSE("GPL v2");
