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
void TIMER6_Init(void);
void Button_Init(void);

//void LSE_Configuration(void);

void SetupGPIOsForPowerSave(void);
void SetupGPIOAForPowerSave(void);
void SetupGPIODForPowerSave(void);


UART_HandleTypeDef uart_2;
TIM_HandleTypeDef htimer6;

void SetupGPIOsForPowerSave(void)
{
	SetupGPIOAForPowerSave();
	SetupGPIODForPowerSave();
}

void SetupGPIOAForPowerSave(void)
{
	__HAL_RCC_GPIOA_CLK_ENABLE();

	GPIO_InitTypeDef gpioA;

	gpioA.Pin = GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5 | \
			GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | \
			GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12;

	gpioA.Mode = GPIO_MODE_ANALOG;

	HAL_GPIO_Init(GPIOA, &gpioA);
}

void SetupGPIODForPowerSave(void)
{
	__HAL_RCC_GPIOD_CLK_ENABLE();

	GPIO_InitTypeDef gpioD;

	gpioD.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | \
			GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | \
			GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;

	gpioD.Mode = GPIO_MODE_ANALOG;

	HAL_GPIO_Init(GPIOD, &gpioD);
}

void HAL_MSPInit(void)
{
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);
	HAL_NVIC_SetPriority(MemoryManagement_IRQn, 0, 0);
	HAL_NVIC_SetPriority(UsageFault_IRQn, 0, 0);
	HAL_NVIC_SetPriority(BusFault_IRQn, 0, 0);

	SCB->SHCSR |= 0x7 << 16;

	SetupGPIOsForPowerSave();
}


int main(void)
{
	HAL_Init();
	HAL_MSPInit();
	SystemClockConfig();
	GPIO_Init();
	Button_Init();
	//LSE_Configuration();
	UART2_Init();

	//TIMER6_Init();

	//TIM6->SR = 0;
	HAL_PWR_EnableSleepOnExit();
	//SCB->SCR |= (1 << 1);


	/*if(HAL_TIM_Base_Start_IT(&htimer6) != HAL_OK)
	{
		Error_Handler();
	}*/

	while(1)
	{
		__WFI();
	}

	return 0;
}

void TIMER6_Init(void)
{
	htimer6.Instance = TIM6;
	htimer6.Init.Prescaler = 4999;
	htimer6.Init.Period = 80-1;
	htimer6.Init.CounterMode = TIM_COUNTERMODE_UP;

	HAL_TIM_Base_Init(&htimer6);
}

void SystemClockConfig(void)
{
	//uint32_t fLatency = FLASH_LATENCY_5;
	uint32_t fLatency = FLASH_LATENCY_1;

	RCC_OscInitTypeDef osc;

	osc.HSEState = RCC_HSE_ON;
	//osc.HSIState = RCC_HSI_ON;
	//osc.OscillatorType = RCC_OSCILLATORTYPE_HSE | RCC_OSCILLATORTYPE_HSI;
	//osc.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	osc.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	//osc.PLL.PLLState = RCC_PLL_ON;
	//osc.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	//osc.PLL.PLLM = 8;
	//osc.PLL.PLLN = 336;
	//osc.PLL.PLLP = RCC_PLLP_DIV2;

	if(HAL_RCC_OscConfig(&osc) != HAL_OK)
	{
		Error_Handler();
	}


	RCC_ClkInitTypeDef rcc_clk;
	rcc_clk.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK |
			RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	//rcc_clk.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	//rcc_clk.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
	rcc_clk.SYSCLKSource = RCC_SYSCLKSOURCE_HSE;
	rcc_clk.AHBCLKDivider = RCC_SYSCLK_DIV1;
	rcc_clk.APB1CLKDivider = RCC_HCLK_DIV1;
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

	HAL_RCC4200_MCOConfig(RCC_MCO1, RCC_MCO1SOURCE_HSI, RCC_MCODIV_4);
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

void Button_Init()
{
	GPIO_InitTypeDef led_gpio;
	led_gpio.Mode = GPIO_MODE_IT_FALLING;
	led_gpio.Pin = GPIO_PIN_0;
	led_gpio.Pull = GPIO_PULLDOWN;
	led_gpio.Speed = GPIO_SPEED_LOW;

	__HAL_RCC_GPIOA_CLK_ENABLE();
	__NVIC_SetPriority(EXTI0_IRQn, 15);
	__NVIC_EnableIRQ(EXTI0_IRQn);
	HAL_GPIO_Init(GPIOA, &led_gpio);

	__HAL_RCC_GPIOA_CLK_SLEEP_DISABLE();
}

#define DEBOUNCE_DELAY	10U

uint32_t last_run = 0;

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if((last_run + DEBOUNCE_DELAY < HAL_GetTick()) || (last_run > HAL_GetTick()))
	{
		char *msg = "Button pressed.\r\n";
		HAL_UART_Transmit(&uart_2, (uint8_t *)msg, strlen(msg), HAL_MAX_DELAY);
		last_run = HAL_GetTick();
	}
}


void GPIO_Init()
{
	__HAL_RCC_GPIOD_CLK_ENABLE();

	GPIO_InitTypeDef led_gpio;
	led_gpio.Mode = GPIO_MODE_OUTPUT_PP;
	led_gpio.Pull = GPIO_NOPULL;
	led_gpio.Speed = GPIO_SPEED_LOW;

	led_gpio.Pin = GPIO_PIN_12;
	HAL_GPIO_Init(GPIOD, &led_gpio);
}

int packet_index = 0;
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	__HAL_RCC_GPIOD_CLK_ENABLE();
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_SET);

	char *msg = "SLEEP ON EXIT testing\r\n";
	HAL_UART_Transmit(&uart_2, (uint8_t *)msg, strlen(msg), HAL_MAX_DELAY);


	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_RESET);
	__HAL_RCC_GPIOD_CLK_DISABLE();
}


void Error_Handler(void)
{
	// There is a problem.
	// Bling RED LED.
}

