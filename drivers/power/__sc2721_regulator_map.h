/*
 * Copyright (C) 2015 Spreadtrum Communications Inc.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 *************************************************
 * Automatically generated C header: do not edit *
 *************************************************
 */

/*
 * Regulator (0)Name, Regulator (1)Type, Power Off (2)Ctrl and (3)Bit,
 * Voltage Trimming (4)Ctrl and (5)Bits, Calibration (6)Ctrl and (7)Bits,
 * Voltage (8)Default, Voltage (9)Ctrl and (10)Bits, Voltage Select (11)Count and Voltage (12)List[ ... ...]
 */

    SCI_REGU_REG(vddcpu, 0x12, ANA_REG_GLB_POWER_PD_SW, BIT_DCDC_CPU_PD,
	ANA_REG_GLB_DCDC_CPU_VOL, BIT(0)|BIT(1)|BIT(2)|BIT(3)|BIT(4)|BIT(5)|BIT(6)|BIT(7)|BIT(8), ANA_REG_GLB_DCDC_CH_CTRL, BIT(13)|BIT(14)|BIT(16)|BIT(18)|BIT(19),
	900, 0, 0, 2, 400, 3125);
    SCI_REGU_REG(vddcore, 0x12, ANA_REG_GLB_POWER_PD_SW, BIT_DCDC_CORE_PD,
	ANA_REG_GLB_DCDC_CORE_VOL, BIT(0)|BIT(1)|BIT(2)|BIT(3)|BIT(4)|BIT(5)|BIT(6)|BIT(7)|BIT(8), ANA_REG_GLB_DCDC_CH_CTRL, BIT(13)|BIT(14)|BIT(16)|BIT(18)|BIT(19),
	900, 0, 0, 2, 400, 3125);
    SCI_REGU_REG(vddmem, 0x12, ANA_REG_GLB_POWER_PD_SW, BIT_DCDC_MEM_PD,
	ANA_REG_GLB_DCDC_MEM_VOL, BIT(0)|BIT(1)|BIT(2)|BIT(3)|BIT(4)|BIT(5)|BIT(6)|BIT(7)|BIT(8)|BIT(9), ANA_REG_GLB_DCDC_CH_CTRL, BIT(13)|BIT(14)|BIT(16)|BIT(18)|BIT(19),
	1000, 0, 0, 2, 400, 3125);
    SCI_REGU_REG(vddgen, 0x12, ANA_REG_GLB_POWER_PD_SW, BIT_DCDC_GEN_PD,
	ANA_REG_GLB_DCDC_GEN_VOL, BIT(0)|BIT(1)|BIT(2)|BIT(3)|BIT(4)|BIT(5)|BIT(6)|BIT(7)|BIT(8)|BIT(9), ANA_REG_GLB_DCDC_CH_CTRL, BIT(13)|BIT(14)|BIT(16)|BIT(18)|BIT(19),
	1800, 0, 0, 2, 600, 3125);
    SCI_REGU_REG(vddwpa, 0x2, ANA_REG_GLB_DCDC_WPA_REG2, BIT_DCDC_WPA_PD,
	ANA_REG_GLB_DCDC_WPA_VOL, BIT(0)|BIT(1)|BIT(2), 0, BIT(13)|BIT(14)|BIT(15)|BIT(16)|BIT(18)|BIT(19),
	0, 0, 0, 0);
    SCI_REGU_REG(vddcama, 0x0, ANA_REG_GLB_LDO_CAMA_REG0, BIT_LDO_CAMA_PD,
	ANA_REG_GLB_LDO_CAMA_REG1, BIT(0)|BIT(1)|BIT(2)|BIT(3)|BIT(4)|BIT(5)|BIT(6)|BIT(7), ANA_REG_GLB_DCDC_CH_CTRL, BIT(9)|BIT(17)|BIT(18)|BIT(20),
	2800, 0, 0, 2, 1200, 10000);
    SCI_REGU_REG(vddcammot, 0x0, ANA_REG_GLB_LDO_CAMMOT_REG0, BIT_LDO_CAMMOT_PD,
	ANA_REG_GLB_LDO_CAMMOT_REG1, BIT(0)|BIT(1)|BIT(2)|BIT(3)|BIT(4)|BIT(5)|BIT(6)|BIT(7), ANA_REG_GLB_DCDC_CH_CTRL, BIT(9)|BIT(17)|BIT(18)|BIT(20),
	2800, 0, 0, 2, 1200, 10000);
    SCI_REGU_REG(avdd18, 0x10, ANA_REG_GLB_LDO_AVDD18_REG0, BIT(0),
	ANA_REG_GLB_LDO_AVDD18_REG1, BIT(0)|BIT(1)|BIT(2)|BIT(3)|BIT(4)|BIT(5)|BIT(6)|BIT(7), ANA_REG_GLB_DCDC_CH_CTRL, BIT(8)|BIT(10)|BIT(17)|BIT(18)|BIT(20),
	1800, 0, 0, 2, 1200, 6250);
    SCI_REGU_REG(vddrf18, 0x10, ANA_REG_GLB_LDO_VDDRF18_REG0, BIT_LDO_RF18_PD,
	ANA_REG_GLB_LDO_VDDRF18_REG1, BIT(0)|BIT(1)|BIT(2)|BIT(3)|BIT(4)|BIT(5)|BIT(6)|BIT(7), ANA_REG_GLB_DCDC_CH_CTRL, BIT(8)|BIT(10)|BIT(17)|BIT(18)|BIT(20),
	1800, 0, 0, 2, 1200, 6250);
    SCI_REGU_REG(vddrf15, 0x10, ANA_REG_GLB_LDO_VDDRF15_REG0, BIT_LDO_RF15_PD,
	ANA_REG_GLB_LDO_VDDRF15_REG1, BIT(0)|BIT(1)|BIT(2)|BIT(3)|BIT(4)|BIT(5)|BIT(6)|BIT(7), ANA_REG_GLB_DCDC_CH_CTRL, BIT(8)|BIT(10)|BIT(17)|BIT(18)|BIT(20),
	1500, 0, 0, 2, 1200, 6250);
