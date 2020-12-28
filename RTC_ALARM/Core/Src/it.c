/*
 * it.c
 *
 *  Created on: 02-Jun-2018
 *      Author: kiran
 */

#include <main.h>

extern UART_HandleTypeDef huart2;
extern RTC_HandleTypeDef hrtc;


void SysTick_Handler (void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();

}


void EXTI0_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
}

void RTC_Alarm_IRQHandler(void)
{
	HAL_RTC_AlarmIRQHandler(&hrtc);
}

/*
void USART2_IRQHandler(void)
{
	HAL_UART_IRQHandler(&huart2);
}
*/
