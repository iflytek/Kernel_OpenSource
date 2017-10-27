/*
 * Copyright (c) 2013-2014, spreadtrum.com.
 *
 */

#include "uboot_sec_drv.h"


int uboot_set_rpmb_size(unsigned long start_addr, uint32_t lenth)
{
  smc_param *param = tee_common_call(FUNCTYPE_SET_RPMB_SIZE, (uint32_t)start_addr, lenth);

  return param->a0;
}
