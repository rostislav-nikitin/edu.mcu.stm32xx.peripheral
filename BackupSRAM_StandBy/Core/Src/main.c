/*
 * main.c
 *
 *  Created on: 02-Jun-2018
 *      Author: kiran
 */

#include "main.h"
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>
#include "stm32f4xx_hal.h"


void printmsg(char *format, ...);
void GPIO_Init(void);
void Error_handler(void);
void UART2_Init(void);
void SystemClock_Config_HSE(uint8_t clock_freq);
void GPIO_AnalogConfig(void);

UART_HandleTypeDef huart2;

extern uint8_t some_data[];


int main(void)
{
	char msg[50];

	HAL_Init();
	GPIO_Init();
	SystemClock_Config_HSE(SYS_CLOCK_FREQ_50_MHZ);
	//HAL_SuspendTick();
	UART2_Init();



	//GPIO_AnalogConfig();

	//HAL_DBGMCU_EnableDBGStopMode();
	//HAL_DBGMCU_EnableDBGSleepMode();
	//HAL_DBGMCU_EnableDBGStandbyMode();

	// 1. Turn on the clock in the RCC register for backup sram
	__HAL_RCC_BKPSRAM_CLK_ENABLE();
	// 2. Enable Write access to the backup domain
	__HAL_RCC_PWR_CLK_ENABLE();

	uint32_t *p_backup_sram = (uint32_t *)BKPSRAM_BASE;

	if(__HAL_PWR_GET_FLAG(PWR_FLAG_SB) != RESET)
	{
		__HAL_PWR_CLEAR_FLAG(PWR_FLAG_SB);
		__HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);

		printmsg("Woke up from the StandBy mode.\r\n");
		if(*p_backup_sram != 'H')
		{
			printmsg("Backup SRAM data was broken.\r\n");
		}
	}
	else
	{
		HAL_PWR_EnableBkUpAccess();
		// 3. Write data

		char write_buffer[] = "Hello";

		for(uint32_t i = 0; i < strlen(write_buffer); i++)
		{
			*(p_backup_sram + i) = write_buffer[i];
		}
	}

	printmsg("Press the button to StandBy.\r\n");
	//while(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) != GPIO_PIN_SET);

	printmsg("Going to StandBy mode.\r\n");
	HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN1);
	HAL_PWREx_DisableBkUpReg();
	HAL_PWR_EnterSTANDBYMode();

	while(1)
	{
	}
	return 0;
}

void printmsg(char *format, ...)
{
	char str[128];

	va_list args;
	va_start(args, format);
	vsprintf(str, format, args);
	HAL_UART_Transmit(&huart2, (uint8_t *)str, strlen(str), HAL_MAX_DELAY);
	va_end(args);
}



