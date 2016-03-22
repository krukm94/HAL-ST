#include "dac.h"

//=============================================================================================

#define   OUT_FREQ          3200                                 // Output waveform frequency
#define   SINE_RES          127                                  // Waveform resolution
#define   CNT_FREQ          84000000                             // TIM6 counter clock (prescaled APB1)
#define   TIM_PERIOD        ((CNT_FREQ)/((SINE_RES)*(OUT_FREQ))) // Autoreload reg value

//=============================================================================================

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
																			
/* DAC HANDLE */
DAC_HandleTypeDef    										DAC_Handle;
static DAC_ChannelConfTypeDef 					DAC_ChannelConf;

//=============================================================================================
//void DAC_init(DAC_Channel_t DACx) 
//{
//	GPIO_InitTypeDef gpio;
//	uint16_t GPIO_Pin;
//	
//	/* Select proper GPIO pin */
//	if (DACx == DAC_Channel_1) 
//	{
//		GPIO_Pin = GPIO_PIN_4;
//	} 
//	else 
//	{
//		GPIO_Pin = GPIO_PIN_5;
//	}
//	
//	/* clock enable */
//	__HAL_RCC_DAC_CLK_ENABLE();
//	__HAL_RCC_GPIOD_CLK_ENABLE();
//	
//	/* gpio config */
//	gpio.Mode = GPIO_MODE_ANALOG;
//	gpio.Pin = GPIO_Pin;
//	gpio.Pull = GPIO_NOPULL;
//	gpio.Speed = GPIO_SPEED_HIGH;
//	/* gpio init */
//	HAL_GPIO_Init(GPIOA, &gpio);
//	
//	/* Set DAC options */
//	DAC_Handle[(uint8_t)DACx].Instance = DAC;
//	
//	/* Set channel settings */
//	DAC_ChannelConf.DAC_Trigger = DAC_TRIGGER_NONE;
//	DAC_ChannelConf.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
//	
//	/* Init DAC */
//	HAL_DAC_Init(&DAC_Handle[(uint8_t)DACx]);
//	
//	/* Init and enable proper DAC */
//	if (DACx == DAC_Channel_1) 
//	{
//		/* Init channel 1 */
//		HAL_DAC_ConfigChannel(&DAC_Handle[(uint8_t)DACx], &DAC_ChannelConf, DAC_CHANNEL_1);
//		
//		/* Enable DAC channel 1 */
//		DAC->CR |= DAC_CR_EN1;
//	} 
//	else 
//	{
//		/* Init channel 2 */
//		HAL_DAC_ConfigChannel(&DAC_Handle[(uint8_t)DACx], &DAC_ChannelConf, DAC_CHANNEL_2);
//		
//		/* Enable DAC channel 2 */
//		DAC->CR |= DAC_CR_EN2;
//	}
//}

//void DAC_SetValue(DAC_Channel_t DACx, uint16_t value) {
//	/* Check value */
//	if (value > 0x0FFF) {
//		value = 0x0FFF;
//	}
//	
//	/* Set 12-bit value, right aligned */
//	if (DACx == DAC_Channel_1) {
//#if defined(STM32F4) || defined(STM32F7)
//		DAC->DHR12R1 = value;
//#else
//		HAL_DAC_SetValue(&DAC_Handle[(uint8_t)DACx], DAC_CHANNEL_1, DAC_ALIGN_12B_R, value);
//#endif
//	} else {
//#if defined(STM32F4) || defined(STM32F7)
//		DAC->DHR12R2 = value;
//#else
//		HAL_DAC_SetValue(&DAC_Handle[(uint8_t)DACx], DAC_CHANNEL_2, DAC_ALIGN_12B_R, value);
//#endif
//	}
//}

//=============================================================================================
static void TIM6_Config(void)
{
	TIM_HandleTypeDef  					tim;
  TIM_MasterConfigTypeDef 		sMasterConfig;
  
  /*##-1- Configure the TIM peripheral #######################################*/
  tim.Instance = TIM6;
  
  tim.Init.Period = (uint16_t)TIM_PERIOD;          
  tim.Init.Prescaler = 0;       
  tim.Init.ClockDivision = 0;    
  tim.Init.CounterMode = TIM_COUNTERMODE_UP; 
  HAL_TIM_Base_Init(&tim);

  /* TIM6 TRGO selection */
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  
  HAL_TIMEx_MasterConfigSynchronization(&tim, &sMasterConfig);
  
  /*##-2- Enable TIM peripheral counter ######################################*/
  HAL_TIM_Base_Start(&tim);
}

