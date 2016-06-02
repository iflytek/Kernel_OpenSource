#include "sprd_cpcmdline.h"
#include "loader_common.h"




char *g_CPcmdlineBuf = NULL;
#if defined( CONFIG_KERNEL_BOOT_CP )
char CPcmdlineBuf[MAX_CP_CMDLINE_LEN];
#endif

struct rf_board_info {
	unsigned char id;
	unsigned char b32K;
	char* pInfo;
};

static const struct rf_board_info boad_info_arr[] = {
	{0, 0, "SP9832A-2_SR3593_JP_V1.1.1"},
	{8, 0, "SP9832A-2_SR3593_V1.0.0"},
	{9, 0, "SP9832A-2_SR3593_LA_V1.0.0"},
	{10, 0, "SP9832A-2_SR3593_V1.1.0/V1.1.1"},
	{11, 0, "SP9832A-2_SR3593_LA_V1.1.1"},
	{12, 1, "SP9832A-3_V1.0.0(TSX)"}, // repalce 23
	{20, 0, "SP9832A-3_V1.0.0(TCXO)"},
	{22, 0, "SP9832A-3_V1.1.0(TCXO)"},
	{23, 0, "SP9832A-3_V1.0.0(TSX)"}, // is closed, the new id is 12
};

static const char *cmd_arr[] = {
	BOOT_MODE,
	CALIBRATION_MODE,
	LTE_MODE,
	AP_VERSION,
	RF_BOARD_ID,
	RF_HW_INFO,
	K32_LESS,
	AUTO_TEST,
	NULL
};

static unsigned int gpio_state(unsigned int GPIO_NUM){
	int value = 0 ;
	sprd_gpio_request(NULL, GPIO_NUM);
	sprd_gpio_direction_input(NULL, GPIO_NUM);
	value = sprd_gpio_get(NULL,GPIO_NUM);
	return value > 0;
}

static int get_adc_value_board(int channel){
	extern uint16_t sprdbat_auxadc2vbatvol(uint16_t adcvalue);
	int adc_value=0,vol = 0;
	uint32_t result;
	uint32_t res;
	//uint32_t channel_vol = sprd_adc_to_vol(adcvalue);
	uint32_t m, n;
	uint32_t bat_numerators, bat_denominators;
	uint32_t adc_channel_numerators, adc_channel_denominators;

	adc_value = pmic_adc_get_value_by_isen(channel,0,15,2000);
	vol = sprd_chan_small_adc_to_vol(channel, 0 , 0 ,adc_value);
	debugf("result = %d\n",vol);


	if (vol <= 0 || vol >= 1200) {
		errorf("vol is out of ranger [0~1200]\n");
		return -1;
	}

	if (vol >0 && vol <= 100)
		vol = 0;
	if (vol > 100 && vol <= 300 )
		vol = 1;
	if (vol > 300 && vol <= 500)
		vol = 2;
	if (vol > 500 && vol <= 700)
		vol = 3;
	if (vol > 700 && vol <= 850)
		vol = 4;
	if (vol > 850 && vol <= 1000)
		vol = 5;
	if (vol > 1000 && vol < 1200)
		vol = 6;

	return vol;
}

static int Sprd_getHWRFboardID(void){

#ifdef RF_BAND_INFO
	unsigned int gpio_val = 0;
	unsigned int adc_val = 0;
	unsigned char shift_bit = 2;

#ifdef ADC_CHANNEL_FOR_NV
	adc_val = get_adc_value_board(ADC_CHANNEL_FOR_NV);
#endif
	shift_bit++;
	debugf("adc_val: %d\n", adc_val);

#ifdef LB_GPIO_NUM
	gpio_val |= gpio_state(LB_GPIO_NUM) << shift_bit;
	shift_bit++;
	debugf("gpio_val0: %d\n", gpio_val);
#endif

#ifdef MB_GPIO_NUM
	gpio_val |= gpio_state(MB_GPIO_NUM) << shift_bit;
	shift_bit++;
	debugf("gpio_val1: %d\n", gpio_val);
#endif

#ifdef HB_GPIO_NUM
	gpio_val |= gpio_state(HB_GPIO_NUM) << shift_bit;
	debugf("gpio_val2: %d\n", gpio_val);
#endif
	return gpio_val + adc_val;
#endif
	return INVALID_BOARD_ID;
}

