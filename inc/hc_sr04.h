/*
 * hc_sr04.h
 *
 *  Created on: 28 ott 2019
 *      Author: franc
 */

#ifndef HC_SR04_H_
#define HC_SR04_H_

#define HC_TRIGGER_PIN		GPIO_Pin_10
#define HC_TRIGGER_PORT		GPIOB
#define HC_TRIGGER_CLOCK	RCC_AHB1Periph_GPIOB

#define HC_ECHO_PIN			GPIO_Pin_0
#define HC_ECHO_PIN_SRC		GPIO_Pin_0
#define HC_ECHO_PORT		GPIOA
#define HC_ECHO_CLOCK		RCC_AHB1Periph_GPIOA

volatile uint8_t hcsr04_signalDone;

extern void HC_SR04_Init(void);
extern void HC_SR04_StartInterrupt(void);
extern void HC_SR04_StopInterrupt(void);
extern void HC_SR04_PollRead(void);
extern uint32_t HC_SR04_GetVal(void);

#endif /* HC_SR04_H_ */