/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config_HSE(uint8_t clock_freq)
{
	RCC_OscInitTypeDef Osc_Init;
	RCC_ClkInitTypeDef Clock_Init;
    uint8_t flash_latency=0;

	Osc_Init.OscillatorType = RCC_OSCILLATORTYPE_HSE ;
	Osc_Init.HSEState = RCC_HSE_ON;
	Osc_Init.PLL.PLLState = RCC_PLL_ON;
	Osc_Init.PLL.PLLSource = RCC_PLLSOURCE_HSE;

	switch(clock_freq)
	 {
	  case SYS_CLOCK_FREQ_50_MHZ:
		  Osc_Init.PLL.PLLM = 4;
		  Osc_Init.PLL.PLLN = 50;
		  Osc_Init.PLL.PLLP = RCC_PLLP_DIV2;
		  Osc_Init.PLL.PLLQ = 2;
		  //Osc_Init.PLL.PLLR = 2;
		  Clock_Init.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
	                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
		  Clock_Init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
		  Clock_Init.AHBCLKDivider = RCC_SYSCLK_DIV1;
		  Clock_Init.APB1CLKDivider = RCC_HCLK_DIV2;
		  Clock_Init.APB2CLKDivider = RCC_HCLK_DIV1;
          flash_latency = 1;
	     break;

	  case SYS_CLOCK_FREQ_84_MHZ:
		  Osc_Init.PLL.PLLM = 4;
		  Osc_Init.PLL.PLLN = 84;
		  Osc_Init.PLL.PLLP = RCC_PLLP_DIV2;
		  Osc_Init.PLL.PLLQ = 2;
		  //Osc_Init.PLL.PLLR = 2;
		  Clock_Init.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
	                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
		  Clock_Init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
		  Clock_Init.AHBCLKDivider = RCC_SYSCLK_DIV1;
		  Clock_Init.APB1CLKDivider = RCC_HCLK_DIV2;
		  Clock_Init.APB2CLKDivider = RCC_HCLK_DIV1;
          flash_latency = 2;
	     break;

	  case SYS_CLOCK_FREQ_120_MHZ:
		  Osc_Init.PLL.PLLM = 4;
		  Osc_Init.PLL.PLLN = 120;
		  Osc_Init.PLL.PLLP = RCC_PLLP_DIV2;
		  Osc_Init.PLL.PLLQ = 2;
		  //Osc_Init.PLL.PLLR = 2;
		  Clock_Init.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
	                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
		  Clock_Init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
		  Clock_Init.AHBCLKDivider = RCC_SYSCLK_DIV1;
		  Clock_Init.APB1CLKDivider = RCC_HCLK_DIV4;
		  Clock_Init.APB2CLKDivider = RCC_HCLK_DIV2;
          flash_latency = 3;
	     break;

	  default:
	   return ;
	 }

		if (HAL_RCC_OscConfig(&Osc_Init) != HAL_OK)
	{
			Error_handler();
	}



	if (HAL_RCC_ClockConfig(&Clock_Init, flash_latency) != HAL_OK)
	{
		Error_handler();
	}


	/*Configure the systick timer interrupt frequency (for every 1 ms) */
	uint32_t hclk_freq = HAL_RCC_GetHCLKFreq();
	HAL_SYSTICK_Config(hclk_freq/1000);

	/**Configure the Systick
	*/
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

	/* SysTick_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);



 }

void GPIO_AnalogConfig(void)
{
	GPIO_InitTypeDef GpioA,GpioC;

	uint32_t gpio_pins = GPIO_PIN_1 |GPIO_PIN_4 | \
						 GPIO_PIN_5 | GPIO_PIN_6 |GPIO_PIN_7 |\
						 GPIO_PIN_8 | GPIO_PIN_9 |GPIO_PIN_10 |\
						 GPIO_PIN_11 | GPIO_PIN_12;// | GPIO_PIN_13 | GPIO_PIN_14 |\
						 //GPIO_PIN_15;


	GpioA.Pin = gpio_pins;
	GpioA.Mode = GPIO_MODE_ANALOG;
	HAL_GPIO_Init(GPIOA,&GpioA);

	gpio_pins = GPIO_PIN_0 | GPIO_PIN_1 |GPIO_PIN_2|  \
			    GPIO_PIN_3 | GPIO_PIN_4 |  GPIO_PIN_5 | \
			    GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | \
			    GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | \
			    GPIO_PIN_12 | GPIO_PIN_14 | GPIO_PIN_15;

	/*GpioC.Pin = gpio_pins;
	GpioC.Mode = GPIO_MODE_ANALOG;
	HAL_GPIO_Init(GPIOC,&GpioC);*/


}


void GPIO_Init(void)
{
    __HAL_RCC_GPIOA_CLK_ENABLE();
    //__HAL_RCC_GPIOA_CLK_SLEEP_DISABLE();

	GPIO_InitTypeDef buttongpio;

	buttongpio.Pin = GPIO_PIN_0;
	buttongpio.Mode = GPIO_MODE_INPUT;
	buttongpio.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA,&buttongpio);

	//HAL_NVIC_EnableIRQ(EXTI0_IRQn);
}



void UART2_Init(void)
{


	huart2.Instance = USART2;
	huart2.Init.BaudRate =9600;
	huart2.Init.WordLength = UART_WORDLENGTH_8B;
	huart2.Init.StopBits = UART_STOPBITS_1;
	huart2.Init.Parity = UART_PARITY_NONE;
	huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart2.Init.Mode = UART_MODE_TX;


	if ( HAL_UART_Init(&huart2) != HAL_OK )
	{
		//There is a problem
		Error_handler();
	}

}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if ( HAL_UART_Transmit(&huart2,(uint8_t*)some_data,(uint16_t)strlen((char*)some_data),HAL_MAX_DELAY) != HAL_OK)
	{
		Error_handler();
	}
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{

}
void Error_handler(void)
{
	while(1);
}

