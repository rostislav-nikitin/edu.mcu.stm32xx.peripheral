/*
 * it.c
 *
 *  Created on: Sep 16, 2020
 *      Author: s0lid
 */

#include "main.h"

extern UART_HandleTypeDef uart_2;
extern TIM_HandleTypeDef htimer2;

void SysTick_Handler(void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}

void USART2_IRQHandler(void)
{
	HAL_UART_IRQHandler(&uart_2);
}

/*void TIM6_DAC_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&htimer6);
}*/

void TIM2_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&htimer2);
}

#define DOWN	0
#define UP		1

uint8_t direction = UP;
uint8_t step = 20;

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{

	uint32_t brightness = __HAL_TIM_GET_COMPARE(htim, TIM_CHANNEL_1);
	if(direction == UP)
	{
		if(brightness < htim->Instance->ARR)
		{
			__HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_1, brightness + step);
		}
		else
		{
			direction = DOWN;
		}
	}
	else
	{
		if(brightness > 1)
		{
			__HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_1, brightness - step);
		}
		else
		{
			direction = UP;
		}
	}
}
