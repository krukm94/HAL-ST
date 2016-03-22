/* ==================================================================================================================================

																												USART SOURCE FILE
																												Author: MK
																												Data: 22.03.2016
									
 ====================================================================================================================================
 */


#include "uart.h"

// TYPEdef's and Handles
UART_HandleTypeDef USART_HandleStruct;

//=============================================================================================
uint8_t usart_init(USART_TypeDef 	*USARTx,
										uint32_t			BAUDRATE,
										GPIO_TypeDef 	*GPIOx,					//PORT TX i RX pin's np. GPIOA
										uint16_t 			USART_PIN_TX, 
										uint16_t 			USART_PIN_RX) {

	
	//zmienna do zwracania statusu funkcji
	uint8_t usart_status;
	
	//inicjalizacja struktur od GPIO
	GPIO_InitTypeDef     GPIO_InitStruct;
	
	usart_status = 0x01;
 
/* Enable GPIO clock */
	USARTx_GPIO_CLK_ENABLE();

// Enable clock for USART1 peripheral
	USARTx_CLK_ENABLE();
	

	
/* Configure USART Tx as alternate function */
		GPIO_InitStruct.Pin = USART_PIN_TX;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
		GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
		HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
	
/* Configure USART Rx as alternate function */
		GPIO_InitStruct.Pin = USART_PIN_RX;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
		GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
		HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
		//Priorytet przerwania
		
		NVIC_SetPriority(USART2_IRQn, 2);

		NVIC_EnableIRQ(USART2_IRQn);

//Handle
		USART_HandleStruct.Instance = USARTx;
		USART_HandleStruct.Init.BaudRate = BAUDRATE;
		USART_HandleStruct.Init.StopBits = USART_STOPBITS_1;
		USART_HandleStruct.Init.Parity = USART_PARITY_NONE;
		USART_HandleStruct.Init.Mode = USART_MODE_TX_RX;
		
	
//Init
		usart_status = HAL_UART_Init(&USART_HandleStruct);
	
		
//USART2 enable	
		__HAL_UART_ENABLE(&USART_HandleStruct);

			#ifdef USART_IT_RX_ON
			USART1 -> CR1 |= USART_CR1_RXNEIE;
			#endif
			
			
			usart_WriteS("\n\r\n\r---> USART INIT OK <---\n\r\n\r");
			
		return usart_status;
}
/* ==================================================================================================================================

																												usart_Write
									
 ====================================================================================================================================
 */
void usart_Write(char data){	
	while(!(__HAL_USART_GET_FLAG(&USART_HandleStruct, USART_FLAG_TXE)));	
		//HAL_UART_Transmit(&USART_HandleStruct,(uint8_t*) data, 8 , 100); <--- TO KURESTWO NIE DZIALA
	USART2 -> DR = data;
} //void usart_Write(char data)
/* ==================================================================================================================================

																												usart_WriteS
									
 ====================================================================================================================================
 */
void usart_WriteS(char *s){
	while(*s)
	{
		while(!(__HAL_USART_GET_FLAG(&USART_HandleStruct, USART_FLAG_TC)));
		usart_Write(*s++);
		
	}
}//void usart_WriteS(char *s)

/* ==================================================================================================================================

																												END OF FILE
									
 ====================================================================================================================================
 */

