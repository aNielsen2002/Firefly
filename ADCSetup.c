#include <adc.h>
#include "Globals.h"
inline void SetupADC(void)
{

	AD1CON2=(
		ADC_VREF_AVDD_AVSS			&//Analog refs are Vdd and Vss
		ADC_SCAN_OFF					&//no scan No kill
		ADC_SELECT_CHAN_0				&//chan 0
		ADC_BUF_FILL_0x0_0x7			&
		ADC_SAMPLES_PER_INT_1			&
		ADC_SAMPLE_TIME_31			&
		ADC_CONV_CLK_INTERNAL_RC	&
		ADC_ALT_INPUT_OFF				&
		ADC_ALT_BUF_OFF				&
		0xE7BF);							//unused bits Masked
	
	AD1CON3=(
		ADC_SAMPLE_TIME_31			&//Longest sample time
		ADC_CONV_CLK_INTERNAL_RC   	&//A2D ownclock
		ADC_CONV_CLK_1Tcy				&//Does not mater
		0x9fff);								//unused bits Masked
		
	AD1CHS0	=	1;//VREF- for lower and AN3 for +

	AD1CON1=(
		ADC_MODULE_ON					&
		ADC_IDLE_STOP					&
		ADC_AD12B_10BIT					&//IS 10 bit
		ADC_ADDMABM_ORDER			&
		ADC_FORMAT_INTG				&
		ADC_AUTO_SAMPLING_OFF		&
		ADC_MULTIPLE					&
		ADC_CLK_AUTO					&
		ADC_SAMP_OFF					&//OFF
		0xA7EE);							//Unused bits mask

	ConfigIntADC1(
		ADC_INT_PRI_6	&//Level 6...
		ADC_INT_DISABLE
		);
    
    AD1CHS0bits.CH0SA = 6;
    
	return;
	}
