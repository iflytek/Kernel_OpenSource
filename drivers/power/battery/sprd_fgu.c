#include <common.h>
#include <asm/io.h>

#include <regs_adi.h>
#include "adi_hal_internal.h"
#include <asm/arch/sprd_reg.h>
#include <asm/arch/sprd_eic.h>
#include <sprd_battery.h>
#include "sprd_chg_helper.h"
#ifdef DEBUG
#define debugf(fmt, args...) do { printf("fgu:%s(): ", __func__); printf(fmt, ##args); } while (0)
#else
#define debugf(fmt, args...)
#endif

extern int charger_connected(void);

extern void power_down_devices(unsigned pd_cmd);

#define REGS_FGU_BASE ANA_FGU_BASE
#define REG_FGU_START                   SCI_ADDR(REGS_FGU_BASE, 0x0000)
#define REG_FGU_CONFIG                  SCI_ADDR(REGS_FGU_BASE, 0x0004)
#define REG_FGU_INT_EN                  SCI_ADDR(REGS_FGU_BASE, 0x0010)
#define REG_FGU_INT_CLR                 SCI_ADDR(REGS_FGU_BASE, 0x0014)
#define REG_FGU_VOLT_VAL                SCI_ADDR(REGS_FGU_BASE, 0x0020)
#define REG_FGU_OCV_VAL                 SCI_ADDR(REGS_FGU_BASE, 0x0024)
#define REG_FGU_POCV_VAL                SCI_ADDR(REGS_FGU_BASE, 0x0028)
#define REG_FGU_CURT_VAL                SCI_ADDR(REGS_FGU_BASE, 0x002c)
#define REG_FGU_CURT_OFFSET             SCI_ADDR(REGS_FGU_BASE, 0x0090)
#define BIT_VOLT_H_VALID                ( BIT(12) )
#define BITS_VOLT_DUTY(_x_)             ( (_x_) << 5 & (BIT(5)|BIT(6)) )

#define REG_FGU_USER_AREA_SET             SCI_ADDR(REGS_FGU_BASE, 0x00A0)
#define REG_FGU_USER_AREA_CLEAR             SCI_ADDR(REGS_FGU_BASE, 0x00A4)
#define REG_FGU_USER_AREA_STATUS             SCI_ADDR(REGS_FGU_BASE, 0x00A8)

#define BITS_POWERON_TYPE_SHIFT   12
#define BITS_POWERON_TYPE(_x_)           ( (_x_) << 12 & (0xF000))
#define BITS_RTC_AREA_SHIFT     0
#define BITS_RTC_AREA(_x_)           ( (_x_) << 0 & (0xFFF) )

#define FIRST_POWERTON  0xF
#define NORMAIL_POWERTON  0x5
#define WDG_POWERTON  0xA

static void sprdfgu_rtc_reg_write(uint32_t val)
{
	sci_adi_write(REG_FGU_USER_AREA_CLEAR, BITS_RTC_AREA(~val),
		      BITS_RTC_AREA(~0));
	sci_adi_write(REG_FGU_USER_AREA_SET, BITS_RTC_AREA(val),
		      BITS_RTC_AREA(~0));
}

static uint32_t sprdfgu_rtc_reg_read(void)
{
	return (sci_adi_read(REG_FGU_USER_AREA_STATUS) & BITS_RTC_AREA(~0)) >>
	    BITS_RTC_AREA_SHIFT;
}

static void sprdfgu_poweron_type_write(uint32_t val)
{
	sci_adi_write(REG_FGU_USER_AREA_CLEAR, BITS_POWERON_TYPE(~val),
		      BITS_POWERON_TYPE(~0));
	sci_adi_write(REG_FGU_USER_AREA_SET, BITS_POWERON_TYPE(val),
		      BITS_POWERON_TYPE(~0));
}

static uint32_t sprdfgu_poweron_type_read(void)
{
	return (sci_adi_read(REG_FGU_USER_AREA_STATUS) & BITS_POWERON_TYPE(~0))
	    >> BITS_POWERON_TYPE_SHIFT;
}

#define mdelay(_ms) udelay(_ms*1000)
unsigned int fgu_vol, fgu_cur;
void sprdfgu_init(void)
{

	sci_adi_set(ANA_REG_GLB_ARM_MODULE_EN, BIT_ANA_FGU_EN);
#if defined(CONFIG_ADIE_SC2721)
	sci_adi_set(ANA_REG_GLB_RTC_CLK_EN, BIT_RTC_FGU_EN);
#else
	sci_adi_set(ANA_REG_GLB_RTC_CLK_EN, BIT_RTC_FGU_EN | BIT_RTC_FGUA_EN);
#endif
	sci_adi_clr(REG_FGU_INT_EN, 0xFFFF);	//disable int after watchdog reset
	sci_adi_set(REG_FGU_INT_CLR, 0xFFFF);
	sci_adi_write(REG_FGU_CURT_OFFSET, 0, ~0);	//init offset after watchdog reset

	//mdelay(1000);

	fgu_vol = 0;		//sci_adi_read(REG_FGU_VOLT_VAL);

	fgu_cur = 0;		//sci_adi_read(REG_FGU_CURT_VAL);
	debugf("fgu_init fgu_vol 0x%x fgu_cur 0x%x \n", fgu_vol, fgu_cur);

	if (charger_connected() && sprdbat_is_battery_connected()) {
		if ((FIRST_POWERTON == sprdfgu_poweron_type_read())
			|| (sprdfgu_rtc_reg_read() == 0xFFF)) {
			sprdfgu_rtc_reg_write(0xFF);
			mdelay(1);
			sprdfgu_poweron_type_write(NORMAIL_POWERTON);
			printf("charge first poweron reset!!!!!\n");
			mdelay(3);
			power_down_devices(0);
		}
	}

	if (sprdfgu_rtc_reg_read() == 0xFF) {
		printf("secend poweron !!!!!\n");
		sprdfgu_poweron_type_write(FIRST_POWERTON);
		mdelay(1);
		sprdfgu_rtc_reg_write(0xFFF);
	}

}

