#include "init.h"

uint16_t duty = 0;   																														

//=============================================================================================
int main(void)
{
	init();													//system init
	DAC1_Config();									//DAC with DMA triggered from TIM6
	
	while(1)
	{
		
	}
}
