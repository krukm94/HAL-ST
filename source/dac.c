#include "dac.h"


/* DAC HANDLE */
DAC_HandleTypeDef    										DAC_Handle;
static DAC_ChannelConfTypeDef 					DAC_ChannelConf;

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

static void TIM6_Config(void)
{
	TIM_HandleTypeDef  					tim;
  TIM_MasterConfigTypeDef 		sMasterConfig;
  
  /*##-1- Configure the TIM peripheral #######################################*/
  /* Time base configuration */
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

void DAC1_Config(void)
{
	DAC_Channel_t DACx = DAC_Channel_1;
	GPIO_InitTypeDef gpio;
	uint16_t GPIO_Pin;
	DAC_Handle.Instance = DAC;
	
	//tim6 init and config
	TIM6_Config();
	
	/* Select proper GPIO pin */
	if (DACx == DAC_Channel_1) 
	{
		GPIO_Pin = GPIO_PIN_4;
	} 
	else 
	{
		GPIO_Pin = GPIO_PIN_5;
	}
	
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
}

void HAL_DAC_MspInit(DAC_HandleTypeDef* hdac)
{
  static DMA_HandleTypeDef  hdma_dac1;
  
  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* DAC Periph clock enable */
  __HAL_RCC_DAC_CLK_ENABLE();
 /* DMA1 clock enable */
  DMAx_CLK_ENABLE();
  
  
  /*##-3- Configure the DMA streams ##########################################*/
  /* Set the parameters to be configured for Channel1*/
  hdma_dac1.Instance = DAC_DMA_STREAM1;
  
  hdma_dac1.Init.Channel  = DAC_DMA_CHANNEL1;
  hdma_dac1.Init.Direction = DMA_MEMORY_TO_PERIPH;
  hdma_dac1.Init.PeriphInc = DMA_PINC_DISABLE;
  hdma_dac1.Init.MemInc = DMA_MINC_ENABLE;
  hdma_dac1.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
  hdma_dac1.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
  hdma_dac1.Init.Mode = DMA_CIRCULAR;
  hdma_dac1.Init.Priority = DMA_PRIORITY_HIGH;
  hdma_dac1.Init.FIFOMode = DMA_FIFOMODE_DISABLE;         
  hdma_dac1.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_HALFFULL;
  hdma_dac1.Init.MemBurst = DMA_MBURST_SINGLE;
  hdma_dac1.Init.PeriphBurst = DMA_PBURST_SINGLE; 

  HAL_DMA_Init(&hdma_dac1);
    
  /* Associate the initialized DMA handle to the the DAC handle */
  __HAL_LINKDMA(hdac, DMA_Handle1, hdma_dac1);

  /*##-4- Configure the NVIC for DMA #########################################*/
  /* Enable the DMA1 Stream5 IRQ Channel */
  HAL_NVIC_SetPriority(DAC_DMA_IRQn1, 2, 0);
  HAL_NVIC_EnableIRQ(DAC_DMA_IRQn1);
}
  
/**
  * @brief  DeInitializes the DAC MSP.
  * @param  hdac: pointer to a DAC_HandleTypeDef structure that contains
  *         the configuration information for the specified DAC.  
  * @retval None
  */
void HAL_DAC_MspDeInit(DAC_HandleTypeDef* hdac)
{
  static DMA_HandleTypeDef  hdma_dac1;

  /*##-2- Disable peripherals and GPIO Clocks ################################*/
  /* De-initialize the DAC Channel1 GPIO pin */
  HAL_GPIO_DeInit(DAC_CHANNEL1_GPIO_PORT, DAC_CHANNEL1_PIN);
  
  /*##-3- Disable the DMA Streams ############################################*/
  /* De-Initialize the DMA Stream associate to Channel1 */
  hdma_dac1.Instance = DAC_DMA_STREAM1;
  HAL_DMA_DeInit(&hdma_dac1);
    
  /*##-4- Disable the NVIC for DMA ###########################################*/
  HAL_NVIC_DisableIRQ(DAC_DMA_IRQn1);
}