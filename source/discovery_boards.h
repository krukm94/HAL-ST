#ifndef __DISCOVERY_BOARDS_H__
#define __DISCOVERY_BOARDS_H__

//=============================================================================================

#ifdef STM32F4_DISCOVERY

#define LED_PORT		GPIOD
#define LED1				GPIO_PIN_12
#define LED2				GPIO_PIN_13
#define LED3				GPIO_PIN_14
#define LED4				GPIO_PIN_15
#define LED_ALL			GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15

/* Definition for DAC clock resources */
#define DACx_CHANNEL1_GPIO_CLK_ENABLE()  	__HAL_RCC_GPIOA_CLK_ENABLE()
#define DMAx_CLK_ENABLE()               	__HAL_RCC_DMA1_CLK_ENABLE()  

/* Definition for DACx Channel1 Pin */
#define DAC_CHANNEL1_PIN                	GPIO_PIN_4
#define DAC_CHANNEL1_GPIO_PORT          	GPIOA 

/* Definition for DACx's DMA Channel1 */
#define DAC_DMA_CHANNEL1                	DMA_CHANNEL_7
#define DAC_DMA_STREAM1                 	DMA1_Stream5 

/* Definition for DACx's NVIC */
#define DAC_DMA_IRQn1                   	DMA1_Stream5_IRQn
#define DAC_DMA_IRQHandler1             	DMA1_Stream5_IRQHandler

/* Definition for ADC clock resourses */
#define ADC1_CLK_ENABLE() 								__HAL_RCC_ADC1_CLK_ENABLE()
#define ADC1_GPIO_CLK_ENABLE()						__HAL_RCC_GPIOA_CLK_ENABLE()

/* Definition resolution and V_REF of ADC  
Example: if your resolution is 12b you mast set RESOLUTION 4096 it's logic!
*/
#define RESOLUTION												4096
#define V_REF															2.96


/* Definition for USART clock resources */
#define USARTx_CLK_ENABLE()								__HAL_RCC_USART2_CLK_ENABLE();

/* Definition for USART GPIO clock resourses */
#define USARTx_GPIO_CLK_ENABLE()					__HAL_RCC_GPIOA_CLK_ENABLE();

/* Definition Interrups for USARTS */
#define USART_IT_RX_ON										0



#endif

//=============================================================================================

#ifdef STM32F7_DISCOVERY

#define LED_PORT		GPIOI
#define LED1				GPIO_PIN_1
#define LED_ALL			LED1

#endif


#endif //__DISCOVERY_BOARDS_H__
