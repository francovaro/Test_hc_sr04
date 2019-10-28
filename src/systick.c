/*
 * systick.c
 *
 *  Created on: 28 ott 2019
 *      Author: franc
 */

#include "stm32f4xx.h"
#include "systick.h"

void setSysTick(uint32_t timeMs)
{
	 RCC_ClocksTypeDef RCC_Clocks;
	 RCC_GetClocksFreq(&RCC_Clocks);
	 SysTick_Config(RCC_Clocks.HCLK_Frequency/timeMs); // hz/s
}

void SysTick_Handler(void)
{

}
