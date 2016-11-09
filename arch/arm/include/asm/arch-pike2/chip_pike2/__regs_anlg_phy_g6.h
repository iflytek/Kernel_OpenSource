/*
 * Copyright (C) 2015 Spreadtrum Communications Inc.
 *
 * This file is dual-licensed: you can use it either under the terms
 * of the GPL or the X11 license, at your option. Note that this dual
 * licensing only applies to this file, and not this project as a
 * whole.
 *
 */


#ifndef _ANLG_PHY_G6_REG_H
#define _ANLG_PHY_G6_REG_H

#define CTL_BASE_ANLG_PHY_G6 SPRD_ANLG_PHY_G6


#define REG_ANLG_PHY_G6_ANALOG_TSEN_ADC_TSEN_ADC_CTRL0      ( CTL_BASE_ANLG_PHY_G6 + 0x0000 )
#define REG_ANLG_PHY_G6_ANALOG_TSEN_ADC_TSEN_ADC_CTRL1      ( CTL_BASE_ANLG_PHY_G6 + 0x0004 )
#define REG_ANLG_PHY_G6_ANALOG_TSEN_ADC_TSEN_ADC_CTRL2      ( CTL_BASE_ANLG_PHY_G6 + 0x0008 )
#define REG_ANLG_PHY_G6_ANALOG_TSEN_ADC_REG_SEL_CFG_0       ( CTL_BASE_ANLG_PHY_G6 + 0x000C )

