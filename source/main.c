#include "init.h"

uint16_t duty = 0;   																														
volatile uint32_t adc;
//=============================================================================================
int main(void)
{
	init();													//system init
	//DAC1_Config();
	
	while(1)
	{
		get_adc_value();
	}
}
