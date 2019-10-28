/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/


#include "stm32f4xx.h"

#include "uart.h"
#include "systick.h"
#include "hc_sr04.h"
			

int main(void)
{
	uint32_t readVal;
	/* peripheral setup */
	UART_fv_config(0);
	/* main while */

	setSysTick(100);

	while(1)
	{
		if (hcsr04_signalDone == SET)
		{
			readVal = HC_SR04_GetVal();
			hcsr04_signalDone = RESET;
		}
	}
}
