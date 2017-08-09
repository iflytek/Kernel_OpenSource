/*
 * Copyright (C) 2017 spreadtrum.com
 */

#include <linux/types.h>
#include <malloc.h>



#include <sprd_imgversion.h>
#include <sprd_rpmb.h>
#include "sprd_rpmb_blk_rng.h"



#define SPRD_IMGVER_MAGIC 0xA50000A5

struct sprd_img_ver_t {
    uint32_t magic;
    uint32_t system_imgver;
    uint32_t vendor_imgver;
};

struct sprd_modem_img_ver_t {
    uint32_t magic;
    uint32_t l_modem_imgver;
    uint32_t l_ldsp_imgver;
    uint32_t l_lgdsp_imgver;
    uint32_t pm_sys_imgver;
    uint32_t agdsp_imgver;
    uint32_t wcn_imgver;
    uint32_t gps_imgver;
    uint32_t gpu_imgver;
    uint32_t vbmeta_imgver;
    uint32_t boot_imgver;
    uint32_t recovery_imgver;
};

int sprd_get_imgversion(antirb_image_type imgType, unsigned int* swVersion)
{
    struct sprd_img_ver_t imgver;
    struct sprd_modem_img_ver_t modem_imgver;
    uint8_t data_rd[MMC_BLOCK_SIZE];
    uint16_t block_ind, block_count;
    int ret;

    if (imgType < IMAGE_L_MODEM) {
        block_ind = SPRD_IMGVERSION_BLK;
    } else {
        block_ind = SPRD_MODEM_IMGVERSION_BLK;
    }

    memset(data_rd, 0x0, sizeof(data_rd));
    block_count = sizeof(data_rd) / RPMB_DATA_SIZE;
    ret = rpmb_blk_read(data_rd, block_ind, block_count);
    if(ret < 0) {
        printf("rpmb read fail! return code %d \n", ret);
        return ret;
    }

    if (imgType < IMAGE_L_MODEM) {
        memcpy((void *)&imgver, data_rd, sizeof(struct sprd_img_ver_t));

        if(imgver.magic != SPRD_IMGVER_MAGIC) {
            printf("invalid sprd imgversion magic %x exp %x \n",
                imgver.magic, SPRD_IMGVER_MAGIC);
            return -1;
        }
    } else {
        memcpy((void *)&modem_imgver, data_rd, sizeof(struct sprd_modem_img_ver_t));

        if(modem_imgver.magic != SPRD_IMGVER_MAGIC) {
            printf("invalid sprd imgversion magic %x exp %x \n",
                modem_imgver.magic, SPRD_IMGVER_MAGIC);
            return -1;
        }

    }

    switch(imgType) {
        case IMAGE_SYSTEM:
            *swVersion = imgver.system_imgver;
            break;
        case IMAGE_VENDOR:
            *swVersion = imgver.vendor_imgver;
            break;
        case IMAGE_L_MODEM:
            *swVersion = modem_imgver.l_modem_imgver;
            break;
        case IMAGE_L_LDSP:
            *swVersion = modem_imgver.l_ldsp_imgver;
            break;
        case IMAGE_L_LGDSP:
            *swVersion = modem_imgver.l_lgdsp_imgver;
            break;
        case IMAGE_PM_SYS:
            *swVersion = modem_imgver.pm_sys_imgver;
            break;
        case IMAGE_AGDSP:
            *swVersion = modem_imgver.agdsp_imgver;
            break;
        case IMAGE_WCN:
            *swVersion = modem_imgver.wcn_imgver;
            break;
        case IMAGE_GPS:
            *swVersion = modem_imgver.gps_imgver;
            break;
        case IMAGE_GPU:
            *swVersion = modem_imgver.gpu_imgver;
            break;
        case IMAGE_VBMETA:
            *swVersion = modem_imgver.vbmeta_imgver;
            break;
        case IMAGE_BOOT:
            *swVersion = modem_imgver.boot_imgver;
            break;
        case IMAGE_RECOVERY:
            *swVersion = modem_imgver.recovery_imgver;
            break;
        default:
            printf("invalid sprd image type %d\n", imgType);
            return -1;
    }

    return 0;
}
