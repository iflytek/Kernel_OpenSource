#ifndef __CONFIG_H
#define __CONFIG_H

#include <configs/soc-sprd-isharkl2.h>
#include <configs/x86-sprd-common.h>

#define CONFIG_SPRD_SOC_SP9853I

#define SETUP_DATA_OFFSET 0x10
#define DEFAULT_SETUP_BASE 0x350c0000
#define DEFAULT_SETUP_DTB_ADDR 0x3b000000
#define DT_ADR 	DEFAULT_SETUP_DTB_ADDR + SETUP_DATA_OFFSET
#define RAMDISK_ADR 0X3b800000
#define KERNEL_ADR  0x35100000
#define BOOTIMG_ADR 0X39000000

/*for sysdump*/
#define KERNEL_BASE    (KERNEL_ADR - 0x100000)


#endif /* __CONFIG_H */
