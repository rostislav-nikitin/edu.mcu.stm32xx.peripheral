/*
 * msp.c
 *
 *  Created on: Sep 16, 2020
 *      Author: s0lid
 */

#include "stm32f4xx_hal.h"

void HAL_MSPInit(void)
{
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);
	HAL_NVIC_SetPriority(MemoryManagement_IRQn, 0, 0);
	HAL_NVIC_SetPriority(UsageFault_IRQn, 0, 0);
	HAL_NVIC_SetPriority(BusFault_IRQn, 0, 0);

	SCB->SHCSR |= 0x7 << 16;
}
