/*
 * Copyright (C) 2018 Spreadtrum Communications Inc.
 *
 * This file is dual-licensed: you can use it either under the terms
 * of the GPL or the X11 license, at your option. Note that this dual
 * licensing only applies to this file, and not this project as a
 * whole.
 *
 ********************************************************************
 * Auto generated c code from ASIC Documentation, PLEASE DONOT EDIT *
 ********************************************************************
 * updated at 2018-09-04 11:00:16
 *
 */

#ifndef ORCA_AP_MMAP_H
#define ORCA_AP_MMAP_H

/* Some defs, in case these are not defined elsewhere */        
#ifndef ORCA_AP_MMAP_H
#define SCI_IOMAP(_b_) ( (u32)(_b_) )
#endif /* ORCA_AP_MMAP_H */

/* Address Mapping definitions for sub-system AP */
#define REGS_AON_TOP_BASE               SCI_IOMAP(0x00000000)
#define REGS_ROM_TOP_BASE               SCI_IOMAP(0x20000000)
#define REGS_AHB_RF_BASE                SCI_IOMAP(0x21000000)
#define REGS_CLK_TOP_BASE               SCI_IOMAP(0x21100000)
#define REGS_DMA_AMST_TOP_BASE          SCI_IOMAP(0x21200000)
#define REGS_NANDC_CTRL_BASE            SCI_IOMAP(0x21300000)
#define REGS_USB3_0_BASE                SCI_IOMAP(0x21400000)
#define REGS_AP_APB_RF_BASE             SCI_IOMAP(0x24000000)
#define REGS_UART0_TOP_BASE             SCI_IOMAP(0x24100000)
#define REGS_I2C0_TOP_BASE              SCI_IOMAP(0x24200000)
#define REGS_I2C1_TOP_BASE              SCI_IOMAP(0x24300000)
#define REGS_I2C2_TOP_BASE              SCI_IOMAP(0x24400000)
#define REGS_I2C3_TOP_BASE              SCI_IOMAP(0x24500000)
#define REGS_I2C4_TOP_BASE              SCI_IOMAP(0x24600000)
#define REGS_SPI_HS0_TOP_BASE           SCI_IOMAP(0x24700000)
#define REGS_SPI_HS1_TOP_BASE           SCI_IOMAP(0x24800000)
#define REGS_SPI_HS2_TOP_BASE           SCI_IOMAP(0x24900000)
#define REGS_U0_UNIT_BASE               SCI_IOMAP(0x24a00000)
#define REGS_U1_UNIT_BASE               SCI_IOMAP(0x24a00020)
#define REGS_U2_UNIT_BASE               SCI_IOMAP(0x24a00040)
#define REGS_U3_UNIT_BASE               SCI_IOMAP(0x24a00060)
#define REGS_SIM0_TOP_BASE              SCI_IOMAP(0x24b00000)
#define REGS_SDIO_MST_TOP_BASE          SCI_IOMAP(0x26000000)
#define REGS_EMMC_TOP_BASE              SCI_IOMAP(0x26100000)
#define REGS_MMTX_WRAP_BASE             SCI_IOMAP(0x26200000)
#define REGS_IPA_SYS_TOP_BASE           SCI_IOMAP(0x48000000)

#endif /* ORCA_AP_MMAP_H */

