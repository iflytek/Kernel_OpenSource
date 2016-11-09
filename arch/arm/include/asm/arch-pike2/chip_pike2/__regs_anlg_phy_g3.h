/*
 * Copyright (C) 2015 Spreadtrum Communications Inc.
 *
 * This file is dual-licensed: you can use it either under the terms
 * of the GPL or the X11 license, at your option. Note that this dual
 * licensing only applies to this file, and not this project as a
 * whole.
 *
 */


#ifndef _ANLG_PHY_G3_REG_H
#define _ANLG_PHY_G3_REG_H

#define CTL_BASE_ANLG_PHY_G3 SPRD_ANLG_PHY_G3


#define REG_ANLG_PHY_G3_ANALOG_EFUSE_EFS0_ENK           ( CTL_BASE_ANLG_PHY_G3 + 0x0000 )
#define REG_ANLG_PHY_G3_ANALOG_EFUSE_EFS1_ENK           ( CTL_BASE_ANLG_PHY_G3 + 0x0004 )
#define REG_ANLG_PHY_G3_ANALOG_EFUSE_REG_SEL_CFG_0      ( CTL_BASE_ANLG_PHY_G3 + 0x0008 )

/*---------------------------------------------------------------------------
// Register Name   : REG_ANLG_PHY_G3_ANALOG_EFUSE_EFS0_ENK
// Register Offset : 0x0000
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_ANLG_PHY_G3_ANALOG_EFUSE_EFS0_ENK1                           BIT(1)
#define BIT_ANLG_PHY_G3_ANALOG_EFUSE_EFS0_ENK2                           BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_ANLG_PHY_G3_ANALOG_EFUSE_EFS1_ENK
// Register Offset : 0x0004
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_ANLG_PHY_G3_ANALOG_EFUSE_EFS1_ENK1                           BIT(1)
#define BIT_ANLG_PHY_G3_ANALOG_EFUSE_EFS1_ENK2                           BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_ANLG_PHY_G3_ANALOG_EFUSE_REG_SEL_CFG_0
// Register Offset : 0x0008
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_ANLG_PHY_G3_DBG_SEL_ANALOG_EFUSE_EFS0_ENK1                   BIT(3)
#define BIT_ANLG_PHY_G3_DBG_SEL_ANALOG_EFUSE_EFS0_ENK2                   BIT(2)
#define BIT_ANLG_PHY_G3_DBG_SEL_ANALOG_EFUSE_EFS1_ENK1                   BIT(1)
#define BIT_ANLG_PHY_G3_DBG_SEL_ANALOG_EFUSE_EFS1_ENK2                   BIT(0)


#endif // _ANLG_PHY_G3_REG_H
