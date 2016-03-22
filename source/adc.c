/* ==================================================================================================================================

																												ADC SOURCE FILE
																												Author: MK
																												Data: 22.03.2016
									
 ====================================================================================================================================
 */

// -------------------------> INCLUDES
#include "adc.h"

volatile uint32_t adc_value;
volatile float voltage_value;
// -------------------------> Handles

//TYPEdef's for GPIO
GPIO_InitTypeDef GPIO_Init_Str;

//TYPEdef's and Handle's for ADC
ADC_ChannelConfTypeDef ADC_Channel_Str;
ADC_HandleTypeDef ADC_Init;

/* ==================================================================================================================================

																											ADC INIT FUNCTION
									
 ====================================================================================================================================
 */

uint8_t adc_init(ADC_TypeDef *ADCx ,
								 GPIO_TypeDef *ADC_GPIO_PORT ,
								 uint16_t ADC_PIN,
								 uint32_t ADC_Channel ){
		
	uint8_t adc_status;
	
		// RCC ON FOR ADC
		ADC1_CLK_ENABLE();
		// RCC ON FOR GPIO
		ADC1_GPIO_CLK_ENABLE();
		
		adc_status = 0x01;
	
		//GPIO config ( If you want use ADC that pins must be in analog mode
		GPIO_Init_Str.Pin = 	ADC_PIN;
		GPIO_Init_Str.Mode = GPIO_MODE_ANALOG;
		GPIO_Init_Str.Pull = GPIO_NOPULL;
		//Inicjalizacjia GPIO
		HAL_GPIO_Init(ADC_GPIO_PORT,&GPIO_Init_Str);
	
		//ADC CONFIG
		/* Configuration below set:
		12 bit resolution
		Continous Mode of mesurment
		Prescaler for ADC Clock = 8
		*/
		ADC_Init.Instance = ADCx;
		ADC_Init.Init.Resolution = ADC_RESOLUTION12b;
		ADC_Init.Init.ContinuousConvMode = ENABLE;
		ADC_Init.Init.ScanConvMode = DISABLE;
		ADC_Init.Init.NbrOfConversion = 1;
		ADC_Init.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
		ADC_Init.Init.DataAlign = ADC_DATAALIGN_RIGHT;
		ADC_Init.Init.ClockPrescaler = ADC_CLOCKPRESCALER_PCLK_DIV8;  // f ABP2 = 84 MHz / 8 = 10,5 MHz
		
		//ADC INIT
		adc_status = HAL_ADC_Init(&ADC_Init);
		
		//ADC CHANNEL CONFIGURATION
		ADC_Channel_Str.Channel =	ADC_Channel;
		ADC_Channel_Str.Rank = 1;
		ADC_Channel_Str.SamplingTime = ADC_SAMPLETIME_28CYCLES;
		ADC_Channel_Str.Offset = 0;
		
		//CHANNEL CONFIGURATION INIT
		HAL_ADC_ConfigChannel(&ADC_Init,&ADC_Channel_Str);
		
		//SET ADC ENABLE
		__HAL_ADC_ENABLE(&ADC_Init);
		
		//START FIRST CONVERSATION
		HAL_ADC_Start(&ADC_Init);	
		
		return adc_status;
		
} //END OF: uint8_t adc_init(ADC_TypeDef *ADCx)

/* ==================================================================================================================================

																												get_adc_value
									
 ====================================================================================================================================
 */
uint32_t get_adc_value(void){

		if (HAL_ADC_PollForConversion(&ADC_Init, 100) == HAL_OK)
		adc_value = HAL_ADC_GetValue(&ADC_Init);
		return adc_value;
}
/* ==================================================================================================================================

																												get_voltage_value
									
 ====================================================================================================================================
 */
float get_voltage_value(void){

		return (adc_value*(V_REF/RESOLUTION));
}
/* ==================================================================================================================================

																												END OF FILE
									
 ====================================================================================================================================
 */