static char *cpcmdline_get_hw_info(unsigned char id, unsigned char *ptr_b32k)
{
	int i;

	for(i = 0; i < sizeof(boad_info_arr)/ sizeof(struct rf_board_info); i++ )
	{
		if(id == boad_info_arr[i].id) {
			*ptr_b32k = boad_info_arr[i].b32K;
			return boad_info_arr[i].pInfo;
		}
	}

	return NULL;
}


static bool is_invalid_cmd(const char *cmd)
{
	int i = 0;
	while(NULL != cmd_arr[i])
	{
		if(0 == strcmp(cmd_arr[i], cmd)){
			return true;
		}
		i++;
	}
	return false;
}

static char *cpcmdline_get_lte_mode(void)
{
	char *ltemode = NULL;

#ifdef CONFIG_SUPPORT_TDLTE
	ltemode = "tcsfb";
#elif defined CONFIG_SUPPORT_WLTE
	ltemode = "fcsfb";
#elif defined CONFIG_SUPPORT_LTE
	ltemode = "lcsfb";
#endif

	return ltemode;
}

static void cmdline_prepare(void)
{
#if defined( CONFIG_KERNEL_BOOT_CP )
	g_CPcmdlineBuf = CPcmdlineBuf;
#else
#ifdef CALIBRATION_FLAG_CP0
	g_CPcmdlineBuf = (char*)CALIBRATION_FLAG_CP0;
#endif
#ifdef CALIBRATION_FLAG_CP1
	g_CPcmdlineBuf = (char*)CALIBRATION_FLAG_CP1;
#endif
#endif
	memset(g_CPcmdlineBuf, 0, MAX_CP_CMDLINE_LEN);
	debugf("g_CPcmdlineBuf = 0x%p\n" , g_CPcmdlineBuf);
}

static cmdline_add_cp_cmdline(char *cmd, char* value)
{
	char *p;
	int len;
	//printf("add cmd, cmd = %s, value = %s\n", cmd, value);

	if(!is_invalid_cmd(cmd))return;

	len = strlen(g_CPcmdlineBuf);
	p = g_CPcmdlineBuf + len;

	snprintf(p, MAX_CP_CMDLINE_LEN - len, "%s=%s ", cmd, value);
	//printf("cmd = %s\n" , p);
}

void cp_cmdline_fixup(void)
{
	char *value = NULL;
	int boardid = 0;
#if defined(CONFIG_32K_LESS_COMPATIBLE)
	int value_less = 0;
#endif
	char buf[10] = {0};
	unsigned char v32kless;

	cmdline_prepare();

	// androd boot mode
	value = getenv("bootmode");
	if (NULL != value)
	{
		cmdline_add_cp_cmdline(BOOT_MODE, value);
	}

	// calibration parameters
	value = get_calibration_parameter();
	if (NULL != value)
	{
		if (NULL != strstr(value, CALIBRATION_MODE))
		{
			// "calibration=%d,%d,146 ", must skipp calibration=
			value += strlen(CALIBRATION_MODE) + 1;
			cmdline_add_cp_cmdline(CALIBRATION_MODE, value);
		} else if (NULL != strstr(value, AUTO_TEST)) {
			// "autotest=1", must skipp autotest=
			value += strlen(AUTO_TEST) + 1;
			cmdline_add_cp_cmdline(AUTO_TEST, value);
		}
	}

	// lte mode
	value = cpcmdline_get_lte_mode();
	if(NULL != value)
	{
		cmdline_add_cp_cmdline(LTE_MODE, value);
	}

	//rf borad id
	boardid = Sprd_getHWRFboardID();
	if (INVALID_BOARD_ID != boardid)
	{
		sprintf(buf,"%d",boardid);
		cmdline_add_cp_cmdline(RF_BOARD_ID, buf);

		// get hw info
		value = cpcmdline_get_hw_info((unsigned char)boardid, &v32kless);
		if (NULL != value)
		{
			cmdline_add_cp_cmdline(RF_HW_INFO, value);
#ifndef CONFIG_32K_LESS_COMPATIBLE
			if (v32kless > 0)
			{
				sprintf(buf, "%d", v32kless);
				cmdline_add_cp_cmdline(K32_LESS, buf);
			}
#endif
		}
	}

	debugf("cp cmdline: %s\n", g_CPcmdlineBuf);
}

char *cp_getcmdline(void)
{
	return g_CPcmdlineBuf;
}

