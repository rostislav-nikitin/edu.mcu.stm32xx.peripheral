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
void Init_Pulse(void);
uint32_t Calc_Pulse(uint32_t ch_freq);
void GPIO_Init(void);
void Error_Handler(void);
void UART2_Init(void);
void Timer2_Init(void);
//void Timer6_Init(void);
//void LSE_Configuration(void);

UART_HandleTypeDef uart_2;
TIM_HandleTypeDef htimer2;
//TIM_HandleTypeDef htimer6;

uint32_t input_captures[2] = {0};
uint8_t count = 1;

const int timer2_prescaler = 1;

const uint32_t ch1_freq = 500;
const uint32_t ch2_freq = 1000;
const uint32_t ch3_freq = 2000;
const uint32_t ch4_freq = 4000;

uint32_t ch1_pulse = 500;
uint32_t ch2_pulse = 1000;
uint32_t ch3_pulse = 2000;
uint32_t ch4_pulse = 4000;


volatile uint8_t is_capture_done = FALSE;


int main(void)
{
	uint32_t capture_difference = 0;

	HAL_Init();
	SystemClockConfig();
	Init_Pulse();
	GPIO_Init();
	//LSE_Configuration();
	//UART2_Init();
	//Timer6_Init();
	Timer2_Init();

	/*if(HAL_TIM_Base_Start_IT(&htimer6) != HAL_OK)
	{
		Error_Handler();
	}*/

	if(HAL_TIM_OC_Start_IT(&htimer2, TIM_CHANNEL_1) != HAL_OK)
	{
		Error_Handler();
	}

	if(HAL_TIM_OC_Start_IT(&htimer2, TIM_CHANNEL_2) != HAL_OK)
	{
		Error_Handler();
	}

	if(HAL_TIM_OC_Start_IT(&htimer2, TIM_CHANNEL_3) != HAL_OK)
	{
		Error_Handler();
	}

	if(HAL_TIM_OC_Start_IT(&htimer2, TIM_CHANNEL_4) != HAL_OK)
	{
		Error_Handler();
	}




	while(1)
	{
		if(is_capture_done)
		{
			if(input_captures[1] > input_captures[0])
			{
				capture_difference = input_captures[1] - input_captures[0];
			}
			else
			{
				capture_difference = (0xffffffff - input_captures[0]) + input_captures[1];
			}

			double timer2_cnt_freq = (HAL_RCC_GetPCLK1Freq() * 2) / (htimer2.Init.Prescaler + 1);
			double timer2_cnt_res = 1 / timer2_cnt_freq;

			double user_signal_time_period = capture_difference * timer2_cnt_res;
			double user_signal_freq = 1 / user_signal_time_period;

			char usr_msg[128];

			sprintf(usr_msg, "External signal frequency is: %f Hz.\r\n", user_signal_freq);

			HAL_UART_Transmit(&uart_2, (uint8_t *)usr_msg, strlen(usr_msg), HAL_MAX_DELAY);

			is_capture_done = FALSE;
		}
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

void Init_Pulse(void)
{
	ch1_pulse = Calc_Pulse(ch1_freq);
	ch2_pulse = Calc_Pulse(ch2_freq);
	ch3_pulse = Calc_Pulse(ch3_freq);
	ch4_pulse = Calc_Pulse(ch4_freq);
}

uint32_t Calc_Pulse(uint32_t ch_freq)
{
	uint32_t pcl1_freq = HAL_RCC_GetPCLK1Freq() * 2;
	uint32_t result  = ((1.0 / ch_freq) / (1.0 / (pcl1_freq / (1.0 + timer2_prescaler)))) / 2;

	return result;
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
	htimer2.Init.Period = 0xffffffff;
	htimer2.Init.Prescaler = timer2_prescaler;

	if(HAL_TIM_OC_Init(&htimer2) != HAL_OK)
	{
		Error_Handler();
	}

	TIM_OC_InitTypeDef tim2OC_init;

	tim2OC_init.OCMode = TIM_OCMODE_TOGGLE;
	tim2OC_init.OCPolarity = TIM_OCPOLARITY_HIGH;



	tim2OC_init.Pulse = ch1_pulse;
	if(HAL_TIM_OC_ConfigChannel(&htimer2, &tim2OC_init, TIM_CHANNEL_1) != HAL_OK)
	{
		Error_Handler();
	}

	tim2OC_init.Pulse = ch2_pulse;
	if(HAL_TIM_OC_ConfigChannel(&htimer2, &tim2OC_init, TIM_CHANNEL_2) != HAL_OK)
	{
		Error_Handler();
	}

	tim2OC_init.Pulse = ch3_pulse;
	if(HAL_TIM_OC_ConfigChannel(&htimer2, &tim2OC_init, TIM_CHANNEL_3) != HAL_OK)
	{
		Error_Handler();
	}


	tim2OC_init.Pulse = ch4_pulse;
	if(HAL_TIM_OC_ConfigChannel(&htimer2, &tim2OC_init, TIM_CHANNEL_4) != HAL_OK)
	{
		Error_Handler();
	}




}

/*void Timer6_Init(void)
{
	//prescalar = (target_period *  f_apb1_timer_clk - 1  - target_periods_count) / (1 + target_periods_count)
	htimer6.Instance = TIM6;
	htimer6.Init.CounterMode = TIM_COUNTERMODE_UP;
	//htimer6.Init.Prescaler = 24;
	htimer6.Init.Prescaler = 9;
	//htimer6.Init.Period = 64000 - 1;
	htimer6.Init.Period = 50-1;

	if(HAL_TIM_Base_Init(&htimer6) != HAL_OK)
	{
		Error_Handler();
	}

}*/


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

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htimer)
{
	if(!is_capture_done)
	{
		if(count == 1)
		{
			uint32_t value = __HAL_TIM_GET_COMPARE(htimer, TIM_CHANNEL_1);
			input_captures[0] = value;
			count++;
		}
		else
		{
			uint32_t value = __HAL_TIM_GET_COMPARE(htimer, TIM_CHANNEL_1);
			input_captures[1] = value;
			is_capture_done = TRUE;
			count = 1;
		}
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htimer)
{
	HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_13);
}
