/*
 * msp.c
 *
 *  Created on: Sep 16, 2020
 *      Author: s0lid
 */

#include "main.h"

void HAL_MSPInit(void)
{
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);
	HAL_NVIC_SetPriority(MemoryManagement_IRQn, 0, 0);
	HAL_NVIC_SetPriority(UsageFault_IRQn, 0, 0);
	HAL_NVIC_SetPriority(BusFault_IRQn, 0, 0);

	SCB->SHCSR |= 0x7 << 16;
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
	// 1. Enable clock for TIM6 peripheral
	__HAL_RCC_TIM6_CLK_ENABLE();
	// 2. Enable IRQ handler
	HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn);
	// 3. Setup priority
	HAL_NVIC_SetPriority(TIM6_DAC_IRQn, 15, 0);

}
