#ifndef _AW8898_L_H_
#define _AW8898_L_H_


/*
 * i2c transaction on Linux limited to 64k
 * (See Linux kernel documentation: Documentation/i2c/writing-clients)
*/
#define MAX_I2C_BUFFER_SIZE 65536

#define AW8898_L_FLAG_START_ON_MUTE   (1 << 0)
#define AW8898_L_FLAG_SKIP_INTERRUPTS     (1 << 1)
#define AW8898_L_FLAG_SAAM_AVAILABLE      (1 << 2)
#define AW8898_L_FLAG_STEREO_DEVICE       (1 << 3)
#define AW8898_L_FLAG_MULTI_MIC_INPUTS    (1 << 4)

#define AW8898_L_NUM_RATES                9

#define AW8898_L_MAX_REGISTER             0xff

#define AW8898_L_START_WORK_DELAY_MS      400

#define AW8898_L_BST_ILIMIT_4P5A          ( 7<< 4)
#define AW8898_L_BST_ILIMIT_4P25A         ( 6<< 4)
#define AW8898_L_BST_ILIMIT_4A            ( 5<< 4)
#define AW8898_L_BST_ILIMIT_3P75A         ( 4<< 4)
#define AW8898_L_BST_ILIMIT_3P5A          ( 3<< 4)
#define AW8898_L_BST_ILIMIT_3P25A         ( 2<< 4)
#define AW8898_L_BST_ILIMIT_3A            ( 1<< 4)
#define AW8898_L_BST_ILIMIT_2P75A         ( 0<< 4)
#define AW8898_L_BST_VOUT_10P25V          (15<< 0)
#define AW8898_L_BST_VOUT_10V             (14<< 0)
#define AW8898_L_BST_VOUT_9P75V           (13<< 0)
#define AW8898_L_BST_VOUT_9P5V            (12<< 0)
#define AW8898_L_BST_VOUT_9P25V           (11<< 0)
#define AW8898_L_BST_VOUT_9V              (10<< 0)
#define AW8898_L_BST_VOUT_8P75V           ( 9<< 0)
#define AW8898_L_BST_VOUT_8P5V            ( 8<< 0)
#define AW8898_L_BST_VOUT_8P25V           ( 7<< 0)
#define AW8898_L_BST_VOUT_8V              ( 6<< 0)
#define AW8898_L_BST_VOUT_7P75V           ( 5<< 0)
#define AW8898_L_BST_VOUT_7P5V            ( 4<< 0)
#define AW8898_L_BST_VOUT_7P25V           ( 3<< 0)
#define AW8898_L_BST_VOUT_7V              ( 2<< 0)
#define AW8898_L_BST_VOUT_6P75V           ( 1<< 0)
#define AW8898_L_BST_VOUT_6P5V            ( 0<< 0)
#define AW8898_L_VBAT_MONITOR
#ifdef AW8898_L_VBAT_MONITOR
#define SYS_BAT_VBAT "/sys/class/power_supply/battery/voltage_now"
#define SYS_BAT_SOC "/sys/class/power_supply/battery/capacity"
#define AW8898_L_SYS_VBAT_UP              3700000
#define AW8898_L_SYS_VBAT_DOWN            3600000
#define AW8898_L_SYS_VBAT_SOC_UP          110
#define AW8898_L_SYS_VBAT_SOC_DOWN        101
#define AW8898_L_IPEAK_LOW        	      AW8898_L_BST_ILIMIT_2P75A
#define AW8898_L_IPEAK_HIGH        	      AW8898_L_BST_ILIMIT_3P5A
#define AW8898_L_VMAX_LOW        	      AW8898_L_BST_VOUT_6P5V
#define AW8898_L_VMAX_HIGH         	      AW8898_L_BST_VOUT_9P5V
#endif


enum aw8898_l_chipid{
    AW8898_L_ID,
};

enum aw8898_l_mode_spk_rcv{
    AW8898_L_SPEAKER_MODE = 0,
    AW8898_L_RECEIVER_MODE = 1,
};

struct aw8898_l {
    struct regmap *regmap;
    struct i2c_client *i2c;
    struct snd_soc_codec *codec;
    struct device *dev;
    struct mutex cfg_lock;
#ifdef AW8898_L_VBAT_MONITOR
    struct hrtimer vbat_monitor_timer;
    struct work_struct vbat_monitor_work;
#endif
    int sysclk;
    int rate;
    int pstream;
    int cstream;
    struct work_struct start_work;


    int reset_gpio;
    int irq_gpio;

#ifdef CONFIG_DEBUG_FS
    struct dentry *dbg_dir;
#endif
    u8 reg;

    unsigned int flags;
    unsigned int chipid;
    unsigned int init;
    unsigned int spk_rcv_mode;
    unsigned int bst_ilimit;
};

struct aw8898_l_container{
    int len;
    unsigned char data[];
};

#ifdef AW8898_L_VBAT_MONITOR
static int aw8898_l_get_sys_battery_info(char *dev);
#endif
#endif
