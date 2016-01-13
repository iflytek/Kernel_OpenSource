/******************************************************************************
 ** File Name:      sprd_pmic_adc.h                                                  *
 ** Author:                                                    *
 ** DATE:                                                          *
 ** Copyright:      2015 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the basic input and output operations   *
 **                 on hardware, it can be treated as a hardware abstract     *
 **                 layer interface.                                          *
 
 ******************************************************************************/

#ifndef _SPRD_ADC_H_
#define _SPRD_ADC_H_

/* adc channel cal definition */
#define ADC_CHANNEL_INVALID	-1
#define SPRD_AUXADC_CAL_NO 0
#define SPRD_AUXADC_CAL_NV 1
#define SPRD_AUXADC_CAL_CHIP 2
#define SPRDBIG_AUXADC_CAL_CHIP 3
#define SPRDLIT_AUXADC_CAL_CHIP 4
#define ADC_CHANNEL_VBATSENSE	5

#define ADC_BIG_SCALE     1
#define ADC_SMALL_SCALE   0

//#define ADC_CHANNEL_VBAT 5
//#define ADC_CHANNEL_PROG 4
//#define ADC_CHANNEL_VCHG 6

#define ADC_MESURE_NUMBER       15

struct sprd_adc_cal {
	uint16_t p0_vol;	//4.2V
	uint16_t p0_adc;
	uint16_t p1_vol;	//3.6V
	uint16_t p1_adc;
	uint16_t cal_type;
};

struct pmic_adc_para {
	int small_adc_p0;
	int small_adc_p1;
	int small_vol_p0;
	int small_vol_p1;
	int small_cal_efuse_blk;
	int small_cal_efuse_bit;
	int big_adc_p0;
	int big_adc_p1;
	int big_vol_p0;
	int big_vol_p1;
	int big_cal_efuse_blk;
	int big_cal_efuse_bit;
	int blk_width;
	int adc_data_off;
};
#if 0
static enum adc_channel {
	ADC_CHANNEL_0 = 0,  //ADCI0
	ADC_CHANNEL_1 = 1,  //ADCI1
	ADC_CHANNEL_2 = 2,  //ADCI2
	ADC_CHANNEL_3 = 3,  //ADCI3
	ADC_CHANNEL_2723_PROG = 4,
	ADC_CHANNEL_2723_VBAT = 5,
	ADC_CHANNEL_VCHGSEN = 6,
	ADC_CHANNEL_VCHGBG = 7,
	ADC_CHANNEL_ISENSE = 8,
	ADC_CHANNEL_TPYD = 9,
	ADC_CHANNEL_TPYU = 10,
	ADC_CHANNEL_TPXR = 11,
	ADC_CHANNEL_TPXL = 12,
	ADC_CHANNEL_DCDCCORE = 13,
	ADC_CHANNEL_DCDCARM = 14,
	ADC_CHANNEL_DCDCMEM = 15,
	ADC_CHANNEL_DCDCLDO = 16,
	ADC_CHANNEL_DCDCGPU = 17,
	ADC_CHANNEL_DCDCWRF = 18,
	ADC_CHANNEL_VBATBK = 19,
	ADC_CHANNEL_HEADMIC = 20,
	ADC_CHANNEL_LDO0 = 21,
	ADC_CHANNEL_LDO1 = 22,
	ADC_CHANNEL_LDO2 = 23,
	ADC_CHANNEL_WHTLED = 24,
	ADC_CHANNEL_OTP = 25,
	ADC_CHANNEL_LPLDO0 = 26,	/*SIM0/SIM1/SIM2/EMMCCORE/VDD28/VDD25/USB, Low power mode reference*/
	ADC_CHANNEL_LPLDO1 = 27,	/*CAMD/EMMCIO/VDD18/AVDD18/CAMIO/CLSG, Low power mode reference*/
	ADC_CHANNEL_LPLDO2 = 28,	/*RF0/RF1/RF2/CAMA/SD/CAMMOT, Low power mode reference*/
	ADC_CHANNEL_WHTLED_VFB  = 29,
	ADC_CHANNEL_USBDP = 30,
	ADC_CHANNEL_USBDM = 31,
	ADC_2723_MAX = 31,
};
#endif

void pmic_adc_Init(void);

int32_t pmic_adc_get_values(unsigned int channel, bool scale, uint8_t num,
			    int32_t * p_buf);

int32_t pmic_adc_get_value_by_isen(unsigned int channel, int scale, uint8_t num,
				   int isen);

uint32_t sprd_chan_bat_adc_to_vol(uint16_t channel, int scale, int mux,
				  uint16_t adcvalue);

uint32_t sprd_chan_small_adc_to_vol(uint16_t channel, int scale, int mux,
				    uint16_t adcvalue);

uint32_t sprd_chan_adc_to_vol(uint16_t channel, int scale, uint8_t num,
			      int cal_type);

#endif
