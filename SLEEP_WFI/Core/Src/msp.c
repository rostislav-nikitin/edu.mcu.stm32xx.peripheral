/*
 * msp.c
 *
 *  Created on: Sep 16, 2020
 *      Author: s0lid
 */

#include "main.h"

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
	if(huart->Instance == USART2)
	{
		// here we are going to do low levelt init
		// 1. Enable Clock for USART2 periph.
		//RCC->AHB1ENR |= (1 << RCC_APB1ENR_USART2EN_Msk);
		__HAL_RCC_USART2_CLK_ENABLE();
		// 2. Configure GPIO
		__HAL_RCC_GPIOA_CLK_ENABLE();
		GPIO_InitTypeDef gpioUsart2;

		// GPIOA::PIN2 (TX)
		gpioUsart2.Pin = GPIO_PIN_2;
		gpioUsart2.Mode = GPIO_MODE_AF_PP;
		gpioUsart2.Pull = GPIO_PULLUP;
		gpioUsart2.Speed = GPIO_SPEED_FREQ_LOW;
		gpioUsart2.Alternate = GPIO_AF7_USART2;
		HAL_GPIO_Init(GPIOA, &gpioUsart2);
		// GPIOA::PIN3 (RX)
		gpioUsart2.Pin = GPIO_PIN_3;
		HAL_GPIO_Init(GPIOA, &gpioUsart2);
		// 3. Enable IRQ/Set Priority
		HAL_NVIC_EnableIRQ(USART2_IRQn);
		HAL_NVIC_SetPriority(USART2_IRQn, 15, 0);
	}
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
	__HAL_RCC_TIM6_CLK_ENABLE();

	HAL_NVIC_SetPriority(TIM6_DAC_IRQn, 15, 0);
	HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn);
}
