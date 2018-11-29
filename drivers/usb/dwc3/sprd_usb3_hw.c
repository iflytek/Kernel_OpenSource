/**
 * sprd_usb3_hw.c - Spreadtrum USB3 HW glue file
 *
 * Copyright (C) 2015 SPREADTRUM  - http://www.spreadtrum.com
 *
 * Authors: Miao.Zhu <miao.zhu@spreadtrum.com>,
 *
 * SPDX-License-Identifier:     GPL-2.0
 */

#include <common.h>
#include <malloc.h>
#include <asm/arch/sprd_reg.h>
#include <asm/io.h>
#include <sprd_regulator.h>
#include "linux-compat.h"
#include "core.h"

//#define TMSC_28NM_HPM		0x5533286e /* USB3.0_PHY_TSMC_28MN */
//#define TMSC_16NM_FFPLL	0x5533166e /* USB3.0_PHY_TSMC_16MN */

extern int regulator_enable(const char con_id[]);
extern int regulator_disable(const char con_id[]);

#define LDO_USB3 "vddusb33"

static struct dwc3_device dwc3_dev = {
	.base = SPRD_USB_BASE,
	.maximum_speed = USB_SPEED_HIGH,
	.dis_u3_susphy_quirk = 1,
	.dis_u2_susphy_quirk = 1,
};
#ifdef CONFIG_ZEBU
static void usb_clk_enable(u32 is_on){}
#else
static void usb_clk_enable(u32 is_on)
{
	u32 val;

	val = readl(REG_AON_APB_APB_EB1);
	if (is_on)
		/* Purpose: To enable clk cfg */
		writel(val | BIT_AON_APB_CKG_EB, REG_AON_APB_APB_EB1);
	else
		writel(val & ~BIT_AON_APB_CKG_EB, REG_AON_APB_APB_EB1);
}
#endif
#ifdef CONFIG_FPGA
static void usb_phy_enable(u32 is_on)
{
	debugf("%s, enter !\n", __func__);
}
#else
void setpll_clock(void)
{
	uint32_t val;

	debugf("%s, enter !\n", __func__);
	/* Purpose: all clk generated form TWPLL */
	/* Purpose: To enable clk cfg */
	val = readl(REG_AON_APB_APB_EB1);
	writel(val | BIT_AON_APB_CKG_EB, REG_AON_APB_APB_EB1);

	/* Purpose: To select AON USB clk: 96M */
	val = readl(REG_AON_CLK_CORE_CGM_USB_CFG);
	val &= ~(0x7);
	val |= (0x3);
	writel(val, REG_AON_CLK_CORE_CGM_USB_CFG);

	/* Purpose: To select  USB suspend: 1M */
	val = readl(REG_AON_CLK_CORE_CGM_USB_SUSPEND_CFG);
	writel(val | BIT_AON_CLK_CORE_CGM_USB_SUSPEND_CFG_CGM_USB_SUSPEND_SEL,
			REG_AON_CLK_CORE_CGM_USB_CFG);

	/* Purpose: To select IPA USB clk: 24M */
	val = readl(REG_IPA_CLK_CORE_CGM_USB_REF_CFG);
	writel(val | BIT_IPA_CLK_CORE_CGM_USB_REF_CFG_CGM_USB_REF_SEL,
			REG_IPA_CLK_CORE_CGM_USB_REF_CFG);

	/* Purpose: To select IPA core clk: 192M  */
	val = readl(REG_IPA_CLK_CORE_CGM_IPA_CORE_CFG);
	val &= ~(BIT_IPA_CLK_CORE_CGM_IPA_CORE_CFG_CGM_IPA_CORE_SEL(0x3));
	val | = BIT_IPA_CLK_CORE_CGM_IPA_CORE_CFG_CGM_IPA_CORE_SEL(0x1);
	writel(val,
			REG_IPA_CLK_CORE_CGM_IPA_CORE_CFG);

	/* Purpose: To select aon apb clk: 96M  */
	val = readl(REG_AON_CLK_CORE_CGM_AON_APB_CFG);
	val &= ~(0x7);
	val |= (0x5);
	writel(val, REG_AON_CLK_CORE_CGM_AON_APB_CFG);

	/* Purpose: To select ap axi clk: 128M  */
	val = readl(REG_AON_CLK_CORE_CGM_AP_AXI_CFG);
	val &= ~(0x3);
	val |= (0x2);
	writel(val, REG_AON_CLK_CORE_CGM_AP_AXI_CFG);

	/* Purpose: To select scu clk: 256M */
	val = readl(REG_AON_SEC_APB_APCPU_BUS_CLK_CFG);
	val &= ~(0x77);
	val |= (0x21);
	writel(val, REG_AON_SEC_APB_APCPU_BUS_CLK_CFG);

	/* Purpose: To select ace clk: 256M */
	val = readl(REG_AON_SEC_APB_APCPU_BUS_CLK_CFG);
	val &= ~(0x7000);
	writel(val, REG_AON_SEC_APB_APCPU_BUS_CLK_CFG);

	/* Purpose: To select periph clk: 128M */
	val = readl(REG_AON_SEC_APB_APCPU_TOP_MISC_CLK_CFG);
	val &= ~(BIT_AON_SEC_APB_CGM_PERIPH_DIV(7) |
			BIT_AON_SEC_APB_CGM_PERIPH_SEL(3));
	val | = (BIT_AON_SEC_APB_CGM_PERIPH_DIV(3) |
			BIT_AON_SEC_APB_CGM_PERIPH_SEL(2));
	writel(val, REG_AON_SEC_APB_APCPU_TOP_MISC_CLK_CFG);

	/* Purpose: To select  gic clk: 128M */
	val = readl(REG_AON_SEC_APB_APCPU_TOP_MISC_CLK_CFG);
	val &= ~(BIT_AON_SEC_APB_CGM_GIC_DIV(7) |
			BIT_AON_SEC_APB_CGM_GIC_SEL(3));
	val | = (BIT_AON_SEC_APB_CGM_GIC_DIV(3) |
			BIT_AON_SEC_APB_CGM_GIC_SEL(2));
	writel(val, REG_AON_SEC_APB_APCPU_TOP_MISC_CLK_CFG);

	/* Purpose: To select ANANKE core0  clk: 384M */
	val = readl(REG_AON_SEC_APB_APCPU_ANANKE_CLK_CFG);
	val &= ~(BIT_AON_SEC_APB_CGM_CORE4_DIV(7) |
			BIT_AON_SEC_APB_CGM_CORE4_SEL(7));
	val | = (BIT_AON_SEC_APB_CGM_CORE4_DIV(1) |
			BIT_AON_SEC_APB_CGM_CORE4_SEL(1));
	writel(val, REG_AON_SEC_APB_APCPU_ANANKE_CLK_CFG);

}

