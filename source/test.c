#include "test.h"
#include "init.h"


TIM_HandleTypeDef tim4;
DMA_HandleTypeDef dma_tim4_ch3;

extern uint16_t duty;

//=============================================================================================
static void gpio_init_pwm(void)
{
	GPIO_InitTypeDef gpio;
	
	__HAL_RCC_GPIOD_CLK_ENABLE();
	gpio.Mode = GPIO_MODE_AF_PP;
	gpio.Pin = LED3;
	gpio.Pull = GPIO_PULLUP;
	gpio.Speed = GPIO_SPEED_LOW;
	gpio.Alternate = GPIO_AF2_TIM4;
	
	HAL_GPIO_Init(LED_PORT, &gpio);
}


/*

w main'ie:
uint8_t up = 1;
dma_init();											//dma init for PWM from TIM4
pwm_init();											//TIM4 init with pwm mode

w while'u:
		if ( up == 1 ) duty++;
		if ( up == 0 ) duty--;
		if ( duty == 1000) up = 0;
		if ( duty == 0 ) 	 up = 1;




*/


//=============================================================================================
void pwm_init(void)
{
	TIM_OC_InitTypeDef configOC;
	
	gpio_init_pwm();		//gpio init for PWM from TIM4
	
	__HAL_RCC_TIM4_CLK_ENABLE();
	
	tim4.Instance = TIM4;
	tim4.Init.ClockDivision = TIM_CLOCKPRESCALER_DIV1;
	tim4.Init.Prescaler = 839;
  tim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  tim4.Init.Period = 999;
	
  HAL_TIM_PWM_Init(&tim4);
	
	configOC.OCMode = TIM_OCMODE_PWM1;
  configOC.OCIdleState = TIM_OCIDLESTATE_SET;
  configOC.Pulse = 0;
  configOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  configOC.OCFastMode = TIM_OCFAST_DISABLE;
	
	HAL_TIM_PWM_ConfigChannel(&tim4, &configOC, TIM_CHANNEL_3);

	//Capture/Compare 1 output enable
	TIM_CCxChannelCmd(TIM4, TIM_CHANNEL_3, TIM_CCx_ENABLE);

  HAL_TIMEx_PWMN_Start_DMA(&tim4, TIM_CHANNEL_3, (uint32_t*)&duty, 1);
}

//=============================================================================================
void dma_init(void)
{
	/* DMA controller clock enable */
  __DMA1_CLK_ENABLE();
	
	dma_tim4_ch3.Instance = DMA1_Stream7;
	dma_tim4_ch3.Init.Channel = DMA_CHANNEL_2;
  dma_tim4_ch3.Init.Direction = DMA_MEMORY_TO_PERIPH;
  dma_tim4_ch3.Init.PeriphInc = DMA_PINC_DISABLE;
  dma_tim4_ch3.Init.MemInc = DMA_MINC_DISABLE;
  dma_tim4_ch3.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
  dma_tim4_ch3.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
  dma_tim4_ch3.Init.Mode = DMA_CIRCULAR;
  dma_tim4_ch3.Init.Priority = DMA_PRIORITY_LOW;
  dma_tim4_ch3.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
  HAL_DMA_Init(&dma_tim4_ch3);
	
	__HAL_LINKDMA(&tim4, hdma[TIM_DMA_ID_CC3], dma_tim4_ch3);
}
