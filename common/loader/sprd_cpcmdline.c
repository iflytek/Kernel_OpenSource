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
	RF_HW_INFO,
	K32_LESS,
	AUTO_TEST,
#ifdef CONFIG_BOARD_ID
	CRYSTAL_TYPE,
	RF_HW_ID,
#endif
	CHIPUID,
	NULL
};

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
#ifdef CONFIG_MEM_LAYOUT_DECOUPLING
extern void *parse_cpcmdline_addr();
	g_CPcmdlineBuf = (char*)parse_cpcmdline_addr();
#else
#ifdef CALIBRATION_FLAG_CP0
	g_CPcmdlineBuf = (char*)CALIBRATION_FLAG_CP0;
#endif
#ifdef CALIBRATION_FLAG_CP1
	g_CPcmdlineBuf = (char*)CALIBRATION_FLAG_CP1;
#endif
#endif
#endif
	if (g_CPcmdlineBuf)
		memset(g_CPcmdlineBuf, 0, MAX_CP_CMDLINE_LEN);
	debugf("g_CPcmdlineBuf = 0x%p\n" , g_CPcmdlineBuf);
}

static void cmdline_add_cp_cmdline(char *cmd, char* value)
{
	char *p;
	int len;
	//printf("add cmd, cmd = %s, value = %s\n", cmd, value);

	if(!is_invalid_cmd(cmd))return;

	if (NULL == g_CPcmdlineBuf)
		return;

	len = strlen(g_CPcmdlineBuf);
	p = g_CPcmdlineBuf + len;

	snprintf(p, MAX_CP_CMDLINE_LEN - len, "%s=%s ", cmd, value);
	//printf("cmd = %s\n" , p);
}

void cp_cmdline_fixup(void)
{
	char *value = NULL;
	int boardid = 0;
	int value_less = 0;
	char buf[30] = {0};
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

	//chip uid
#ifdef CONFIG_READ_UID
	{
		unsigned int block0;
		unsigned int block1;
		get_efuse_uid_ex(&block0,&block1);
		sprintf(buf,"0x%08x,0x%08x",block0,block1);
		cmdline_add_cp_cmdline(CHIPUID,buf);
	}
#endif
#ifdef CONFIG_BOARD_ID
	//rf borad id
	boardid = sprd_get_rfid();
	sprintf(buf,"%d",boardid);
	cmdline_add_cp_cmdline(RF_BOARD_ID, buf);

	//board id
	value_less=sprd_get_boardid();
	sprintf(buf,"%d",value_less);
	cmdline_add_cp_cmdline(RF_HW_ID, buf);

	//26m crystal
	sprintf(buf,"%d", sprd_get_crystal());
	cmdline_add_cp_cmdline(CRYSTAL_TYPE, buf);
#endif

/* Ugly but no other way, 32k type must be passed */
#if (defined(CONFIG_SC9833) || defined(CONFIG_SOC_SHARKLJ1) || defined(CONFIG_SOC_SHARKLE) || defined(CONFIG_SOC_PIKE2) || defined(CONFIG_SOC_SHARKL3) || defined(CONFIG_SOC_SP9853I))
	//32k less
	sprintf(buf,"%d", sprd_get_32k());
	cmdline_add_cp_cmdline(K32_LESS, buf);

#endif
	debugf("cp cmdline: %s\n", g_CPcmdlineBuf);
}

char *cp_getcmdline(void)
{
	return g_CPcmdlineBuf;
}

