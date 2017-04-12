/*
 * Copyright (C) 2017 Spreadtrum Communications Inc.
 *
 * This file is dual-licensed: you can use it either under the terms
 * of the GPL or the X11 license, at your option. Note that this dual
 * licensing only applies to this file, and not this project as a
 * whole.
 *
 */


#ifndef _SERDES_APB_REG_H
#define _SERDES_APB_REG_H

#define CTL_BASE_SERDES_APB 0x40470000


#define REG_SERDES_APB_FUNC_EN          ( CTL_BASE_SERDES_APB + 0x0000 )
#define REG_SERDES_APB_CH_EN            ( CTL_BASE_SERDES_APB + 0x0004 )
#define REG_SERDES_APB_FUNNEL_EN        ( CTL_BASE_SERDES_APB + 0x0008 )
#define REG_SERDES_APB_FUNNEL_OVERFLOW  ( CTL_BASE_SERDES_APB + 0x000C )
#define REG_SERDES_APB_FSM_CUT_OFF_LEN  ( CTL_BASE_SERDES_APB + 0x0010 )

/*---------------------------------------------------------------------------
// Register Name   : REG_SERDES_APB_FUNC_EN
// Register Offset : 0x0000
// Description     :
---------------------------------------------------------------------------*/

#define BIT_SERDES_APB_FUNC_EN_FUNC_EN                                  BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_SERDES_APB_CH_EN
// Register Offset : 0x0004
// Description     :
---------------------------------------------------------------------------*/

#define BIT_SERDES_APB_CH_EN_CH_EN(x)                                   (((x) & 0xFFFF))

/*---------------------------------------------------------------------------
// Register Name   : REG_SERDES_APB_FUNNEL_EN
// Register Offset : 0x0008
// Description     :
---------------------------------------------------------------------------*/

#define BIT_SERDES_APB_FUNNEL_EN_FUNNEL_EN                              BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_SERDES_APB_FUNNEL_OVERFLOW
// Register Offset : 0x000C
// Description     :
---------------------------------------------------------------------------*/

#define BIT_SERDES_APB_FUNNEL_OVERFLOW_FUNNEL_OVERFLOW                  BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_SERDES_APB_FSM_CUT_OFF_LEN
// Register Offset : 0x0010
// Description     :
---------------------------------------------------------------------------*/

#define BIT_SERDES_APB_FSM_CUT_OFF_LEN_FSM_CUT_OFF(x)                   (((x) & 0xFFFF))


#endif // _SERDES_APB_REG_H
