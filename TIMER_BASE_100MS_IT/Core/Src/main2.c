/*
 * main2.c
 *
 *  Created on: Sep 17, 2020
 *      Author: s0lid
 */

#include "stm32f4xx_hal.h"
#include "main.h"

void SystemClockConfig(void);
void GPIO_Init(void);
void Timer6_Init(void);
void Error_Handler(void);

TIM_HandleTypeDef htimer6;



int main(void)
{
	HAL_Init();
	SystemClockConfig();
	GPIO_Init();
	Timer6_Init();

	if(HAL_TIM_Base_Start_IT(&htimer6) != HAL_OK)
	{
		Error_Handler();
	}

	/*while(1)
	{
		if((TIM6->SR & TIM_SR_UIF) == TIM_SR_UIF)
		{
			HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_13);
			TIM6->SR &= (1 << TIM_SR_UIF);
		}
	}*/

	while(1);

	return 0;
}


void SystemClockConfig(void)
{
	uint32_t fLatency = FLASH_LATENCY_1;

	RCC_OscInitTypeDef osc;

	osc.HSEState = RCC_HSE_ON;
	osc.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	osc.PLL.PLLState = RCC_PLL_ON;
	osc.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	osc.PLL.PLLM = 8;
	osc.PLL.PLLN = 100;
	osc.PLL.PLLP = RCC_PLLP_DIV2;

	HAL_RCC_OscConfig(&osc);


	RCC_ClkInitTypeDef rcc_clk;
	rcc_clk.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK |
			RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	rcc_clk.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	rcc_clk.AHBCLKDivider = RCC_SYSCLK_DIV1;
	rcc_clk.APB1CLKDivider = RCC_HCLK_DIV2;
	rcc_clk.APB2CLKDivider = RCC_HCLK_DIV1;

	HAL_RCC_ClockConfig(&rcc_clk, fLatency);

	uint32_t sys_speed = HAL_RCC_GetSysClockFreq();
	uint32_t hclck_speed = HAL_RCC_GetHCLKFreq();
	uint32_t pcl1_speed = HAL_RCC_GetPCLK1Freq();
	uint32_t pcl2_speed = HAL_RCC_GetPCLK2Freq();

}

void GPIO_Init()
{
	GPIO_InitTypeDef led_gpio;(HAL_GetTick() - tickstart)
	led_gpio.Mode = GPIO_MODE_OUTPUT_PP;
	led_gpio.Pin = GPIO_PIN_13;
	led_gpio.Pull = GPIO_NOPULL;
	led_gpio.Speed = GPIO_SPEED_LOW;

	__HAL_RCC_GPIOD_CLK_ENABLE();
	HAL_GPIO_Init(GPIOD, &led_gpio);
}

void Timer6_Init(void)
{
	//prescalar = (target_period *  f_apb1_timer_clk - 1  - target_periods_count) / (1 + target_periods_count)
	htimer6.Instance = TIM6;
	htimer6.Init.CounterMode = TIM_COUNTERMODE_UP;
	//htimer6.Init.Prescaler = 24;
	htimer6.Init.Prescaler = 99;
	//htimer6.Init.Period = 64000 - 1;
	htimer6.Init.Period = 4;

	if(HAL_TIM_Base_Init(&htimer6) != HAL_OK)
	{
		Error_Handler();
	}

}


void Error_Handler(void)
{
	// There is a problem.
	// Bling RED LED.
}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htimer)
{
	HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_13);
}
