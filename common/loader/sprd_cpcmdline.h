#ifndef _MODEM_COMMON_H_
#define _MODEM_COMMON_H_

#include <config.h>
#include <common.h>
#include <linux/string.h>


#define MAX_CP_CMDLINE_LEN  (200)

/*cp cmd define */

#define BOOT_MODE        "androidboot.mode"
#define CALIBRATION_MODE "calibration"
#define LTE_MODE         "ltemode"
#define AP_VERSION       "apv"
#define RF_BOARD_ID      "rfboard.id"
#define RF_HW_INFO       "hardware.version"
#define K32_LESS         "32k.less"
#define AUTO_TEST        "autotest"
#ifdef CONFIG_TSX_32K
#define CRYSTAL_TYPE    "crystal"
#endif

#define CHIPUID          "chip.uid"
#define INVALID_BOARD_ID 0xFF

void cp_cmdline_fixup(void);
char *cp_getcmdline(void);

#endif // _MODEM_COMMON_H_