static void usb_enable()
{
	u32 val;

	debugf("%s, enter !\n", __func__);
	/*enable analog*/
	val = readl(REG_AON_APB_APB_EB1);
	writel(val | BIT_AON_APB_ANA_EB, REG_AON_APB_APB_EB1);

	/* USB2/USB3 PHY power on */
	val = readl(REG_PMU_APB_ANALOG_PHY_PD_CFG);
	val &= ~(BIT_PMU_APB_USB3_PHY_PD_REG | BIT_PMU_APB_USB2_PHY_PD_REG);
	writel(val, REG_PMU_APB_ANALOG_PHY_PD_CFG);

	/*disable IPA_SYS shutdown*/
	val = readl(REG_PMU_APB_PD_IPA_SYS_CFG);
	writel(val & ~BIT_PMU_APB_PD_IPA_SYS_FORCE_SHUTDOWN, REG_PMU_APB_PD_IPA_SYS_CFG);

	/* disable low power */
	val = readl(REG_IPA_AHB_MAIN_MTX_LPC);
	writel(val & ~BIT_IPA_AHB_MAIN_LP_EB, REG_IPA_AHB_MAIN_MTX_LPC);

	udelay(10 * 1000);

	/* select the IPA_SYS USB controller */
	val = readl(REG_AON_APB_AON_SOC_USB_CTRL);
	writel(val | BIT_AON_APB_USB20_CTRL_MUX_REG, REG_AON_APB_AON_SOC_USB_CTRL);

	/* purpose: set bvalid vbus_valid and clear vbus_valid_sel */
	val = readl(REG_IPA_AHB_USB_CTL0);
	val |= BIT_IPA_AHB_UTMISRP_BVALID_REG | BIT_IPA_AHB_OTG_VBUS_VALID_PHYREG;
	writel(val , REG_IPA_AHB_USB_CTL0);

	val = readl(REG_IPA_AHB_USB_CTL0);
	writel(val & ~BIT_IPA_AHB_OTG_VBUS_VALID_PHYREG_SEL, REG_IPA_AHB_USB_CTL0);

	/* diable power management event */
	val = readl(REG_IPA_AHB_USB_CTL0);
	writel(val & ~BIT_IPA_AHB_PME_EN, REG_IPA_AHB_USB_CTL0);

	/* reset power present to default value */
	val = readl(REG_IPA_AHB_USB_CTL0);
	writel(val & ~BIT_IPA_AHB_USB_HOST_PORT_POWER_CONTROL_PRESENT, REG_IPA_AHB_USB_CTL0);

	/* Purpose: To set IPA AHB USB enable interface: Little Endian */
	val = readl(REG_IPA_AHB_IPA_EB);
	val |= BIT_IPA_AHB_USB_EB | BIT_IPA_AHB_USB_SUSPEND_EB | BIT_IPA_AHB_USB_REF_EB;
	writel(val , REG_IPA_AHB_IPA_EB);

	/* Purpose: To soft-reset USB control */
	val = readl(REG_IPA_AHB_IPA_RST);
	val |= BIT_IPA_AHB_USB_SOFT_RST | BIT_IPA_AHB_PAM_U3_SOFT_RST |
			BIT_IPA_AHB_PAM_IPA_SOFT_RST | BIT_IPA_AHB_IPA_SOFT_RST ;
	writel(val , REG_IPA_AHB_IPA_RST);

	udelay(10 * 1000);
	val = readl(REG_IPA_AHB_IPA_RST);
	val &= ~(BIT_IPA_AHB_USB_SOFT_RST | BIT_IPA_AHB_PAM_U3_SOFT_RST |
			BIT_IPA_AHB_PAM_IPA_SOFT_RST | BIT_IPA_AHB_IPA_SOFT_RST) ;
	writel(val , REG_IPA_AHB_IPA_RST);

	/* enable AON USB control 16bit */
	//val = readl(REG_AON_APB_OTG_PHY_CTRL);
	//writel(val | BIT_AON_APB_UTMI_WIDTH_SEL, REG_AON_APB_OTG_PHY_CTRL);

	/* enable USB2 PHY 16bit */
	val = readl(REG_ANLG_PHY_G3_ANALOG_USB20_USB20_UTMI_CTL1);
	writel(val | BIT_ANLG_PHY_G3_ANALOG_USB20_USB20_DATABUS16_8,
				REG_ANLG_PHY_G3_ANALOG_USB20_USB20_UTMI_CTL1);

	/* vbus valid */
	val = readl(REG_AON_APB_OTG_PHY_TEST);
	writel(val | BIT_AON_APB_OTG_VBUS_VALID_PHYREG,
				REG_AON_APB_OTG_PHY_TEST);
	val = readl(REG_ANLG_PHY_G3_ANALOG_USB20_USB20_UTMI_CTL1);
	writel(val | BIT_ANLG_PHY_G3_ANALOG_USB20_USB20_VBUSVLDEXT,
				REG_ANLG_PHY_G3_ANALOG_USB20_USB20_UTMI_CTL1);

	/* Purpose: To soft-reset USB2 PHY */
	val = readl(REG_AON_APB_APB_RST1);
	val |= (BIT_AON_APB_OTG_UTMI_SOFT_RST | BIT_AON_APB_OTG_PHY_SOFT_RST);
	writel(val , REG_AON_APB_APB_RST1);
	udelay(10 * 1000);
	val = readl(REG_AON_APB_APB_RST1);
	val &= ~(BIT_AON_APB_OTG_UTMI_SOFT_RST | BIT_AON_APB_OTG_PHY_SOFT_RST);
	writel(val , REG_AON_APB_APB_RST1);

	/* please wait for a moment */
	udelay(30 * 1000);
}

