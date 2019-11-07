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
#include <string.h>

#include "uart.h"
#include "systick.h"
#include "hc_sr04.h"
			

static void _converti(uint16_t dato,char str[]);

int main(void)
{
	uint32_t 	readVal;
	uint8_t 	stringa[15];
	/* peripheral setup */
	UART_fv_config(0);

	/* Init timer */
	HC_SR04_Init();

	setSysTick(100);
	HC_SR04_StartInterrupt();

	UART_fv_SendData("START\n", strlen("START\n"));

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
			snprintf(stringa, sizeof(stringa), "%d\n", readVal);
			UART_fv_SendData(stringa, strlen(stringa));
			interruptSys = RESET;
		}
	}
}

void _converti(uint16_t dato, char str[])
{
	unsigned char iniziato=0,quoz=0,index=0;
	unsigned int base,rest;

	for (base=1000;base;base/=10)
	{
		quoz=dato/base;
		rest=dato%base;
		if(!iniziato)
		{
			if(quoz)
			{
				str[index++]=quoz + '0';
				iniziato=1;
			}
		}
		else
		{
			str[index++]=quoz + '0';
		}
		dato=rest;
	}
	if(!iniziato)
	{
		str[index++]=quoz + '0';
	}

	str[index]=0;
}
