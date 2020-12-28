/*
 * it.c
 *
 *  Created on: Sep 16, 2020
 *      Author: s0lid
 */

#include "main.h"

extern TIM_HandleTypeDef htimer6;

void SysTick_Handler(void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}

void TIM6_DAC_IRQHandler(void)
{
	//HAL_TIM_IRQHandler(&htimer6);
	TIM6->SR &= (1 << TIM_SR_UIF);
	HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_13);
}