static usb_disable()
{
	u32 val;

	debugf("%s, enter !\n", __func__);

	/* Purpose: To set IPA AHB USB disable interface: Little Endian */
	val = readl(REG_IPA_AHB_IPA_EB);
	writel(val & ~(BIT_IPA_AHB_USB_EB | BIT_IPA_AHB_USB_SUSPEND_EB
		 | BIT_IPA_AHB_USB_REF_EB), REG_IPA_AHB_IPA_EB);

	val = readl(REG_AON_APB_OTG_PHY_TEST);
	writel(val & ~BIT_AON_APB_OTG_VBUS_VALID_PHYREG,
				REG_AON_APB_OTG_PHY_TEST);
	val = readl(REG_ANLG_PHY_G3_ANALOG_USB20_USB20_UTMI_CTL1);
	writel(val & ~BIT_ANLG_PHY_G3_ANALOG_USB20_USB20_VBUSVLDEXT,
				REG_ANLG_PHY_G3_ANALOG_USB20_USB20_UTMI_CTL1);

	/* USB2/USB3 PHY power off */
	val = readl(REG_PMU_APB_ANALOG_PHY_PD_CFG);
	val |= (BIT_PMU_APB_USB3_PHY_PD_REG | BIT_PMU_APB_USB2_PHY_PD_REG);
	writel(val, REG_PMU_APB_ANALOG_PHY_PD_CFG);

	/*disable analog*/
	val = readl(REG_AON_APB_APB_EB1);
	writel(val & ~BIT_AON_APB_ANA_EB, REG_AON_APB_APB_EB1);

}
static void usb_phy_enable(u32 is_on)
{
	if (is_on) {
		setpll_clock();
		usb_enable();
	} else {
		usb_disable();
	}
}
#endif

