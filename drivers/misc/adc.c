#include <common.h>
#include <asm/io.h>
#include "adc_reg_v3.h"
#include "adc_drvapi.h"
#include "adi_hal_internal.h"
#include <asm/arch/sprd_reg.h>
#include <linux/compat.h>
static int __average(int a[], int N)
{
        int i, sum = 0;
        for (i = 0; i < N; i++)
                sum += a[i];
        return DIV_ROUND_CLOSEST(sum, N);
}


void ADC_Init(void)
{
	ANA_REG_OR(ANA_REG_GLB_ARM_MODULE_EN, BIT_ANA_ADC_EN); //ADC enable
	ANA_REG_OR(ANA_REG_GLB_ARM_CLK_EN,    BIT_CLK_AUXAD_EN|BIT_CLK_AUXADC_EN); //enable auxad clock
	ANA_REG_OR(ANA_REG_GLB_XTL_WAIT_CTRL,    BIT_XTL_EN);	//enable clk
#if !(defined(CONFIG_SC9001))
	__raw_writel(__raw_readl(REG_AON_APB_SINDRV_CTRL) |BIT_SINDRV_ENA, REG_AON_APB_SINDRV_CTRL);	//enable ddie to adie clk
#endif
	ANA_REG_OR(ADC_CTRL, ADC_EN_BIT);
	ANA_REG_OR(ADC_CTRL, ADC_MODE_12B);
}

void ADC_SetCs(adc_channel id)
{
    if(id >= ADC_MAX){
        sprd_err("adc limits to 0~%d\n", ADC_MAX);
        return;
    }

    ANA_REG_MSK_OR(ADC_CS, id, ADC_CS_BIT_MSK);
}

void ADC_SetScale(bool scale)
{
    if(ADC_SCALE_1V2 == scale)
    {
        ANA_REG_AND(ADC_CS, ~ADC_SCALE_BIT);
    }
    else if(ADC_SCALE_3V == scale)
    {
        ANA_REG_OR(ADC_CS, ADC_SCALE_BIT);
    }
    else
    {
        sprd_err("adc scale %d not support\n", scale);
    }
}

int32_t ADC_GetValues(adc_channel id, bool scale, uint8_t num, int32_t *p_buf)
{
	int32_t count,i;

	/* clear int */
	ANA_REG_OR(ADC_INT_CLR, ADC_IRQ_CLR_BIT);

	/* choose channel */
	ADC_SetCs(id);

	/* set ADC scale */
	ADC_SetScale(scale);

	/* set read numbers run ADC soft channel */
	if (num < 1) {
		return -1;
	}
	ANA_REG_MSK_OR(ADC_CTRL, BIT_SW_CH_RUN_NUM(num), SW_CH_NUM_MSK);
	ANA_REG_OR(ADC_CTRL, SW_CH_ON_BIT);

	/* wait adc complete */
	count = 1000;
	while(!(ANA_REG_GET(ADC_INT_SRC)&ADC_IRQ_RAW_BIT) && count--) {
		for (i =0; i < 0xff; i++);
	}
	if (count <= 0) {
		sprd_warning("WARNING: ADC_GetValue timeout....\n");
		return -1;
	}

	for (count = 0; count < num; count++) {
		p_buf[count] = ANA_REG_GET(ADC_DAT) & ADC_DATA_MSK;
	}

	ANA_REG_AND(ADC_CTRL, ~SW_CH_ON_BIT);			// turn off adc soft channel
	ANA_REG_MSK_OR(ADC_CTRL, BIT_SW_CH_RUN_NUM(1), SW_CH_NUM_MSK);
	ADC_SetCs(TPC_CHANNEL_X);						// set tpc channel x back
	ANA_REG_OR(ADC_INT_CLR, ADC_IRQ_CLR_BIT);		// clear irq of this time

	return 0;
}
int32_t ADC_GetValue(adc_channel id, bool scale)
{
	int32_t result;

	if (-1 == ADC_GetValues(id, scale, 1, &result)) {
		return -1;
	}

	return result;
}

static int sci_adc_set_current(u8 enable, int isen)
{
        if(enable) {
                /* BITS_AUXAD_CURRENT_IBS = (isen * 100 / 125 -1) */
                isen = (isen * 100 / 125 -1);
                if(isen > BITS_AUXAD_CURRENT_IBS(-1))
                        isen = BITS_AUXAD_CURRENT_IBS(-1);
                sci_adi_write(ANA_REG_GLB_AUXAD_CTL, (BIT_AUXAD_CURRENTSEN_EN | BITS_AUXAD_CURRENT_IBS(isen)),
                        BIT_AUXAD_CURRENTSEN_EN | BITS_AUXAD_CURRENT_IBS(-1));
        } else {
                sci_adi_clr(ANA_REG_GLB_AUXAD_CTL, BIT_AUXAD_CURRENTSEN_EN);
        }

        return 0;
}

int sci_adc_get_value_by_isen(unsigned int channel, int scale, int isen)
{

        #define ADC_MESURE_NUMBER       15
        //extern int sci_adc_vol_request(int channel, int scale, int mux, int* adc);
        int results[ADC_MESURE_NUMBER + 1] = {0};
        int ret = 0, i = 0;
        int adc;
        /* Fixme: only external adc channel used */
        BUG_ON(channel > 3);

        WARN_ON(isen > 40);

        printf("isen value: %d\n", isen);
        sci_adc_set_current(1, isen);

        if(0 == ADC_GetValues(channel,scale,ADC_MESURE_NUMBER,results)) {
                ret = __average(&results[ADC_MESURE_NUMBER/5],
                        (ADC_MESURE_NUMBER - ADC_MESURE_NUMBER * 2 /5));
        }


        //ret = sci_adc_vol_request(channel , scale, 0, &adc);
        sci_adc_set_current(0, 0);

        for(i = 0; i < ARRAY_SIZE(results); i++) {
                printf("%d\t", results[i]);
        }

        printf("\n%s() adc[%d] value: %d\n", __func__, channel, ret);

        return ret;
}

int get_adc_value(int channel){
	extern uint16_t sprdbat_auxadc2vbatvol(uint16_t adcvalue);
	int vol = 0;
	uint32_t result;
	uint32_t res;
	//uint32_t channel_vol = sprd_adc_to_vol(adcvalue);
	uint32_t m, n;
	uint32_t bat_numerators, bat_denominators;
	uint32_t adc_channel_numerators, adc_channel_denominators;

	vol = sci_adc_get_value_by_isen(channel,1,30);
	vol = sprdbat_auxadc2vbatvol(vol);
	//res = (u32) sci_adc_ratio(5, 0, 0);
	bat_numerators = 7;//res >> 16;
	bat_denominators = 29;//res & 0xffff;
	//res = (u32) sci_adc_ratio(channel, 1, 0);
	adc_channel_numerators = 1;//res >> 16;
	adc_channel_denominators = 1;//res & 0xffff;

	vol = vol * bat_numerators * (adc_channel_denominators)/
		( bat_denominators * adc_channel_numerators);
	printf("result = %d\n",vol);
	if(vol <=0 ){
		return -1;
	}

	vol /= 100;
	if(vol >0 &&vol <= 23)
		vol = 0;
	if(vol > 23 && vol <= 35)
		vol = 1;
	if(vol > 35 && vol <= 46)
		vol = 2;
	if(vol > 46 && vol <= 55)
		vol = 3;
	if(vol > 55 && vol <= 66)
		vol = 4;
	if(vol > 66 && vol <= 79)
		vol = 5;
	if(vol > 79 && vol <= 97)
		vol = 6;
	if(vol > 97)
		vol = 7;
	return vol;

}
