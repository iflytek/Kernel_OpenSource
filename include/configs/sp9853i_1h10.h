#ifndef __CONFIG_H
#define __CONFIG_H

#include <configs/soc-sprd-isharkl2.h>
#include <configs/x86-sprd-common.h>

#define CONFIG_SPRD_SOC_SP9853I

#define CONFIG_E820_AUTO_DETECT

/***********MEMORY LAYOUT****************/
#define SETUP_DATA_OFFSET 0x10
#define DEFAULT_SETUP_BASE 0x350c0000
#define DEFAULT_SETUP_DTB_ADDR 0x3b000000
#define DT_ADR 	DEFAULT_SETUP_DTB_ADDR + SETUP_DATA_OFFSET
#define RAMDISK_ADR 0X3b800000
#define KERNEL_ADR  0x35100000
#define BOOTIMG_ADR 0X39000000

/*for sysdump*/
#define CONFIG_SPRD_SYSDUMP
#define KERNEL_BASE    (KERNEL_ADR - 0x100000)
#define CONFIG_RAMDUMP_NO_SPLIT 1 /* Don't split sysdump file */
#define SYSDUMP_COPY_PADDR  (KERNEL_ADR-0x100000-0x20000000)
#define SYSDUMP_MAPPING_ADDR  0x60000000
#define SPLIT_MEM_REGION 0

#define CONFIG_MISC_INIT_R
/**********vmm related***********/
#define VMM_RESERVE_SIZE 0

#endif /* __CONFIG_H */
