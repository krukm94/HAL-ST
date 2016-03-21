#include "uart.h"


//=============================================================================================
void uart_init(USART_TypeDef UARTx)
{
	GPIO_InitTypeDef						gpio;
	USART_InitTypeDef 					usart;
	
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_USART2_CLK_ENABLE();
	
	gpio.Pin = GPIO_PIN_2 | GPIO_PIN_3;
	gpio.Mode = GPIO_MODE_AF_PP;
	gpio.Pull = GPIO_PULLUP;
	gpio.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	gpio.Alternate = GPIO_AF7_USART2;
	HAL_GPIO_Init(GPIOA, &gpio);
	
	
	usart.BaudRate = 115200;
	usart.WordLength = USART_WORDLENGTH_8B;
	usart.StopBits = USART_STOPBITS_1;
	usart.Parity = USART_PARITY_NONE;
	usart.Mode = USART_MODE_TX_RX;
	
	//HAL_USART_Init(
	
}
