/*
 * Copyright (C) 2015 Spreadtrum Communications Inc.
 *
 * This file is dual-licensed: you can use it either under the terms
 * of the GPL or the X11 license, at your option. Note that this dual
 * licensing only applies to this file, and not this project as a
 * whole.
 *
 */


#ifndef _ANLG_PHY_G9_REG_H
#define _ANLG_PHY_G9_REG_H

#define CTL_BASE_ANLG_PHY_G9 SPRD_ANLG_PHY_G9


#define REG_ANLG_PHY_G9_ANALOG_LVDSRF_0_REG_SEL_CFG      ( CTL_BASE_ANLG_PHY_G9 + 0x0000 )
#define REG_ANLG_PHY_G9_ANALOG_LVDSRF_1_REG_SEL_CFG      ( CTL_BASE_ANLG_PHY_G9 + 0x0004 )

/*---------------------------------------------------------------------------
// Register Name   : REG_ANLG_PHY_G9_ANALOG_LVDSRF_0_REG_SEL_CFG
// Register Offset : 0x0000
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_ANLG_PHY_G9_REG_SEL_ANALOG_LVDSRF_0                   BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_ANLG_PHY_G9_ANALOG_LVDSRF_1_REG_SEL_CFG
// Register Offset : 0x0004
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_ANLG_PHY_G9_REG_SEL_ANALOG_LVDSRF_1                   BIT(0)


#endif // _ANLG_PHY_G9_REG_H
