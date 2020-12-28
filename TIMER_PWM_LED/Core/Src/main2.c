/*
 * main2.c
 *
 *  Created on: Sep 17, 2020
 *      Author: s0lid
 */

#include <stdio.h>
#include <string.h>
#include "stm32f4xx_hal.h"
#include "main.h"

void SystemClockConfig(void);
void GPIO_Init(void);
void Error_Handler(void);
void UART2_Init(void);
void Timer2_Init(void);
//void LSE_Configuration(void);

UART_HandleTypeDef uart_2;
TIM_HandleTypeDef htimer2;

volatile uint8_t is_capture_done = FALSE;


int main(void)
{
	HAL_Init();
	SystemClockConfig();
	//Init_Pulse();
	GPIO_Init();
	//LSE_Configuration();
	//UART2_Init();
	//Timer6_Init();
	Timer2_Init();

	if(HAL_TIM_PWM_Start_IT(&htimer2, TIM_CHANNEL_1) != HAL_OK)
	{
		Error_Handler();
	}


	while(1)
	{
	}

	return 0;
}


void SystemClockConfig(void)
{
	uint32_t fLatency = FLASH_LATENCY_1;

	RCC_OscInitTypeDef osc;

	osc.HSEState = RCC_HSE_ON;
	osc.HSIState = RCC_HSI_ON;
	osc.OscillatorType = RCC_OSCILLATORTYPE_HSE | RCC_OSCILLATORTYPE_HSI;
	osc.PLL.PLLState = RCC_PLL_ON;
	osc.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	osc.PLL.PLLM = 8;
	osc.PLL.PLLN = 100;
	osc.PLL.PLLP = RCC_PLLP_DIV2;

	if(HAL_RCC_OscConfig(&osc) != HAL_OK)
	{
		Error_Handler();
	}


	RCC_ClkInitTypeDef rcc_clk;
	rcc_clk.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK |
			RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	rcc_clk.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	rcc_clk.AHBCLKDivider = RCC_SYSCLK_DIV1;
	rcc_clk.APB1CLKDivider = RCC_HCLK_DIV2;
	rcc_clk.APB2CLKDivider = RCC_HCLK_DIV1;

	HAL_RCC_ClockConfig(&rcc_clk, fLatency);

	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / 1000);
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

	uint32_t sys_speed = HAL_RCC_GetSysClockFreq();
	uint32_t hclck_speed = HAL_RCC_GetHCLKFreq();
	uint32_t pcl1_speed = HAL_RCC_GetPCLK1Freq();
	uint32_t pcl2_speed = HAL_RCC_GetPCLK2Freq();

}

void LSE_Configuration(void)
{
	/*RCC_OscInitTypeDef osc;

	osc.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	osc.HSIState = RCC_HSI_ON;

	if(HAL_RCC_OscConfig(&osc) != HAL_OK)
	{
		Error_Handler();
	}*/

	HAL_RCC_MCOConfig(RCC_MCO1, RCC_MCO1SOURCE_HSI, RCC_MCODIV_4);
}

void UART2_Init(void)
{
	uart_2.Instance = USART2;

	uart_2.Init.BaudRate = 9600;
	uart_2.Init.WordLength = UART_WORDLENGTH_8B;
	uart_2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	uart_2.Init.StopBits = UART_STOPBITS_1;
	uart_2.Init.Parity = UART_PARITY_NONE;
	uart_2.Init.Mode = UART_MODE_TX;

	if(HAL_UART_Init(&uart_2) != HAL_OK)
	{
		Error_Handler();
	}
}



void Timer2_Init(void)
{
	htimer2.Instance = TIM2;
	htimer2.Init.Period = 10000 - 1;
	htimer2.Init.Prescaler = 4;

	if(HAL_TIM_PWM_Init(&htimer2) != HAL_OK)
	{
		Error_Handler();
	}

	TIM_OC_InitTypeDef tim_oc_init;
	tim_oc_init.OCMode = TIM_OCMODE_PWM1;
	tim_oc_init.OCNPolarity = TIM_OCNPOLARITY_HIGH;

	//tim_oc_init.Pulse = (0.25 * htimer2.Init.Period);
	tim_oc_init.Pulse = 1;
	if(HAL_TIM_PWM_ConfigChannel(&htimer2, &tim_oc_init, TIM_CHANNEL_1) != HAL_OK)
	{
		Error_Handler();
	}
}

void GPIO_Init()
{
	GPIO_InitTypeDef led_gpio;
	led_gpio.Mode = GPIO_MODE_OUTPUT_PP;
	led_gpio.Pin = GPIO_PIN_13;
	led_gpio.Pull = GPIO_NOPULL;
	led_gpio.Speed = GPIO_SPEED_LOW;

	__HAL_RCC_GPIOD_CLK_ENABLE();
	HAL_GPIO_Init(GPIOD, &led_gpio);
}


void Error_Handler(void)
{
	// There is a problem.
	// Bling RED LED.
}
