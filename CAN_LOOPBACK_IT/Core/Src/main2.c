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
void CAN1_Init(void);
void CAN1_FilterConfig(void);
void CAN1_Tx(void);
void CAN1_Rx(void);
void SendMessageTransmitted(int mailboxNumber);
//void LSE_Configuration(void);

UART_HandleTypeDef uart_2;
CAN_HandleTypeDef hcan1;


volatile uint8_t is_capture_done = FALSE;


int main(void)
{
	HAL_Init();
	SystemClockConfig();
	//GPIO_Init();
	//LSE_Configuration();
	UART2_Init();
	CAN1_Init();
	CAN1_FilterConfig();
	HAL_CAN_ActivateNotification(&hcan1,
			CAN_IT_TX_MAILBOX_EMPTY | CAN_IT_RX_FIFO0_MSG_PENDING | CAN_IT_BUSOFF | CAN_IT_ERROR);

	if(HAL_CAN_Start(&hcan1) != HAL_OK)
	{
		Error_Handler();
	}

	CAN1_Tx();

	while(1);

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

void CAN1_Init(void)
{
	hcan1.Instance = CAN1;


	hcan1.Init.Mode = CAN_MODE_LOOPBACK;

	hcan1.Init.AutoBusOff = ENABLE;
	hcan1.Init.AutoRetransmission = ENABLE;
	hcan1.Init.AutoWakeUp = DISABLE;
	hcan1.Init.ReceiveFifoLocked = DISABLE;
	hcan1.Init.TransmitFifoPriority = DISABLE;

	// Setup timings
	hcan1.Init.Prescaler = 5;
	hcan1.Init.SyncJumpWidth = CAN_SJW_1TQ;
	hcan1.Init.TimeSeg1 = CAN_BS1_8TQ;
	hcan1.Init.TimeSeg2 = CAN_BS2_1TQ;

	if(HAL_CAN_Init(&hcan1) != HAL_OK)
	{
		Error_Handler();
	}
}

void CAN1_FilterConfig(void)
{
	CAN_FilterTypeDef can1_filter_init;

	can1_filter_init.FilterActivation = ENABLE;
	can1_filter_init.FilterBank = 0;
	can1_filter_init.FilterFIFOAssignment = CAN_RX_FIFO0;
	can1_filter_init.FilterIdHigh = 0x0000;
	can1_filter_init.FilterIdLow = 0x0000;
	can1_filter_init.FilterMaskIdHigh = 0x0000;
	can1_filter_init.FilterMaskIdLow = 0x0000;
	can1_filter_init.FilterMode = CAN_FILTERMODE_IDMASK;
	can1_filter_init.FilterScale = CAN_FILTERSCALE_32BIT;

	if(HAL_CAN_ConfigFilter(&hcan1, &can1_filter_init) != HAL_OK)
	{
		Error_Handler();
	}
}

void CAN1_Tx(void)
{
	char *data = "Hello";
	//uint16_t address = 0x65D;
	uint32_t p_tx_mailbox;

	CAN_TxHeaderTypeDef tx_header;
	tx_header.DLC = strlen(data);
	tx_header.StdId = 0x65D;;
	tx_header.IDE = CAN_ID_STD;
	tx_header.RTR = CAN_RTR_DATA;


	if(HAL_CAN_AddTxMessage(&hcan1, &tx_header, (uint8_t *)data, &p_tx_mailbox) != HAL_OK)
	{
		Error_Handler();
	}
}

void HAL_CAN_TxMailbox0CompleteCallback(CAN_HandleTypeDef *hcan)
{
	SendMessageTransmitted(0);
}

void HAL_CAN_TxMailbox1CompleteCallback(CAN_HandleTypeDef *hcan)
{
	SendMessageTransmitted(1);
}

void HAL_CAN_TxMailbox2CompleteCallback(CAN_HandleTypeDef *hcan)
{
	SendMessageTransmitted(2);
}

void SendMessageTransmitted(int mailboxNumber)
{
	char msg[128];
	sprintf(msg, "Message transmitted. Used mailbox is %i\r\n", mailboxNumber);
	HAL_UART_Transmit(&uart_2, (uint8_t *)msg, strlen(msg), HAL_MAX_DELAY);
}


void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	CAN_RxHeaderTypeDef rx_header;

	uint8_t recived[16];

	//while(!HAL_CAN_GetRxFifoFillLevel(&hcan1, CAN_RX_FIFO0));

	if(HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO0, &rx_header, recived) != HAL_OK)
	{
		Error_Handler();
	}

	char msg[128];
	sprintf(msg, "Message received %s\r\n", (char *)recived);
	HAL_UART_Transmit(&uart_2, (uint8_t *)msg, strlen(msg), HAL_MAX_DELAY);
}

void HAL_CAN_ErrorCallback(CAN_HandleTypeDef *hcan)
{
	char msg[128];
	sprintf(msg, "Error occurred.\r\n");
	HAL_UART_Transmit(&uart_2, (uint8_t *)msg, strlen(msg), HAL_MAX_DELAY);
}

void Error_Handler(void)
{
	// There is a problem.
	// Bling RED LED.
}

