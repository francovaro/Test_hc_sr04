/*
 * systick.c
 *
 *  Created on: 28 ott 2019
 *      Author: franc
 */

#include "stm32f4xx.h"
#include "systick.h"

#define SYSTICK_LIMIT	100

void setSysTick(uint32_t timeMs)
{
	 RCC_ClocksTypeDef RCC_Clocks;
	 RCC_GetClocksFreq(&RCC_Clocks);
	 SysTick_Config(RCC_Clocks.HCLK_Frequency/timeMs); // hz/s

	 interruptSys = RESET;
}

void SysTick_Handler(void)
{
	static uint16_t counter = 0;

	counter++;

	if(counter > SYSTICK_LIMIT)
	{
		interruptSys = SET;
	}
}
