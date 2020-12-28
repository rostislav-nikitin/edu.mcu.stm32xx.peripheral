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
void RTC_Init(void);
void RTC_CalendarCofig(void);
void print_datatime(void);

void GPIO_InitButton(void);
void GPIO_InitLed(void);
void RTC_AlarmConfig(void );

UART_HandleTypeDef huart2;
RTC_HandleTypeDef hrtc;

extern uint8_t some_data[];


int main(void)
{
	char msg[50];

	HAL_Init();
	SystemClock_Config_HSE(SYS_CLOCK_FREQ_50_MHZ);
	GPIO_Init();

	//HAL_SuspendTick();
	UART2_Init();

	//RTC_Init();

	//__HAL_RCC_BKPSRAM_CLK_ENABLE();
	// 2. Enable Write access to the backup domain
	//__HAL_RCC_PWR_CLK_ENABLE();

	//RTC_CalendarCofig();



	if(__HAL_PWR_GET_FLAG(PWR_FLAG_SB) != RESET)
	{
		__HAL_PWR_CLEAR_FLAG(PWR_FLAG_SB);
		__HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);
		__HAL_RTC_ALARM_CLEAR_FLAG(&hrtc, RTC_FLAG_ALRAF);

		printmsg("Woke up from the StandBy mode.\r\n");
		//print_datatime();
	}


	/*HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN1);
	HAL_PWR_EnterSTANDBYMode();*/


	while(1);

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
						 GPIO_PIN_11 | GPIO_PIN_12;
						// | GPIO_PIN_13 | GPIO_PIN_14 |\
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
	GPIO_InitButton();
	GPIO_InitLed();
}

void GPIO_InitButton(void)
{
    __HAL_RCC_GPIOA_CLK_ENABLE();
    //__HAL_RCC_GPIOA_CLK_SLEEP_DISABLE();

	GPIO_InitTypeDef buttongpio;

	buttongpio.Pin = GPIO_PIN_0;
	buttongpio.Mode = GPIO_MODE_IT_FALLING;
	buttongpio.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA,&buttongpio);

	HAL_NVIC_EnableIRQ(EXTI0_IRQn);
}

void GPIO_InitLed(void)
{

	__HAL_RCC_GPIOD_CLK_ENABLE();

    GPIO_InitTypeDef gpio_led;
    gpio_led.Mode = GPIO_MODE_OUTPUT_PP;
    gpio_led.Pin = GPIO_PIN_12;
    gpio_led.Pull = GPIO_NOPULL;
    gpio_led.Speed = GPIO_SPEED_LOW;


    HAL_GPIO_Init(GPIOD, &gpio_led);
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

void RTC_Init(void)
{
	hrtc.Instance = RTC;
	hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
	hrtc.Init.AsynchPrediv = 0x7f;
	hrtc.Init.SynchPrediv = 0x7a11;
	hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
	hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_LOW;
	hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;

	if(HAL_RTC_Init(&hrtc) != HAL_OK)
	{
		Error_handler();
	}
}

void RTC_CalendarCofig(void)
{
	RTC_DateTypeDef rtc_date_init;
	memset(&rtc_date_init, 0, sizeof(rtc_date_init));

	rtc_date_init.Date = 9U;
	rtc_date_init.Month = RTC_MONTH_OCTOBER;
	rtc_date_init.Year = 20U;
	rtc_date_init.WeekDay = RTC_WEEKDAY_FRIDAY;

	if(HAL_RTC_SetDate(&hrtc, &rtc_date_init, RTC_FORMAT_BIN) != HAL_OK)
	{
		Error_handler();
	}

	RTC_TimeTypeDef rtc_time_init;
	memset(&rtc_time_init, 0, sizeof(rtc_time_init));

	//Ex2
	//rtc_time_init.Hours = 12U;
	//rtc_time_init.Minutes = 00U;
	//rtc_time_init.Seconds = 0U;
	//rtc_time_init.TimeFormat = RTC_HOURFORMAT12_PM;
	//rtc_time_init.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;

	//Ex3
	rtc_time_init.Hours = 23U;
	rtc_time_init.Minutes = 15U;
	rtc_time_init.Seconds = 20U;
	rtc_time_init.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;

	if(HAL_RTC_SetTime(&hrtc, &rtc_time_init, RTC_FORMAT_BIN) != HAL_OK)
	{
		Error_handler();
	}
}

char* get_week_day(uint8_t number)
{
	char *week_days[] = {"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};
	char *result = week_days[number - 1];

	return result;
}

uint8_t rtc_init = DISABLE;

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(rtc_init == DISABLE)
	{
		rtc_init = ENABLE;
		//print_datatime();
		RTC_Init();
		RTC_CalendarCofig();
		RTC_AlarmConfig();


		//__HAL_RCC_BKPSRAM_CLK_ENABLE();
		// 2. Enable Write access to the backup domain
		//__HAL_RCC_PWR_CLK_ENABLE();

		//__HAL_PWR_CLEAR_FLAG(PWR_FLAG_SB);
		__HAL_RTC_ALARM_CLEAR_FLAG(&hrtc, RTC_FLAG_ALRAF);
		__HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);

		HAL_DBGMCU_EnableDBGStopMode();
		HAL_SuspendTick();
		HAL_PWR_EnterSTOPMode(PWR_MAINREGULATOR_ON, PWR_STOPENTRY_WFI);
		HAL_ResumeTick();

	}

	print_datatime();
}

