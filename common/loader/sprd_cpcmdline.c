#include "sprd_cpcmdline.h"
#include "loader_common.h"




char *g_CPcmdlineBuf = NULL;
#if defined( CONFIG_KERNEL_BOOT_CP )
char CPcmdlineBuf[MAX_CP_CMDLINE_LEN];
#endif


static const char *cmd_arr[] = {
	BOOT_MODE,
	CALIBRATION_MODE,
	LTE_MODE,
	AP_VERSION,
	RF_BOARD_ID,
	NULL
};

static unsigned int gpio_state(unsigned int GPIO_NUM){
	int value = 0 ;
	sprd_gpio_request(NULL, GPIO_NUM);
	sprd_gpio_direction_input(NULL, GPIO_NUM);
	value = sprd_gpio_get(NULL,GPIO_NUM);
	return value > 0;
}

static int Sprd_getHWRFboardID(void){

#ifdef RF_BAND_INFO
	unsigned int gpio_val = 0;
	unsigned int adc_val = 0;
	unsigned char shift_bit = 0;
#ifdef LB_GPIO_NUM
	gpio_val |= gpio_state(LB_GPIO_NUM) << shift_bit;
	shift_bit++;
#endif

#ifdef MB_GPIO_NUM
	gpio_val |= gpio_state(MB_GPIO_NUM) << shift_bit;
	shift_bit++;
#endif

#ifdef HB_GPIO_NUM
	gpio_val |= gpio_state(HB_GPIO_NUM) << shift_bit;
	shift_bit++;
#endif

#ifdef ADC_CHANNEL_FOR_NV
	adc_val = get_adc_value(ADC_CHANNEL_FOR_NV);
	if(adc_val >=0){
		adc_val = adc_val << shift_bit;
	}
#endif
	return gpio_val + adc_val;
#endif
	return 0xFF;
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

static char* cpcmdline_get_lte_mode(void)
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
	if(NULL == g_CPcmdlineBuf)
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
}


void cmdline_add_cp_cmdline(char *cmd, char* value)
{
	char *p;
	int len;
	//printf("add cmd, cmd = %s, value = %s\n", cmd, value);

	cmdline_prepare();

	if(!is_invalid_cmd(cmd))return;

	len = strlen(g_CPcmdlineBuf);
	p = g_CPcmdlineBuf + len;

	snprintf(p, MAX_CP_CMDLINE_LEN - len, "%s=%s ", cmd, value);
	//printf("cmd = %s\n" , p);
}

void cp_cmline_fixup(void)
{
	char *value = NULL;
	int boardid = 0;
	char buf[10] = {0};
	cmdline_prepare();
	// androd boot mode
	value = getenv("bootmode");
	if(NULL != value)
	{
		cmdline_add_cp_cmdline(BOOT_MODE, value);
	}

	// calibration parameters
	value = get_calibration_parameter();
	if(NULL != strstr(value, CALIBRATION_MODE))
	{
		// "calibration=%d,%d,146 ", must skipp calibration=
		value += strlen(CALIBRATION_MODE) + 1;
	}
	else
	{
		value = NULL;
	}

	if(NULL != value)
	{
		cmdline_add_cp_cmdline(CALIBRATION_MODE, value);
	}

	// lte mode
	value = cpcmdline_get_lte_mode();
	if(NULL != value)
	{
		cmdline_add_cp_cmdline(LTE_MODE, value);
	}

	//rf borad id
	boardid = Sprd_getHWRFboardID();
		if (0xFF!=boardid)
	{
		sprintf(buf,"%d",boardid);
		cmdline_add_cp_cmdline(RF_BOARD_ID, buf);
	}

	debugf("cp cmdline: %s\n", g_CPcmdlineBuf);
}

char* cp_getcmdline(void)
{
	return g_CPcmdlineBuf;
}

