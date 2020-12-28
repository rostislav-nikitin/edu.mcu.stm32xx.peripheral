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
void UART2_Init(void);
void Error_handler(void);

UART_HandleTypeDef uart_2;
char *user_data = "s\n";

#define FALSE	0
#define TRUE	1

uint8_t rxCompleted = FALSE;
uint8_t rxBuffer[64];
uint32_t rxBufferIndex = 0;
uint8_t rxData;

uint8_t convert_to_capital(uint8_t letter)
{
	uint8_t result;
	if(letter >= 'a' && letter <= 'z')
	{
		result = letter - ('a' - 'A');
	}
	else
	{
		result = letter;
	}

	return result;
}



int main(void)
{
	HAL_Init();

	UART2_Init();

	char msg[128];

	sprintf(msg, "SYSCLK: %ld\r\n", HAL_RCC_GetSysClockFreq());
	HAL_UART_Transmit(&uart_2, (uint8_t *)msg, sizeof(msg), HAL_MAX_DELAY);

	sprintf(msg, "HCLCK: %ld\r\n", HAL_RCC_GetHCLKFreq());
	HAL_UART_Transmit(&uart_2, (uint8_t *)msg, sizeof(msg), HAL_MAX_DELAY);

	sprintf(msg, "PCLK1: %ld\r\n", HAL_RCC_GetPCLK1Freq());
	HAL_UART_Transmit(&uart_2, (uint8_t *)msg, sizeof(msg), HAL_MAX_DELAY);

	sprintf(msg, "PCLK2: %ld\r\n", HAL_RCC_GetPCLK2Freq());
	HAL_UART_Transmit(&uart_2, (uint8_t *)msg, sizeof(msg), HAL_MAX_DELAY);

	RCC_OscInitTypeDef osc_init;
	memset(&osc_init, 0, sizeof(osc_init));

	osc_init.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	osc_init.HSEState = RCC_HSE_ON;

	if(HAL_RCC_OscConfig(&osc_init) != HAL_OK)
	{
		Error_handler();
	}

	RCC_ClkInitTypeDef clk_init;
	memset(&clk_init, 0, sizeof(clk_init));

	clk_init.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_HSE;
	clk_init.AHBCLKDivider = RCC_SYSCLK_DIV2;
	clk_init.APB1CLKDivider = RCC_HCLK_DIV2;
	clk_init.APB2CLKDivider = RCC_HCLK_DIV2;

	if(HAL_RCC_ClockConfig(&clk_init, FLASH_ACR_LATENCY_0WS) != HAL_OK)
	{
		Error_handler();
	}

	__HAL_RCC_HSI_DISABLE(); // Saves some current

	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

	UART2_Init();

	sprintf(msg, "SYSCLK: %ld\r\n", HAL_RCC_GetSysClockFreq());
	HAL_UART_Transmit(&uart_2, (uint8_t *)msg, sizeof(msg), HAL_MAX_DELAY);

	sprintf(msg, "HCLCK: %ld\r\n", HAL_RCC_GetHCLKFreq());
	HAL_UART_Transmit(&uart_2, (uint8_t *)msg, sizeof(msg), HAL_MAX_DELAY);

	sprintf(msg, "PCLK1: %ld\r\n", HAL_RCC_GetPCLK1Freq());
	HAL_UART_Transmit(&uart_2, (uint8_t *)msg, sizeof(msg), HAL_MAX_DELAY);

	sprintf(msg, "PCLK2: %ld\r\n", HAL_RCC_GetPCLK2Freq());
	HAL_UART_Transmit(&uart_2, (uint8_t *)msg, sizeof(msg), HAL_MAX_DELAY);


	while(1);

	return 0;
}


void SystemClockConfig(void)
{

}

void UART2_Init(void)
{
	uart_2.Instance = USART2;

	uart_2.Init.BaudRate = 9600;
	uart_2.Init.WordLength = UART_WORDLENGTH_8B;
	uart_2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	uart_2.Init.StopBits = UART_STOPBITS_1;
	uart_2.Init.Parity = UART_PARITY_NONE;
	uart_2.Init.Mode = UART_MODE_TX_RX;

	if(HAL_UART_Init(&uart_2) != HAL_OK)
	{
		Error_handler();
	}
}

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

void Error_handler(void)
{
	// There is a problem.
	// Bling RED LED.
}
