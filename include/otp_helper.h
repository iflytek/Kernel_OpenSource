#ifndef _OTP_HELP_H_
#define _OTP_HELP_H_

extern u32 sprd_ap_efuse_read(int blk_index);
extern int sprd_ap_efuse_prog(int blk_index, u32 val);
extern u32 sprd_pmic_efuse_read(int blk_index);
extern u32 sprd_pmic_efuse_read_bits(int bit_index, int length);
extern void pmic_efuse_block_dump(void);
extern void ap_efuse_block_dump(void);
extern void sprd_uid_bin_info(void);

#endif
