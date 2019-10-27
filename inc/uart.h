/*
 * uart.h
 *
 *  Created on: 05 ott 2019
 *      Author: franc
 */

#ifndef UART_H_
#define UART_H_

#include "stm32f4xx_usart.h"

#define USART_MAX_BUFFER (255u)

volatile uint8_t usart_rxBuffer[USART_MAX_BUFFER];

volatile FlagStatus usart_serialReceived;
volatile uint16_t usart_nrOfByte;

extern void UART_fv_config(uint8_t irqEnabled);
extern void UART_fv_SendData(const char * strToSend, uint16_t byteToSend);

int uart_putc(int c);

#endif /* UART_H_ */
