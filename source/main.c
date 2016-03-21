#include "init.h"

uint16_t duty = 0;   																														

//=============================================================================================
int main(void)
{
	init();													//system init
	DAC1_Config();
	
	while(1)
	{
		
	}
}