void RTC_AlarmConfig(void)
{
	RTC_AlarmTypeDef rtc_alarm_init;
	memset(&rtc_alarm_init, 0, sizeof(rtc_alarm_init));

	HAL_RTC_DeactivateAlarm(&hrtc, RTC_ALARM_A);

	rtc_alarm_init.Alarm = RTC_ALARM_A;
	// Ex1
	//rtc_alarm_init.AlarmTime.Minutes = 33U;
	//rtc_alarm_init.AlarmTime.Seconds = 10U;
	//rtc_alarm_init.AlarmMask = (RTC_ALARMMASK_HOURS | RTC_ALARMMASK_DATEWEEKDAY);
	// Ex2
	//rtc_alarm_init.AlarmTime.Hours = 12U;
	//rtc_alarm_init.AlarmTime.Minutes = 00U;
	//rtc_alarm_init.AlarmTime.Seconds = 10U;
	//rtc_alarm_init.AlarmTime.TimeFormat = RTC_HOURFORMAT12_PM;
	//rtc_alarm_init.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_NONE;
	//rtc_alarm_init.AlarmMask = RTC_ALARMMASK_DATEWEEKDAY;
	// Ex3
	//rtc_alarm_init.AlarmDateWeekDay = RTC_WEEKDAY_FRIDAY;
	//rtc_alarm_init.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_WEEKDAY;
	rtc_alarm_init.AlarmTime.Hours = 23U;
	rtc_alarm_init.AlarmTime.Minutes = 15U;
	rtc_alarm_init.AlarmTime.Seconds = 30U;
	rtc_alarm_init.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_NONE;
	rtc_alarm_init.AlarmMask = RTC_ALARMMASK_DATEWEEKDAY;

	//rtc_alarm_init.AlarmTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
	rtc_alarm_init.AlarmTime.StoreOperation = RTC_STOREOPERATION_SET;


	HAL_RTC_SetAlarm_IT(&hrtc, &rtc_alarm_init, RTC_FORMAT_BIN);
	//HAL_RTCEx_SetWakeUpTimer_IT(&hrtc, 20U, RTC_WAKEUPCLOCK_RTCCLK_DIV2);

}

void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc)
{
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_SET);
}


void print_datatime(void)
{
	RTC_TimeTypeDef rtc_time_init;
	RTC_DateTypeDef rtc_date_init;

	HAL_RTC_GetTime(&hrtc, &rtc_time_init, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &rtc_date_init, RTC_FORMAT_BIN);

	printmsg("The hour is %i, minute is %i, second is %i\r\n", rtc_time_init.Hours, rtc_time_init.Minutes, rtc_time_init.Seconds);
	printmsg("The year is %i, month is %i, day is %i, weekday is %s\r\n", rtc_date_init.Year, rtc_date_init.Month, rtc_date_init.Date, get_week_day(rtc_date_init.WeekDay));
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{

}
void Error_handler(void)
{
	while(1);
}

