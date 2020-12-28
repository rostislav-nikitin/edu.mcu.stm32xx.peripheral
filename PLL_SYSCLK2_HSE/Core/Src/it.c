/*
 * it.c
 *
 *  Created on: Sep 16, 2020
 *      Author: s0lid
 */

#include "main.h"

void SysTick_Handler(void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}
