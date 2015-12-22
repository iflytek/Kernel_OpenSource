#include <config.h>
#include <asm/io.h>
#include <linux/types.h>
#include "regs_adi.h"
#include "adi_hal_internal.h"
#include <asm/arch/sprd_reg.h>

#define ANA_VIBRATOR_CTRL0_U      (ANA_REGS_GLB_BASE + 0xf8)

#define CUR_DRV_CAL_SEL_U	(0x0 << 12)
#define SLP_LDOVIBR_PD_EN_U	(0x1 << 9)
#define LDO_VIBR_PD_U	(0x1 << 8)
#define LDO_VIBR_V_U	(0xB4)

void set_vibrator(int on)
{
	
}

void vibrator_hw_init(void)
{
	
}
