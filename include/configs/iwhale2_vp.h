#include <configs/soc-sprd-iwhale2.h>
#ifndef __CONFIG_X86_SPRD_COMMON_H
#define __CONFIG_X86_SPRD_COMMON_H
#define CONFIG_SPRD_SOC_IWHALE2
#define CONFIG_SHOW_BOOT_PROGRESS
#define CONFIG_PHYSMEM
#define CONFIG_DISPLAY_BOARDINFO_LATE
#define CONFIG_DISPLAY_CPUINFO

#define CONFIG_SYS_NO_FLASH

#define CONFIG_LMB
#define CONFIG_OF_LIBFDT

/* no need to uncompress kernel right now */
#undef CONFIG_LZO
#undef CONFIG_ZLIB
#undef CONFIG_GZIP

#define CONFIG_SYS_BOOTM_LEN	(16 << 20)

#define CONFIG_SYS_MAX_FLASH_SECT  512

#define CONFIG_NR_DRAM_BANKS	1
#define PHYS_SDRAM_1_SIZE	0x80000000

/*-----------------------------------------------------------------------
 * Environment configuration
 */
#define CONFIG_ENV_IS_NOWHERE
#define CONFIG_ENV_SIZE		0x01000

/* Timer */
#define PIT_BASE	0x40
#define CONFIG_SYS_X86_TSC_TIMER
#define CONFIG_SYS_PCAT_INTERRUPTS
#define CONFIG_SYS_PCAT_TIMER

#define CONFIG_SYS_NUM_IRQS	16

#define CONFIG_SYS_STACK_SIZE	(32 * 1024)
#define CONFIG_SYS_MONITOR_BASE	CONFIG_SYS_TEXT_BASE
#define CONFIG_SYS_MALLOC_LEN	0x200000

/*
 * Miscellaneous configurable options
 */

#define CONFIG_FIT
#define CONFIG_FIT_SIGNATURE
#define CONFIG_RSA

#define CONFIG_SYS_LONGHELP
#define CONFIG_SYS_CBSIZE	512
#define CONFIG_SYS_PBSIZE	(CONFIG_SYS_CBSIZE + \
				sizeof(CONFIG_SYS_PROMPT) + \
				16)
#define CONFIG_SYS_MAXARGS	16
#define CONFIG_SYS_BARGSIZE	CONFIG_SYS_CBSIZE

#define CONFIG_SYS_MEMTEST_START	0x00100000
#define CONFIG_SYS_MEMTEST_END		0x01000000
#define CONFIG_SYS_LOAD_ADDR		0x20000000
/*for device tree*/
#define DT_PLATFROM_ID 9868
#define DT_HARDWARE_ID 1
#define DT_SOC_VER     0x20000
/* commands */
#define CONFIG_CMD_ZBOOT

#define CONFIG_BAUDRATE		115200
#define CONFIG_BOOTDELAY	2

#define CONFIG_BOOTARGS		"earlycon console=ttyS0,115200n8"
#define CONFIG_BOOTCOMMAND   "zboot 10080000 0 15500000"
/* SPRD Serial Configuration */
#define CONFIG_SPRD_UART		1
#define CONFIG_SYS_SC8800X_UART1	1
#define CONFIG_CONS_INDEX		1 /* use UART0 for console */
#define CONFIG_SPRD_UART_PORTS		{ (void *)(CONFIG_SYS_SERIAL0) }
#define CONFIG_SYS_SERIAL0		0xe7000000
/* SPRD VP DEBUG ANDROID     */
#define CONFIG_VP_DEBUG

#endif /* __CONFIG_H */
