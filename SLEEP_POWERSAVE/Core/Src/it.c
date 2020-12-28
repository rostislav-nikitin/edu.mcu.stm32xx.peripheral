/*
 * it.c
 *
 *  Created on: Sep 16, 2020
 *      Author: s0lid
 */

#include "main.h"

extern UART_HandleTypeDef uart_2;
extern TIM_HandleTypeDef htimer6;

void SysTick_Handler(void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}

void USART2_IRQHandler(void)
{
	HAL_UART_IRQHandler(&uart_2);
}

void TIM6_DAC_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&htimer6);
}
