#ifndef __INIT_H_
#define __INIT_H_

//=============================================================================================

#include "stm32f4xx_hal.h"
#include "discovery_boards.h"

//============================================= MY INCLUDES ===================================

#include "test.h"
#include "uart.h"
#include "dac.h"

//=============================================================================================
void init(void);
static void SystemClock_Config(void);
static void led_init(void);
void led_on(uint16_t led);
void led_off(uint16_t led);
void led_toggle(uint16_t led);

#endif //__INIT_H_
