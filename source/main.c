#include "init.h"

uint16_t duty = 0;   																														
volatile uint32_t adc;
//=============================================================================================
int main(void)
{
	init();													//system init
	DAC1_Config();									//DAC with DMA triggered from TIM6
	
	while(1)
	{
		//get_adc_value();
	}
}
