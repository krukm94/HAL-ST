#ifndef __DAC_H_
#define __DAC_H_

#include "stm32f4xx_hal.h"
#include "discovery_boards.h"

//=============================================================================================

typedef enum _DAC_Channel_t {
	DAC_Channel_1 = 0x00, /*!< DAC channel 1 */
	DAC_Channel_2 = 0x01  /*!< DAC channel 2 */
} DAC_Channel_t;

//=============================================================================================
/**
 * @brief  Initializes DAC channel and its GPIO pin
 * @param  DACx: DAC Channel you will use. This parameter can be a value of @ref DAC_Channel_t enumeration
 * @retval None
 */
void DAC_init(DAC_Channel_t DACx);

//=============================================================================================
/**
 * @brief  Sets analog value to ADCx
 * @param  DACx: DAC Channel you will use. This parameter can be a value of @ref TDAC_Channel_t enumeration
 * @param  value: 12-bit unsigned value for 12-bit DAC
 * @retval None
 */
void DAC_SetValue(DAC_Channel_t DACx, uint16_t value);

static void TIM6_Config(void);
void DAC1_Config(void); 

#endif //__DAC_H_