/*---------------------------------------------------------------------------
// Register Name   : REG_ANLG_PHY_G6_ANALOG_TSEN_ADC_TSEN_ADC_CTRL0
// Register Offset : 0x0000
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_ANLG_PHY_G6_ANALOG_TSEN_ADC_RG_TSEN_ADCLDOREF(x)                                  (((x) & 0x1F) << 12)
#define BIT_ANLG_PHY_G6_ANALOG_TSEN_ADC_RG_TSEN_ADCLDO_V(x)                                   (((x) & 0xF) << 8)
#define BIT_ANLG_PHY_G6_ANALOG_TSEN_ADC_RG_TSEN_CHOP_CLKSEL(x)                                (((x) & 0x3) << 6)
#define BIT_ANLG_PHY_G6_ANALOG_TSEN_ADC_RG_TSEN_CLKSEL(x)                                     (((x) & 0x3) << 4)
#define BIT_ANLG_PHY_G6_ANALOG_TSEN_ADC_RG_TSEN_SDADC_BIAS(x)                                 (((x) & 0x3) << 2)
#define BIT_ANLG_PHY_G6_ANALOG_TSEN_ADC_RG_TSEN_UGBUF_BIAS(x)                                 (((x) & 0x3))

/*---------------------------------------------------------------------------
// Register Name   : REG_ANLG_PHY_G6_ANALOG_TSEN_ADC_TSEN_ADC_CTRL1
// Register Offset : 0x0004
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_ANLG_PHY_G6_ANALOG_TSEN_ADC_RG_TSEN_SDADC_VCMI(x)                                 (((x) & 0x3) << 14)
#define BIT_ANLG_PHY_G6_ANALOG_TSEN_ADC_RG_TSEN_SDADC_VCMO(x)                                 (((x) & 0x3) << 12)
#define BIT_ANLG_PHY_G6_ANALOG_TSEN_ADC_RG_UGBUF_CTRL(x)                                      (((x) & 0x3) << 10)
#define BIT_ANLG_PHY_G6_ANALOG_TSEN_ADC_RG_TSEN_ADCLDO_EN                                     BIT(9)
#define BIT_ANLG_PHY_G6_ANALOG_TSEN_ADC_RG_TSEN_SDADC_CAPCHOP_EN                              BIT(8)
#define BIT_ANLG_PHY_G6_ANALOG_TSEN_ADC_RG_TSEN_SDADC_CHOP_EN                                 BIT(7)
#define BIT_ANLG_PHY_G6_ANALOG_TSEN_ADC_RG_TSEN_UGBUF_CHOP_EN                                 BIT(6)
#define BIT_ANLG_PHY_G6_ANALOG_TSEN_ADC_RG_TSEN_SDADC_EN                                      BIT(5)
#define BIT_ANLG_PHY_G6_ANALOG_TSEN_ADC_RG_TSEN_SDADC_OFFSET_EN                               BIT(4)
#define BIT_ANLG_PHY_G6_ANALOG_TSEN_ADC_RG_TSEN_INPUT_EN                                      BIT(3)
#define BIT_ANLG_PHY_G6_ANALOG_TSEN_ADC_RG_TSEN_UGBUF_EN                                      BIT(2)
#define BIT_ANLG_PHY_G6_ANALOG_TSEN_ADC_RG_TSEN_SDADC_DATA_EDGE_SEL                           BIT(1)
#define BIT_ANLG_PHY_G6_ANALOG_TSEN_ADC_RG_TSEN_SDADC_RST                                     BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_ANLG_PHY_G6_ANALOG_TSEN_ADC_TSEN_ADC_CTRL2
// Register Offset : 0x0008
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_ANLG_PHY_G6_ANALOG_TSEN_ADC_RG_TSEN_RESERVED(x)                                   (((x) & 0xFF) << 4)
#define BIT_ANLG_PHY_G6_ANALOG_TSEN_ADC_RG_TSEN_BIST_EN                                       BIT(3)
#define BIT_ANLG_PHY_G6_ANALOG_TSEN_ADC_RG_TSEN_BIST_CODE(x)                                  (((x) & 0x7))

/*---------------------------------------------------------------------------
// Register Name   : REG_ANLG_PHY_G6_ANALOG_TSEN_ADC_REG_SEL_CFG_0
// Register Offset : 0x000C
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_ANLG_PHY_G6_DBG_SEL_ANALOG_TSEN_ADC_RG_TSEN_ADCLDOREF                             BIT(21)
#define BIT_ANLG_PHY_G6_DBG_SEL_ANALOG_TSEN_ADC_RG_TSEN_ADCLDO_V                              BIT(20)
#define BIT_ANLG_PHY_G6_DBG_SEL_ANALOG_TSEN_ADC_RG_TSEN_CHOP_CLKSEL                           BIT(19)
#define BIT_ANLG_PHY_G6_DBG_SEL_ANALOG_TSEN_ADC_RG_TSEN_CLKSEL                                BIT(18)
#define BIT_ANLG_PHY_G6_DBG_SEL_ANALOG_TSEN_ADC_RG_TSEN_SDADC_BIAS                            BIT(17)
#define BIT_ANLG_PHY_G6_DBG_SEL_ANALOG_TSEN_ADC_RG_TSEN_UGBUF_BIAS                            BIT(16)
#define BIT_ANLG_PHY_G6_DBG_SEL_ANALOG_TSEN_ADC_RG_TSEN_SDADC_VCMI                            BIT(15)
#define BIT_ANLG_PHY_G6_DBG_SEL_ANALOG_TSEN_ADC_RG_TSEN_SDADC_VCMO                            BIT(14)
#define BIT_ANLG_PHY_G6_DBG_SEL_ANALOG_TSEN_ADC_RG_UGBUF_CTRL                                 BIT(13)
#define BIT_ANLG_PHY_G6_DBG_SEL_ANALOG_TSEN_ADC_RG_TSEN_ADCLDO_EN                             BIT(12)
#define BIT_ANLG_PHY_G6_DBG_SEL_ANALOG_TSEN_ADC_RG_TSEN_SDADC_CAPCHOP_EN                      BIT(11)
#define BIT_ANLG_PHY_G6_DBG_SEL_ANALOG_TSEN_ADC_RG_TSEN_SDADC_CHOP_EN                         BIT(10)
#define BIT_ANLG_PHY_G6_DBG_SEL_ANALOG_TSEN_ADC_RG_TSEN_UGBUF_CHOP_EN                         BIT(9)
#define BIT_ANLG_PHY_G6_DBG_SEL_ANALOG_TSEN_ADC_RG_TSEN_SDADC_EN                              BIT(8)
#define BIT_ANLG_PHY_G6_DBG_SEL_ANALOG_TSEN_ADC_RG_TSEN_SDADC_OFFSET_EN                       BIT(7)
#define BIT_ANLG_PHY_G6_DBG_SEL_ANALOG_TSEN_ADC_RG_TSEN_INPUT_EN                              BIT(6)
#define BIT_ANLG_PHY_G6_DBG_SEL_ANALOG_TSEN_ADC_RG_TSEN_UGBUF_EN                              BIT(5)
#define BIT_ANLG_PHY_G6_DBG_SEL_ANALOG_TSEN_ADC_RG_TSEN_SDADC_DATA_EDGE_SEL                   BIT(4)
#define BIT_ANLG_PHY_G6_DBG_SEL_ANALOG_TSEN_ADC_RG_TSEN_SDADC_RST                             BIT(3)
#define BIT_ANLG_PHY_G6_DBG_SEL_ANALOG_TSEN_ADC_RG_TSEN_RESERVED                              BIT(2)
#define BIT_ANLG_PHY_G6_DBG_SEL_ANALOG_TSEN_ADC_RG_TSEN_BIST_EN                               BIT(1)
#define BIT_ANLG_PHY_G6_DBG_SEL_ANALOG_TSEN_ADC_RG_TSEN_BIST_CODE                             BIT(0)


#endif // _ANLG_PHY_G6_REG_H
