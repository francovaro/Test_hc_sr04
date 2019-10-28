/*
 * hc_sr04.c
 *
 *  Created on: 28 ott 2019
 *      Author: franc
 */
#include "stm32f4xx.h"
#include "hc_sr04.h"

static uint32_t timeRead;

static void HC_SR04_Init_Pin(void);
static void HC_SR04_Init_Timer(void);

/* ------------------------------- Public function implementation -------------------------------*/

/**
 *
 */
void HC_SR04_Init(void)
{
	timeRead = 0;
	hcsr04_signalDone = RESET;
	HC_SR04_Init_Pin();
	HC_SR04_Init_Timer();
}

/**
 *
 */
void HC_SR04_StartInterrupt(void)
{

}

void HC_SR04_StopInterrupt(void)
{

}

void HC_SR04_PollRead(void)
{

}

/* Static functions */
/**
 *
 */
void HC_SR04_Init_Pin(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(HC_TRIGGER_CLOCK | HC_ECHO_CLOCK, ENABLE);

	GPIO_InitStructure.GPIO_Pin = HC_ECHO_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Fast_Speed;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(HC_TRIGGER_PORT, &GPIO_InitStructure);

	GPIO_PinAFConfig(HC_ECHO_PORT, HC_ECHO_PIN_SRC, GPIO_AF_TIM2);

	GPIO_InitStructure.GPIO_Pin = HC_ECHO_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Fast_Speed;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(HC_ECHO_PORT, &GPIO_InitStructure);
}

/* ------------------------------- Private function implementation -------------------------------*/

/**
 *
 */
void HC_SR04_Init_Timer(void)
{
	TIM_ICInitTypeDef TIM_ICInitStruct;
	RCC_ClocksTypeDef RCC_ClocksStatus;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_OCInitTypeDef TIM_OCInitStruct;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_GetClocksFreq(&RCC_ClocksStatus);
	uint16_t prescaler = RCC_ClocksStatus.SYSCLK_Frequency / 1000000 - 1; //1 tick = 1us (1 tick = 0.165mm resolution)

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	TIM_DeInit(TIM2);

	TIM_TimeBaseStructInit(&TIM_TimeBaseInitStruct);

	TIM_TimeBaseInitStruct.TIM_Prescaler = prescaler;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period = 0xFFFF;
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);


	TIM_OCStructInit(&TIM_OCInitStruct);
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStruct.TIM_Pulse = 15; //us
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC3Init(TIM2, &TIM_OCInitStruct);


	/* set TIM2 channel 1 for input compare on RISING edge */
	TIM_ICStructInit(&TIM_ICInitStruct);
	TIM_ICInitStruct.TIM_Channel = TIM_Channel_1;
	TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Rising;
	TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICInitStruct.TIM_ICFilter = 0;
	TIM_ICInit(TIM2, &TIM_ICInitStruct);

	/* set TIM2 channel 2 for input compare on FALLING edge */
	TIM_ICStructInit(&TIM_ICInitStruct);
	TIM_ICInitStruct.TIM_Channel = TIM_Channel_2;
	TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Falling;
	TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_IndirectTI;
	TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICInitStruct.TIM_ICFilter = 0;
	TIM_ICInit(TIM2, &TIM_ICInitStruct);

	//TIM_PWMIConfig(TIM2, &TIM_ICInitStruct);

	/* select the  */
	TIM_SelectInputTrigger(TIM2 , TIM_TS_TI1FP1);		/* set the input */
	TIM_SelectSlaveMode(TIM2 , TIM_SlaveMode_Reset);
	TIM_SelectMasterSlaveMode(TIM2 , TIM_MasterSlaveMode_Enable);

	TIM_ITConfig(TIM2, TIM_IT_CC1, ENABLE);
	TIM_ITConfig(TIM2, TIM_IT_CC2, ENABLE);


	// No StructInit call in API
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/**
 *
 */
void TIM2_IRQHandler(void)
{
	static uint32_t startVal = 0;
	static uint32_t endValue = 0;

	if (TIM_GetITStatus(TIM2, TIM_IT_CC1))
	{
		startVal = TIM_GetCapture1(TIM2);	/* to read channel 1 ?*/
		TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);	/* maybe not neede */
		/*vCCxIF can be cleared by software by writing it to 0 or by reading the captured data stored in the
		TIMx_CCRx register. */
	}

	if (TIM_GetITStatus(TIM2, TIM_IT_CC2))
	{
		endValue = TIM_GetCapture1(TIM2);	/* to read channel 1 ?*/
		TIM_ClearITPendingBit(TIM2, TIM_IT_CC2);	/* maybe not neede */
		/*vCCxIF can be cleared by software by writing it to 0 or by reading the captured data stored in the
		TIMx_CCRx register. */

		timeRead = endValue - startVal;
		hcsr04_signalDone = SET;
	}
}

/**
 *
 */
uint32_t HC_SR04_GetVal(void)
{
	uint32_t toReturn = timeRead;
	timeRead = 0;
	return toReturn;
}
