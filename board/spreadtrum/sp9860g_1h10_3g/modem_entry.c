#include <common.h>

#include <asm/arch/common.h>
//#include "normal_mode.h"
#include <asm/arch/sprd_reg.h>

#include "cp_boot.h"

#define CP0_ZERO_MAP_ADR    0x50000000
#define CP0_ARM0_EXEC_ADR        0x88400000 

#define CP1_ZERO_MAP_ADR    0x50001000
#define CP1_EXEC_ADR        0x8ae00000

/*add macro for memset agdsp/tddsp/ldsp share memory*/
#define LDSP_SHARE_MEM_START 0xF2000//8k
#define TGDSP_SHARE_MEM_START 0xF4000//4k
#define AGDSP_SHARE_MEM_START 0xF5000//4k
#define MULTI_MODE_MEM_START 0xF6000//4k

#define PHYS_OFFSET_ADDR			0x80000000
//#define TD_CP_OFFSET_ADDR			0x8000000	/*128*/
//#define TD_CP_SDRAM_SIZE			0x1200000	/*18M*/
//#define WCDMA_CP_OFFSET_ADDR		0x10000000	/*256M*/
//#define WCDMA_CP_SDRAM_SIZE		0x4000000	/*64M*/
#define GGE_CP_OFFSET_ADDR 		0x08000000       /*128M*/
#define GGE_CP_SDRAM_SIZE 		0x01600000 	/*22M*/
#define LTE_CP_OFFSET_ADDR              0x09600000       /*150M*/
#define LTE_CP_SDRAM_SIZE 		0x06000000       /*96M*/
//#define PMIC_IRAM_ADDR 		0x50800000       /*pmic arm7 iram address remap at AP side*/
//#define PMIC_IRAM_SIZE 		0x8000       /*32K*/
#define SIPC_APCP_RESET_ADDR_SIZE	0xC00	/*3K*/
#define SIPC_APCP_RESET_SIZE	0x1000	/*4K*/
//#define SIPC_TD_APCP_START_ADDR		(PHYS_OFFSET_ADDR + TD_CP_OFFSET_ADDR + TD_CP_SDRAM_SIZE - SIPC_APCP_RESET_SIZE)	/*0x897FF000*/
//#define SIPC_WCDMA_APCP_START_ADDR	(PHYS_OFFSET_ADDR + WCDMA_CP_OFFSET_ADDR + WCDMA_CP_SDRAM_SIZE - SIPC_APCP_RESET_SIZE) /*0x93FFF000*/
//#define SIPC_GGE_APCP_START_ADDR 	(PHYS_OFFSET_ADDR + GGE_CP_OFFSET_ADDR + GGE_CP_SDRAM_SIZE - SIPC_APCP_RESET_SIZE) 	/*0x895ff000*/
#define SIPC_LTE_APCP_START_ADDR        (PHYS_OFFSET_ADDR + LTE_CP_OFFSET_ADDR + LTE_CP_SDRAM_SIZE - SIPC_APCP_RESET_SIZE)         /*0x8d5ff000*/
//#define SIPC_PMIC_APCP_START_ADDR        (PMIC_IRAM_ADDR+ PMIC_IRAM_SIZE+ - SIPC_APCP_RESET_ADDR_SIZE)         /*0x50807400*/


void cp_adr_remap(u32 cp_kernel_exec_addr, u32 cp_zero_map_addr)
{
    u32 data[3] = {0xe59f0000, 0xe12fff10,   cp_kernel_exec_addr};
    memcpy( cp_zero_map_addr, data, sizeof(data));      /* copy cp0 source code */
}

/*bug:533275  memset tg/l/agdsp share memory
root cause: In agdsp share IRAM memory ,this area exits command for agdsp,when interunpt happens
from ap or cp,the agdsp will use this area. While the area exit the dirty binary data ,ap or cp
do not clean up this area when interrupt happens at the right time,so error command will cause
unexcepted problems.*/

void memset_dsp_share_memory()
{
	memset((void *)LDSP_SHARE_MEM_START,0x0,0x2000);//8k
	memset((void *)TGDSP_SHARE_MEM_START,0x0,0x1000);//4k
	memset((void *)AGDSP_SHARE_MEM_START,0x0,0x1000);//4k
	memset((void *)MULTI_MODE_MEM_START,0x0,0x1000);//4k
}

void modem_entry()
{
	/*workaround,change wtlcp axi to 307.2M*/
	REG32(REG_AON_APB_SUB_SYS_DBG_SIG7) = 0x09;

	/*memset l/tg/agdsp share memory*/
	memset_dsp_share_memory();

	/*sipc addr clear*/
	sipc_addr_reset();
	agdsp_boot();
#ifndef CONFIG_KERNEL_BOOT_CP
	pmic_arm_m4_boot();
	pubcp_boot();
	debugf("boot CP1 OK\n");
#endif
}


void sipc_addr_reset()
{
	//memset((void *)SIPC_GGE_APCP_START_ADDR, 0x0, SIPC_APCP_RESET_ADDR_SIZE);
	memset((void *)SIPC_LTE_APCP_START_ADDR, 0x0, SIPC_APCP_RESET_ADDR_SIZE);
	//memset((void *)SIPC_PMIC_APCP_START_ADDR, 0x0,SIPC_APCP_RESET_ADDR_SIZE );
	return;
}



