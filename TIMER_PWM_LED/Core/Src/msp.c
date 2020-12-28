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

void HAL_MSPInit(void)
{
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);
	HAL_NVIC_SetPriority(MemoryManagement_IRQn, 0, 0);
	HAL_NVIC_SetPriority(UsageFault_IRQn, 0, 0);
	HAL_NVIC_SetPriority(BusFault_IRQn, 0, 0);

	SCB->SHCSR |= 0x7 << 16;
}

/*void HAL_TIM_IC_MspInit(TIM_HandleTypeDef *htim)
{
	GPIO_InitTypeDef tim2ch1_gpio;

	// CLK
	__HAL_RCC_TIM2_CLK_ENABLE();

	// GPIO as timer2 channel
	__HAL_RCC_GPIOA_CLK_ENABLE();

	tim2ch1_gpio.Pin = GPIO_PIN_0;
	tim2ch1_gpio.Mode = GPIO_MODE_AF_PP;
	tim2ch1_gpio.Alternate = GPIO_AF1_TIM2;
	HAL_GPIO_Init(GPIOA, &tim2ch1_gpio);

	HAL_NVIC_EnableIRQ(TIM2_IRQn);
	HAL_NVIC_SetPriority(TIM2_IRQn, 15, 0);
}*/

/*void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM6)
	{
		// 1. Enable clock for TIM6 peripheral
		__HAL_RCC_TIM6_CLK_ENABLE();
		// 2. Enable IRQ handler
		HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn);
		// 3. Setup priority
		HAL_NVIC_SetPriority(TIM6_DAC_IRQn, 15, 0);
	}

}*/

void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
	GPIO_InitTypeDef tim2OC_ch_gpios;

	// 1. Enable Timer & Channels GPIOS clocks
	__HAL_RCC_TIM2_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();

	// 2. Configure GPIOs
	// PA0 --> CH1
	tim2OC_ch_gpios.Pin = GPIO_PIN_0;
	tim2OC_ch_gpios.Alternate = GPIO_AF1_TIM2;
	tim2OC_ch_gpios.Mode = GPIO_MODE_AF_PP;
	tim2OC_ch_gpios.Pull = GPIO_NOPULL;
	tim2OC_ch_gpios.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	//tim2OC_ch_gpios.Alternate =
	HAL_GPIO_Init(GPIOA, &tim2OC_ch_gpios);

	// 3.
	HAL_NVIC_SetPriority(TIM2_IRQn, 15, 0);
	HAL_NVIC_EnableIRQ(TIM2_IRQn);
}
