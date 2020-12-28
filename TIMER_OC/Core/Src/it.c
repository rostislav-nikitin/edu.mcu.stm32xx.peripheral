/*
 * it.c
 *
 *  Created on: Sep 16, 2020
 *      Author: s0lid
 */

#include "main.h"

extern UART_HandleTypeDef uart_2;
extern TIM_HandleTypeDef htimer2;
//extern TIM_HandleTypeDef htimer6;

extern const uint32_t ch1_pulse;
extern const uint32_t ch2_pulse;
extern const uint32_t ch3_pulse;
extern const uint32_t ch4_pulse;

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

void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htimer)
{
	uint32_t ccr_content;
	uint32_t ch_pulse;
	switch(htimer->Channel)
	{
		case HAL_TIM_ACTIVE_CHANNEL_1:
			ccr_content = HAL_TIM_ReadCapturedValue(htimer, TIM_CHANNEL_1);
			__HAL_TIM_SET_COMPARE(htimer, TIM_CHANNEL_1, ccr_content + ch1_pulse);
			break;
		case HAL_TIM_ACTIVE_CHANNEL_2:
			ccr_content = HAL_TIM_ReadCapturedValue(htimer, TIM_CHANNEL_2);
			__HAL_TIM_SET_COMPARE(htimer, TIM_CHANNEL_2, ccr_content + ch2_pulse);
			break;
		case HAL_TIM_ACTIVE_CHANNEL_3:
			ccr_content = HAL_TIM_ReadCapturedValue(htimer, TIM_CHANNEL_3);
			__HAL_TIM_SET_COMPARE(htimer, TIM_CHANNEL_3, ccr_content + ch3_pulse);
			break;
		case HAL_TIM_ACTIVE_CHANNEL_4:
			ccr_content = HAL_TIM_ReadCapturedValue(htimer, TIM_CHANNEL_4);
			__HAL_TIM_SET_COMPARE(htimer, TIM_CHANNEL_4, ccr_content + ch4_pulse);
			break;
		default:
			break;
	}

}
