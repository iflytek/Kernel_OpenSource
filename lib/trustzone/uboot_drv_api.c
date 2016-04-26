/*
 * Copyright (c) 2013-2014, ARM Limited and Contributors. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * Neither the name of ARM nor the names of its contributors may be used
 * to endorse or promote products derived from this software without specific
 * prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "uboot_sec_drv.h"
#include <otp_helper.h>

int uboot_verify_img(uint64_t start_addr, uint64_t lenth){

  uint32_t ret;
  uint32_t smcid;
  uint32_t funcid;

  //1、准备传入参数
  smcid  = TEESMC64_SIPCALL_WITH_ARG;
  funcid = FUNCTYPE_VERIFY_IMG;

  struct smc_param64 param;
  param.a0 = smcid;
  param.a1 = funcid;
  param.a2 = start_addr;
  param.a3 = lenth;

  flush_dcache_all_area();
  //2、调用SMC
  tee_smc_call64(&param);

  //4、获取返回值
  ret = param.a0;
  return ret;
}

static void disable_debug_control_signal(void)
{
#if defined (CONFIG_SPRD_SECBOOT)
	//sprd_ap_efuse_prog(BLOCK_NUM_3,DISABLE_SPRD_DEBUG_CONTROL);
#endif
#if defined (CONFIG_SANSA_SECBOOT)
    sprd_ap_efuse_prog(BLOCK_NUM_3,DISABLE_DX_DEBUG_CONTROL);
#endif
}

int uboot_program_efuse(uint64_t start_addr, uint64_t lenth){

  uint32_t ret;
  uint32_t smcid;
  uint32_t funcid;
  struct smc_param64 param;

  disable_debug_control_signal();
  
  ap_sansa_efuse_prog_power_on();
  smcid  = TEESMC64_SIPCALL_WITH_ARG;
  funcid = FUNCTYPE_PROGRAM_EFUSE;
  printf("###### uboot_program_efuse ######\n");
  param.a0 = smcid;
  param.a1 = funcid;
  param.a2 = start_addr;
  param.a3 = lenth;
  printf("a0=%x,a1=%x,a2=%x,a3=%x\n",param.a0,param.a1,param.a2,param.a3);
  flush_dcache_all_area();
  tee_smc_call64(&param);

  ret = param.a0;
  ap_sansa_efuse_power_off();
  printf("pro_ret=%x\n",param.a0);
  return ret;
}

int uboot_get_socid(uint64_t start_addr, uint64_t lenth){

  uint32_t ret;
  uint32_t smcid;
  uint32_t funcid;
  struct smc_param64 param;

  ap_sansa_efuse_prog_power_on();
  smcid  = TEESMC64_SIPCALL_WITH_ARG;
  funcid = FUNCTYPE_GET_SOCID;

  param.a0 = smcid;
  param.a1 = funcid;
  param.a2 = start_addr;
  param.a3 = lenth;
  flush_dcache_all_area();
  tee_smc_call64(&param);

  ret = param.a0;
  ap_sansa_efuse_power_off();
  return ret;
}

int uboot_update_swVersion(uint64_t start_addr, uint64_t lenth){

  uint32_t ret;
  uint32_t smcid;
  uint32_t funcid;
  struct smc_param64 param;

  smcid  = TEESMC64_SIPCALL_WITH_ARG;
  funcid = FUNCTYPE_UPDATE_VERSION;

  param.a0 = smcid;
  param.a1 = funcid;
  param.a2 = start_addr;
  param.a3 = lenth;
  flush_dcache_all_area();
  tee_smc_call64(&param);

  ret = param.a0;
  return ret;
}

int uboot_sansa_get_lcs(uint32_t *p_lcs){

   uint32_t ret;
   uint32_t smcid;
   uint32_t funcid;
   struct smc_param64 param;

   ap_sansa_efuse_prog_power_on();

   smcid  = TEESMC64_SIPCALL_WITH_ARG;
   funcid = FUNCTYPE_GET_SANSA_LCS;
   printf("###### uboot_sansa_get_lcs ######\n");
   param.a0 = smcid;
   param.a1 = funcid;
   printf("a0=%x,a1=%x\n",param.a0,param.a1);
   flush_dcache_all_area();
   tee_smc_call64(&param);

   ret = param.a0;
   *p_lcs = param.a2;

   ap_sansa_efuse_power_off();
   printf("lcsval=%d\n",param.a2);
   return ret;
}

int uboot_sansa_set_rma(uint32_t *p_rmaret){
   uint32_t ret;
   uint32_t smcid;
   uint32_t funcid;
   struct smc_param64 param;

   ap_sansa_efuse_prog_power_on();

   smcid  = TEESMC64_SIPCALL_WITH_ARG;
   funcid = FUNCTYPE_SET_SANSA_RMA;
   printf("###### uboot_sansa_set_rma ######\n");
   param.a0 = smcid;
   param.a1 = funcid;
   printf("a0=%x,a1=%x\n",param.a0,param.a1);
   flush_dcache_all_area();
   tee_smc_call64(&param);

   ret = param.a0;
   *p_rmaret = param.a2;

   ap_sansa_efuse_power_off();
   printf("setrma=%d\n",param.a2);
   return ret;
}

int uboot_kce_program(uint64_t start_addr, uint64_t lenth){

  uint32_t ret;
  uint32_t smcid;
  uint32_t funcid;
  struct smc_param64 param;

  ap_sansa_efuse_prog_power_on();
  smcid  = TEESMC64_SIPCALL_WITH_ARG;
  funcid = FUNCTYPE_KCE_PROGRAM;
  printf("###### uboot_kce_program ######\n");
  param.a0 = smcid;
  param.a1 = funcid;
  param.a2 = start_addr;
  param.a3 = lenth;
  printf("a0=%x,a1=%x,a2=%x,a3=%x\n",param.a0,param.a1,param.a2,param.a3);
  flush_dcache_all_area();
  tee_smc_call64(&param);

  ret = param.a0;
  ap_sansa_efuse_power_off();
  printf("kce_pro=%x\n",param.a0);
  return ret;
}


