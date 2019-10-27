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
			

int main(void)
{
	/* peripheral setup */
	UART_fv_config(0);;

	/* main while */
	while(1)
	{

	}
}
