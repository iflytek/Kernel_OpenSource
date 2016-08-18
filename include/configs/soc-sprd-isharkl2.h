
/*for device tree*/
#define DT_PLATFROM_ID 9868
#define DT_HARDWARE_ID 1
#define DT_SOC_VER     0x20000


#define CONFIG_SILENT_CONSOLE

#ifdef CONFIG_TARGET_ISHARKL2_FPGA
#define CONFIG_NR_DRAM_BANKS	1
#define PHYS_SDRAM_1_SIZE	0x40000000
#else
#define CONFIG_NR_DRAM_BANKS	1
#define PHYS_SDRAM_1_SIZE	0x80000000
#endif

/*
 * SPREADTRUM BIGPHONE board - SoC Configuration
 */
#define CONFIG_ADIE_SC2723
#define PHYS_SDRAM_1		0x80000000

#define TDDSP_ADR       0x88020000

/*#define CONFIG_KERNEL_BOOT_CP*/

#define FIXNV_SIZE     LTE_FIXNV_SIZE
#define LTE_FIXNV_SIZE 0x0006C000 // 272K the max size is 512k
#define LTE_RUNNV_SIZE 0x00090000 // 400k the max size is 512k

#define CHIP_ENDIAN_LITTLE
#define _LITTLE_ENDIAN 1


/*
 * MMC definition
 */
#define CONFIG_MMC			1
#define CONFIG_MMC_SDMA			1
#define CONFIG_EMMC_VERSION_R7P0
#define CONFIG_EMMC_RST_N_FUNCTION
/*#define CONFIG_MMC_TRACE*/

/* file system config */
#define CONFIG_FS_EXT4
#define CONFIG_EXT4_WRITE
#define CONFIG_CMD_EXT4
#define CONFIG_CMD_EXT4_WRITE
#define CONFIG_CMD_FAT			1
#define CONFIG_FAT_WRITE		1

#define CONFIG_DOS_PARTITION		1
#define CONFIG_EFI_PARTITION		1



#define CONFIG_SYS_HZ			1000
#define CONFIG_SPRD_TIMER_CLK		1000 /*32768*/

#define CP0_ZERO_MAP_ADR    0x50000000
#define CP0_ARM0_EXEC_ADR        0x88400000

#define CP1_ZERO_MAP_ADR    0x50001000
#define CP1_EXEC_ADR        LTE_MODEM_ADDR


#define MODEM_SIZE		(0x800000)
#define DSP_SIZE		(0x2E0000)
#define RUNTIMENV_SIZE		(3*128 * 1024)

/*for spreadtrum secure boot*/
#define CONFIG_SPL_LOAD_LEN (0x6000)
#define PUBKEY_BSC_BLOCK_INDEX 0
#define PUBKEY_VLR_BLOCK_INDEX 0
#define PUBKEY_READ_BLOCK_NUMS (SEC_HEADER_MAX_SIZE / EMMC_SECTOR_SIZE)
#define CONFIG_SPL_HASH_LEN (0xC00) /* 3KB */

#define CONFIG_SYS_SDRAM_BASE 0x0
#define CONFIG_SYS_SDRAM_END (CONFIG_SYS_SDRAM_BASE + PHYS_SDRAM_1_SIZE)

#define CONFIG_DDR_AUTO_DETECT
#define CONFIG_NR_DRAM_BANKS_ADDR_IN_IRAM    0x1C00   /* IRAM store ddr info */

/* U-Boot general configuration */
#define CONFIG_SYS_PROMPT	"=> "	/* Monitor Command Prompt */
/* Boot Argument Buffer Size */
#define CONFIG_SYS_BARGSIZE	CONFIG_SYS_CBSIZE
#define CONFIG_SYS_LONGHELP

#define CONFIG_LOADADDR		(CONFIG_SYS_TEXT_BASE - CONFIG_SYS_MALLOC_LEN - 4*1024*1024)	/* loadaddr env var */

#define CONFIG_USB_DOWNLOAD
#define CONFIG_DWC_OTG
#define CONFIG_USB_GADGET_SC8800G
#define CONFIG_USB_DWC
#define CONFIG_USB_GADGET_DUALSPEED

#define CONFIG_MODEM_CALIBERATE


/*for sysdump*/
/*#define CONFIG_SPRD_SYSDUMP*/
#define CONFIG_RAMDUMP_NO_SPLIT 1	/* Don't split sysdump file */
#define REAL_SDRAM_SIZE 0x40000000	/*dump 1G */


#define CALIBRATE_ENUM_MS 3000
#define CALIBRATE_IO_MS 2000


#define LOW_BAT_VOL            3400 /*phone battery voltage low than this value will not boot up*/
#define LOW_BAT_VOL_CHG        3300

#define PWR_KEY_DETECT_CNT 2 /*this should match the count of boot_pwr_check() function */
#define ALARM_LEAD_SET_MS 0 /* time set for alarm boot in advancd */

#define PHYS_OFFSET_ADDR			0x80000000

/*7S reset config*/
#define CONFIG_7S_RST_MODULE_EN		1

#ifdef DEBUG
#define CONFIG_7S_RST_SW_MODE	1	//0:hw reset,1:arm reset,power keep on	//soft for debug version
#else
#define CONFIG_7S_RST_SW_MODE	0	//0:hw reset,1:arm reset,power keep on	//hard for user version
#endif
#define CONFIG_7S_RST_SHORT_MODE	1
#define CONFIG_7S_RST_2KEY_MODE		0
#define CONFIG_7S_RST_THRESHOLD		7


/* SPRD VP DEBUG ANDROID     */
#define CONFIG_VP_DEBUG
#define DT_ADR 	    0x15400000
#define RAMDISK_ADR 0X15500000
#define KERNEL_ADR  0x10080000
#define BOOTIMG_ADR 0X7D000000

#define CONFIG_PREBOOT "role"
#define CONFIG_BOARD_LATE_INIT


/* Chip Driver Macro Definitions Start*/
/*sprd adc*/
#define CONFIG_SPRD_ADC


/*sprd adi*/
#define CONFIG_SPRD_ADI

/*sprd gpio*/
#define CONFIG_SPRD_GPIO

/*sprd rtc*/
#define CONFIG_RTC_SPRD

#define CONFIG_FPGA

/*sprd watchdog*/
#define CONFIG_HW_WATCHDOG
#define CONFIG_SPRD_WATCHDOG
