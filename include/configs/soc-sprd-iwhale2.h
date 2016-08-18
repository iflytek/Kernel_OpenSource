
/*for device tree*/
#define DT_PLATFROM_ID 9868
#define DT_HARDWARE_ID 1
#define DT_SOC_VER     0x20000


#define CONFIG_SILENT_CONSOLE

#ifdef CONFIG_IWHALE2_DDR4G
#define CONFIG_NR_DRAM_BANKS	4
#define PHYS_SDRAM_1_SIZE	0x40000000
#elif defined(CONFIG_TARGET_IWHALE2_FPGA_SIN)
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
#define CONFIG_SPL_32K

#define TDDSP_ADR       0x88020000



#define CONFIG_PMIC_ARM7_BOOT
#define CONFIG_CP1_BOOT
#define CHIP_ENDIAN_LITTLE
#define _LITTLE_ENDIAN 1

#define CONFIG_ARCH_SCX35L

/*eMMC config, note that even UFS boards need these macros to support SD card*/
#define CONFIG_MMC
#define CONFIG_MMC_SDMA
#define CONFIG_EMMC_VERSION_V40
#define CONFIG_EMMC_RST_N_FUNCTION

/*file system config*/
#define CONFIG_FAT_WRITE
#define CONFIG_FS_EXT4
#define CONFIG_CMD_EXT4
#define CONFIG_CMD_FAT

/*partition type*/
#define CONFIG_DOS_PARTITION
#define CONFIG_EFI_PARTITION

#define CONFIG_SYS_HZ			1000
#define CONFIG_SPRD_TIMER_CLK		1000 /*32768*/

#define CP0_ZERO_MAP_ADR    0x50000000
#define CP0_ARM0_EXEC_ADR        0x88400000

#define CP1_ZERO_MAP_ADR    0x50001000


#define MODEM_SIZE		(0x800000)
#define DSP_SIZE		(0x2E0000)
#define RUNTIMENV_SIZE		(3*128 * 1024)

#define CONFIG_SPL_LOAD_LEN (0x6000)
#define PUBKEY_BSC_BLOCK_INDEX 0
#define PUBKEY_VLR_BLOCK_INDEX 0
#define PUBKEY_READ_BLOCK_NUMS (SEC_HEADER_MAX_SIZE / EMMC_SECTOR_SIZE)
#define CONFIG_SPL_HASH_LEN (0xC00) /* 3KB */



/*#define CMDLINE_NEED_CONV */


#define CONFIG_SYS_SDRAM_BASE 0x0
#define CONFIG_SYS_SDRAM_END (CONFIG_SYS_SDRAM_BASE + PHYS_SDRAM_1_SIZE)

#define CONFIG_SYS_INIT_SP_ADDR     \
	(CONFIG_SYS_SDRAM_END - 0x10000 - GENERATED_GBL_DATA_SIZE)

#define CONFIG_SKIP_LOWLEVEL_INIT





#define CONFIG_CMDLINE_TAG		1	/* enable passing of ATAGs */
#define CONFIG_SETUP_MEMORY_TAGS	1
#define CONFIG_INITRD_TAG		1


#define CONFIG_NR_DRAM_BANKS	1

#define PHYS_SDRAM_1		0x80000000

#define CONFIG_STACKSIZE	(256 * 1024)	/* regular stack */

/*
 * Flash & Environment
 */

#define CONFIG_DDR_AUTO_DETECT
#define CONFIG_NR_DRAM_BANKS_ADDR_IN_IRAM    0x1C00   /* IRAM store ddr info */



/* NAND */
/*#define CONFIG_NAND_SC9630

#define CONFIG_SYS_MAX_NAND_DEVICE	1
#define CONFIG_SYS_NAND_BASE		(0x21100000)*/


#define CONFIG_CMD_MTDPARTS
#define CONFIG_MTD_PARTITIONS
#define CONFIG_MTD_DEVICE
#define CONFIG_CMD_UBI
#define CONFIG_RBTREE

/* U-Boot general configuration */
#define CONFIG_SYS_PROMPT	"=> "	/* Monitor Command Prompt */
/* Boot Argument Buffer Size */
#define CONFIG_SYS_BARGSIZE	CONFIG_SYS_CBSIZE
#define CONFIG_SYS_LONGHELP

#define CONFIG_LOADADDR		(CONFIG_SYS_TEXT_BASE - CONFIG_SYS_MALLOC_LEN - 4*1024*1024)	/* loadaddr env var */

#define MTDIDS_DEFAULT "nand0=sprd-nand"
#define MTDPARTS_DEFAULT "mtdparts=sprd-nand:256k(spl),512k(2ndbl),256k(params),512k(vmjaluna),10m(modem),3840k(fixnv),3840k(backupfixnv),5120k(dsp),3840k(runtimenv),10m(boot),10m(recovery),250m(system),180m(userdata),20m(cache),256k(misc),1m(boot_logo),1m(fastboot_logo),3840k(productinfo),512k(kpanic)"

#define COPY_LINUX_KERNEL_SIZE	(0x600000)

#define CONFIG_USB_DOWNLOAD
#define CONFIG_USB_DWC3
#ifdef CONFIG_USB_DWC3
#define CONFIG_USB_DWC3_GADGET
#define CONFIG_USB_DWC3_SPRD
#endif
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



#define CONFIG_PBINT_7S_RESET_V1
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



#define VLX_TAG_ADDR    0x82000100
/* SPRD VP DEBUG ANDROID     */
#define CONFIG_VP_DEBUG
#define DT_ADR 	    0x15400000
#define RAMDISK_ADR 0X15500000
#define KERNEL_ADR  0x10080000
#define BOOTIMG_ADR 0X1D000000



#define CONFIG_SPRD_VBAT_CHEK

#define CONFIG_ARM32
#define CONFIG_ARCH_SHARK
#define CONFIG_PREBOOT "role"
#define CONFIG_BOARD_LATE_INIT
/*#define CONFIG_SEPARATE_DT*/

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

/* sprd modem  */
#define CONFIG_SUPPORT_LTE
#define CONFIG_ADVANCED_LTE

#define LTE_FIXNV_SIZE	0x00080000 //512k
#define LTE_RUNNV_SIZE	0x000a0000 //640K
#define LTE_FIXNV_ADDR	0x8c520000
#define LTE_RUNNV_ADDR	0x8c5a0000
#define LTE_MODEM_SIZE	0x00d00000   //0x8f0000
#define LTE_MODEM_ADDR 0x8b800000

#define LTE_TGDSP_LOADER_OFFSET		0x20000

#define LTE_TGDSP_SIZE 	0x002E0000
#define LTE_TGDSP_ADDR	(0x89600000 + LTE_TGDSP_LOADER_OFFSET)
#define LTE_WGDSP_SIZE	0x00400000
#define LTE_WGDSP_ADDR	(LTE_TGDSP_ADDR + LTE_TGDSP_SIZE)
#define LTE_LDSP_SIZE 	0x00c00000
#define LTE_LDSP_ADDR	(LTE_WGDSP_ADDR + LTE_WGDSP_SIZE)

#define LTE_AGDSP_SIZE	0x00260000
#define LTE_AGDSP_ADDR	(0x89600000 - LTE_AGDSP_SIZE)

#define FIXNV_SIZE		LTE_FIXNV_SIZE

/* sprd dfs */
#define CONFIG_DFS_ENABLE
#define DFS_ADDR			0xE6040000
#define DFS_SIZE			(256*1024) //256K

#define CONFIG_PARM_RAM_ACTIVE

