/*
 * Copyright (C) 2014 Spreadtrum Communications Inc.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include "secure_efuse.h"
#include <mmc.h>
#include "sprd_common_rw.h"
#include <secureboot/sec_common.h>
#include "uboot_sec_drv.h"

#define  SOCID_SIZE_IN_WORDS 8 

imgToVerifyInfo g_imgToVerInfo = {0};

int secure_efuse_program(void)
{
  int ret=0;
  printf("###### read_spl_image ######\n");
  /**read spl image**/
  common_raw_read("splloader",SPL_IMAGE_SIZE, 0, (unsigned char *)SAVE_SPL_IMAGE);

  g_imgToVerInfo.img_addr = SAVE_SPL_IMAGE;
  g_imgToVerInfo.img_len = SPL_IMAGE_SIZE;
  g_imgToVerInfo.flag = SANSA_FLAG;

  printf("addr = %x,len = %x \n",g_imgToVerInfo.img_addr,g_imgToVerInfo.img_len);

  ret = uboot_program_efuse(&g_imgToVerInfo,sizeof(imgToVerifyInfo));

  if(ret)
  {
    return SANSA_EFUSE_PRO_ERR;
  }
  return PROG_OK;
  
}

unsigned int sansa_compute_socid(void *socid)
{
	int ret = 0;
	int i;
	uint32_t socIdBuff[SOCID_SIZE_IN_WORDS] = {0};

	ret = uboot_get_socid(socIdBuff, sizeof(socIdBuff));
	if (ret == 0) {
		memcpy((uint8_t *)socid, (uint8_t *)socIdBuff, sizeof(socIdBuff));
	}

	return ret;
}

int sansa_get_lcs_cmd(void)
{
  unsigned int t_lcs = 0;
  int ret = 0;

  ret = uboot_sansa_get_lcs(&t_lcs);
  printf("lcs = %d\n",t_lcs);

  if(ret)
  {
    return SANSA_GET_LCS_ERR;
  }

  return t_lcs;
}

int sansa_set_rma_cmd(void)
{
  unsigned int t_result=0;
  int ret = 0;

  ret = uboot_sansa_set_rma(&t_result);
  printf("rma_result = %d\n",t_result);

  if(ret)
  {
    return SANSA_SET_RMA_ERR;
  }

  return t_result;
}

