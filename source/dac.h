#ifndef __DAC_H_
#define __DAC_H_

#define   OUT_FREQ          3200                                 // Output waveform frequency
#define   SINE_RES          127                                  // Waveform resolution
#define   CNT_FREQ          84000000                             // TIM6 counter clock (prescaled APB1)
#define   TIM_PERIOD        ((CNT_FREQ)/((SINE_RES)*(OUT_FREQ))) // Autoreload reg value 

//=============================================================================================

#include "stm32f4xx_hal.h"
#include "discovery_boards.h"
#include "test.h"

//=============================================================================================

typedef enum _DAC_Channel_t {
	DAC_Channel_1 = 0x00, /*!< DAC channel 1 */
	DAC_Channel_2 = 0x01  /*!< DAC channel 2 */
} DAC_Channel_t;


const uint16_t function[SINE_RES] = { 2048, 2145, 2242, 2339, 2435, 2530, 2624, 2717, 2808, 2897, 
                                      2984, 3069, 3151, 3230, 3307, 3381, 3451, 3518, 3581, 3640, 
                                      3696, 3748, 3795, 3838, 3877, 3911, 3941, 3966, 3986, 4002, 
                                      4013, 4019, 4020, 4016, 4008, 3995, 3977, 3954, 3926, 3894, 
                                      3858, 3817, 3772, 3722, 3669, 3611, 3550, 3485, 3416, 3344, 
                                      3269, 3191, 3110, 3027, 2941, 2853, 2763, 2671, 2578, 2483, 
                                      2387, 2291, 2194, 2096, 1999, 1901, 1804, 1708, 1612, 1517, 
                                      1424, 1332, 1242, 1154, 1068, 985, 904, 826, 751, 679, 
                                      610, 545, 484, 426, 373, 323, 278, 237, 201, 169, 
                                      141, 118, 100, 87, 79, 75, 76, 82, 93, 109, 
                                      129, 154, 184, 218, 257, 300, 347, 399, 455, 514, 
                                      577, 644, 714, 788, 865, 944, 1026, 1111, 1198, 1287, 
                                      1378, 1471, 1565, 1660, 1756, 1853, 1950 };  


//=============================================================================================
/**
 * @brief  Initializes DAC channel and its GPIO pin
 * @param  DACx: DAC Channel you will use. This parameter can be a value of @ref DAC_Channel_t enumeration
 * @retval None
 */
void DAC_init(DAC_Channel_t DACx);

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
