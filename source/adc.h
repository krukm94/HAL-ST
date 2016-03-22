/* ==================================================================================================================================

																												ADC HEADER FILE
																												Author: MK
																												Data: 22.03.2016
									
 ====================================================================================================================================
 */


#ifndef __ADC_H_
#define __ADC_H_


// ----------------------> Includes
#include "stm32f4xx_hal.h"
#include "discovery_boards.h"


// ----------------------> Macros

// ----------------------> Variables

// ----------------------> Functions
uint8_t adc_init(ADC_TypeDef *ADCx ,
								 GPIO_TypeDef *ADC_GPIO_PORT ,
								 uint16_t ADC_PIN,
								 uint32_t ADC_Channel );
uint32_t get_adc_value(void);
float get_voltage_value(void);

#endif //__ADC_H_
