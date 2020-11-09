/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/

#include "lib_uart.h"

#include "stm32f4xx.h"
#include <string.h>
#include <stdio.h>
#include <stdint.h>


#include "systick.h"
#include "hc_sr04.h"
			

int main(void)
{
	uint32_t 	readVal;
	char 		stringa[15];
	/* peripheral setup */
	UART_lib_config(e_UART_2, DISABLE, 0, 0);

	/* Init timer */
	HC_SR04_Init();
	HC_SR04_StartInterrupt();

	/* set systick */
	setSysTick(1000);

	UART_lib_sendData("START\n", strlen("START\n"));

	/* main while */
	while(1)
	{
		if (hcsr04_signalDone == SET)
		{
			readVal = HC_SR04_GetVal()*0.17;
			hcsr04_signalDone = RESET;
		}

		if (interruptSys == SET)
		{
			snprintf(stringa, sizeof(stringa), "%lu\n", readVal);
			UART_lib_sendData(stringa, strlen(stringa));
			interruptSys = RESET;
		}
	}
}
