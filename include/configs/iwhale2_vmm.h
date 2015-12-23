#ifndef __CONFIG_H
#define __CONFIG_H

#include <configs/x86-sprd-common.h>
#include <configs/soc-sprd-iwhale2.h>
#define CONFIG_SPRD_SOC_IWHALE2

#define CONFIG_EXTRA_ENV_SETTINGS "setup_dtb_addr=0x00080000"

#endif /* __CONFIG_H */
