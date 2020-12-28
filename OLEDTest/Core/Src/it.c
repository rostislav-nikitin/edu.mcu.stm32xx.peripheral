/*
 * it.c
 *
 *  Created on: Oct 17, 2020
 *      Author: s0lid
 */

#include "it.h"

void SysTick_Handler(void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}