//=============================================================================================
void DAC1_Config(void)
{
	DAC_Channel_t DACx = DAC_Channel_1;
	GPIO_InitTypeDef gpio;
	uint16_t GPIO_Pin;
	DAC_Handle.Instance = DAC;
	
	//tim6 init and config
	TIM6_Config();
	
	/* Select proper GPIO pin */
	if (DACx == DAC_Channel_1) GPIO_Pin = GPIO_PIN_4;
	else GPIO_Pin = GPIO_PIN_5;
	
	/* clock enable */
	__HAL_RCC_DAC_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	
	/* gpio config */
	gpio.Mode = GPIO_MODE_ANALOG;
	gpio.Pin = GPIO_Pin;
	gpio.Pull = GPIO_NOPULL;
	gpio.Speed = GPIO_SPEED_HIGH;
	/* gpio init */
	HAL_GPIO_Init(GPIOA, &gpio);
	
  
  HAL_DAC_Init(&DAC_Handle);
  
  /*##-1- DAC channel1 Configuration #########################################*/
  DAC_ChannelConf.DAC_Trigger = DAC_TRIGGER_T6_TRGO;
  DAC_ChannelConf.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;  

  HAL_DAC_ConfigChannel(&DAC_Handle, &DAC_ChannelConf, DAC_CHANNEL_1);
	
	HAL_DAC_Start(&DAC_Handle, DAC_CHANNEL_1);
  
  /*##-2- Enable DAC Channel1 and associated DMA #############################*/
  HAL_DAC_Start_DMA(&DAC_Handle, DAC_CHANNEL_1, (uint32_t*)function, SINE_RES, DAC_ALIGN_12B_R);
} //void DAC1_Config(void)

//=============================================================================================
void HAL_DAC_MspInit(DAC_HandleTypeDef* hdac)
{
  static DMA_HandleTypeDef  dmaDAC_handle;
  
  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* DAC Periph clock enable */
  __HAL_RCC_DAC_CLK_ENABLE();
 /* DMA1 clock enable */
  DMAx_CLK_ENABLE();
  
  /*##-3- Configure the DMA streams ##########################################*/
  /* Set the parameters to be configured for Channel1*/
  dmaDAC_handle.Instance = DAC_DMA_STREAM1;
  
  dmaDAC_handle.Init.Channel  = DAC_DMA_CHANNEL1;
  dmaDAC_handle.Init.Direction = DMA_MEMORY_TO_PERIPH;
  dmaDAC_handle.Init.PeriphInc = DMA_PINC_DISABLE;
  dmaDAC_handle.Init.MemInc = DMA_MINC_ENABLE;
  dmaDAC_handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
  dmaDAC_handle.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
  dmaDAC_handle.Init.Mode = DMA_CIRCULAR;
  dmaDAC_handle.Init.Priority = DMA_PRIORITY_HIGH;
  dmaDAC_handle.Init.FIFOMode = DMA_FIFOMODE_DISABLE;         
  dmaDAC_handle.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_HALFFULL;
  dmaDAC_handle.Init.MemBurst = DMA_MBURST_SINGLE;
  dmaDAC_handle.Init.PeriphBurst = DMA_PBURST_SINGLE; 

  HAL_DMA_Init(&dmaDAC_handle);
    
  /* Associate the initialized DMA handle to the the DAC handle */
  __HAL_LINKDMA(hdac, DMA_Handle1, dmaDAC_handle);

  /*##-4- Configure the NVIC for DMA #########################################*/
  /* Enable the DMA1 Stream5 IRQ Channel */
  HAL_NVIC_SetPriority(DAC_DMA_IRQn1, 2, 0);
  HAL_NVIC_EnableIRQ(DAC_DMA_IRQn1);
} // void HAL_DAC_MspInit(DAC_HandleTypeDef* hdac)
 
//=============================================================================================
/**
  * @brief  DeInitializes the DAC MSP.
  * @param  hdac: pointer to a DAC_HandleTypeDef structure that contains
  *         the configuration information for the specified DAC.  
  * @retval None
  */
void HAL_DAC_MspDeInit(DAC_HandleTypeDef* hdac)
{
  static DMA_HandleTypeDef  dmaDAC_handle;

  /*##-2- Disable peripherals and GPIO Clocks ################################*/
  /* De-initialize the DAC Channel1 GPIO pin */
  HAL_GPIO_DeInit(DAC_CHANNEL1_GPIO_PORT, DAC_CHANNEL1_PIN);
  
  /*##-3- Disable the DMA Streams ############################################*/
  /* De-Initialize the DMA Stream associate to Channel1 */
  dmaDAC_handle.Instance = DAC_DMA_STREAM1;
  HAL_DMA_DeInit(&dmaDAC_handle);
    
  /*##-4- Disable the NVIC for DMA ###########################################*/
  HAL_NVIC_DisableIRQ(DAC_DMA_IRQn1);
}
