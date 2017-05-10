/******************************************************************************
 ** File Name:      sprd_musb2_driver.c                                        *
 ** Author:         chunhou.wang                                              *
 ** DATE:           07/11/2016                                                *
 ** Copyright:      2010 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 ******************************************************************************/
/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/



#include <common.h>
#include <asm/io.h>
#include <asm/arch/sprd_reg.h>
#include <sprd_regulator.h>
#include <usb/usb200_fdl.h>
#include <packet.h>
#include <dl_crc.h>

static void __raw_bits_and(unsigned int v, unsigned int a)
{
	__raw_writel((__raw_readl(a) & v), a);
}

static void __raw_bits_or(unsigned int v, unsigned int a)
{
	__raw_writel((__raw_readl(a) | v), a);
}

void usb_phy_init(void)
{
	__raw_bits_or(BIT_28|BIT_22, REG_AP_AHB_OTG_PHY_TEST);

	/*Tiger PHY width: 16 bit*/
	__raw_bits_or(BIT_30, REG_AP_AHB_OTG_PHY_CTRL);
	__raw_bits_or(BIT_3, REG_ANLG_PHY_G2_ANALOG_USB20_USB20_CTRL0);

	__raw_bits_or(BIT_OTG_SOFT_RST|BIT_OTG_UTMI_SOFT_RST, REG_AP_AHB_AHB_RST);
	__raw_bits_or(BIT_OTG_PHY_SOFT_RST, REG_AON_APB_APB_RST2);
	Musb_mdelay(5);
	__raw_bits_and(~(BIT_OTG_SOFT_RST|BIT_OTG_UTMI_SOFT_RST), REG_AP_AHB_AHB_RST);
	__raw_bits_and(~BIT_OTG_PHY_SOFT_RST, REG_AON_APB_APB_RST2);
}

