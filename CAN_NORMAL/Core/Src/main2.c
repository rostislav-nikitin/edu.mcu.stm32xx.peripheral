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
void CAN1_Init(void);
void CAN1_FilterConfig(void);
void CAN1_Tx(void);
void CAN1_Rx(void);
void SendMessageTransmitted(int mailboxNumber);
void SwitchOffLeds(void);
void SwitchLeds(uint8_t led_idx);
void SendResponse(uint32_t stdId);
//void LSE_Configuration(void);

UART_HandleTypeDef uart_2;
TIM_HandleTypeDef htimer6;
CAN_HandleTypeDef hcan1;

volatile uint8_t send_data = 0;

volatile uint8_t is_capture_done = FALSE;


int main(void)
{
	HAL_Init();
	SystemClockConfig();
	Button_Init();
	GPIO_Init();
	//LSE_Configuration();
	UART2_Init();

	TIMER6_Init();
	if(HAL_TIM_Base_Start_IT(&htimer6) != HAL_OK)
	{
		Error_Handler();
	}

	CAN1_Init();
	CAN1_FilterConfig();
	HAL_CAN_ActivateNotification(&hcan1,
			CAN_IT_TX_MAILBOX_EMPTY | CAN_IT_RX_FIFO0_MSG_PENDING | CAN_IT_BUSOFF | CAN_IT_ERROR);

	if(HAL_CAN_Start(&hcan1) != HAL_OK)
	{
		Error_Handler();
	}

	//CAN1_Tx();

	while(1);

	return 0;
}

void TIMER6_Init(void)
{
	htimer6.Instance = TIM6;
	htimer6.Init.Prescaler = 9999;
	htimer6.Init.Period = 8400-1;
	htimer6.Init.CounterMode = TIM_COUNTERMODE_UP;

	HAL_TIM_Base_Init(&htimer6);
}

void SystemClockConfig(void)
{
	uint32_t fLatency = FLASH_LATENCY_5;// FLASH_LATENCY_1;

	RCC_OscInitTypeDef osc;

	osc.HSEState = RCC_HSE_ON;
	osc.HSIState = RCC_HSI_ON;
	osc.OscillatorType = RCC_OSCILLATORTYPE_HSE | RCC_OSCILLATORTYPE_HSI;
	osc.PLL.PLLState = RCC_PLL_ON;
	osc.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	osc.PLL.PLLM = 8;
	osc.PLL.PLLN = 336;
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
	rcc_clk.APB1CLKDivider = RCC_HCLK_DIV4;
	rcc_clk.APB2CLKDivider = RCC_HCLK_DIV2;

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

void Button_Init()
{
	GPIO_InitTypeDef led_gpio;
	led_gpio.Mode = GPIO_MODE_IT_RISING;
	led_gpio.Pin = GPIO_PIN_0;
	led_gpio.Pull = GPIO_PULLDOWN;
	led_gpio.Speed = GPIO_SPEED_LOW;

	__HAL_RCC_GPIOA_CLK_ENABLE();
	__NVIC_SetPriority(EXTI0_IRQn, 15);
	__NVIC_EnableIRQ(EXTI0_IRQn);
	HAL_GPIO_Init(GPIOA, &led_gpio);
}

#define DEBOUNCE_DELAY	100
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	uint8_t ticks = 0;
	while(++ticks < DEBOUNCE_DELAY);

	if(GPIO_Pin == GPIO_PIN_0)
	{
		send_data = !send_data;
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

	led_gpio.Pin = GPIO_PIN_13;
	HAL_GPIO_Init(GPIOD, &led_gpio);

	led_gpio.Pin = GPIO_PIN_14;
	HAL_GPIO_Init(GPIOD, &led_gpio);

	led_gpio.Pin = GPIO_PIN_15;
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
	hcan1.Init.TimeTriggeredMode = DISABLE;
	hcan1.Init.TransmitFifoPriority = DISABLE;

	// Setup timings
	hcan1.Init.Prescaler = 3;//5;
	hcan1.Init.SyncJumpWidth = CAN_SJW_1TQ;
	hcan1.Init.TimeSeg1 = CAN_BS1_11TQ; //CAN_BS1_8TQ;
	hcan1.Init.TimeSeg2 = CAN_BS2_2TQ;//CAN_BS2_1TQ;

	if(HAL_CAN_Init(&hcan1) != HAL_OK)
	{
		Error_Handler();
	}
}

void CAN1_FilterConfig(void)
{
	CAN_FilterTypeDef can1_filter_init;

	can1_filter_init.FilterActivation = ENABLE;
	can1_filter_init.FilterMode = CAN_FILTERMODE_IDMASK;
	can1_filter_init.FilterScale = CAN_FILTERSCALE_32BIT;
	can1_filter_init.FilterBank = 0;
	can1_filter_init.FilterFIFOAssignment = CAN_RX_FIFO0;
	can1_filter_init.FilterIdHigh = 0x0180; //0x0000;
	can1_filter_init.FilterIdLow = 0x0000;
	can1_filter_init.FilterMaskIdHigh = 0x01c0;
	can1_filter_init.FilterMaskIdLow = 0x0000;

	if(HAL_CAN_ConfigFilter(&hcan1, &can1_filter_init) != HAL_OK)
	{
		Error_Handler();
	}
}

#define IDX_MIN	0
#define IDX_MAX	4

uint8_t idx = IDX_MIN;
#define TX_BUFFER_SIZE	1

#define CAN_ADDRESS_N1	0x651U
#define CAN_ADDRESS_N2	0x65DU


void CAN1_Tx(void)
{
	//char msg[128];
	//sprintf(msg, "Led %i.\r\n", ++idx);

	//uint16_t address = 0x65D;
	uint32_t p_tx_mailbox;
	uint8_t tx_buffer[TX_BUFFER_SIZE] = { ++idx };

	CAN_TxHeaderTypeDef tx_header;
	tx_header.DLC = TX_BUFFER_SIZE; //strlen(msg);
	tx_header.StdId = CAN_ADDRESS_N2;
	tx_header.IDE = CAN_ID_STD;
	tx_header.RTR = CAN_RTR_DATA;

	if(HAL_CAN_AddTxMessage(&hcan1, &tx_header, tx_buffer, &p_tx_mailbox) != HAL_OK)
	{
		Error_Handler();
	}

	if(idx == IDX_MAX)
	{
		idx = IDX_MIN;
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(send_data)
	{
		CAN1_Tx();
		CheckTimeAndSendRequest();
	}
}

#define SECONDS_START	1
#define SECONDS_END		4

uint8_t seconds_counter = SECONDS_START;

void CheckTimeAndSendRequest()
{
	if(seconds_counter == SECONDS_END)
	{
		SendRequest();
		seconds_counter = SECONDS_START;
	}
	else
	{
		seconds_counter++;
	}
}

void SendRequest()
{
	CAN_TxHeaderTypeDef tx_header;
	uint8_t tx_buffer[0];
	uint32_t pTxMailbox;

	tx_header.StdId = CAN_ADDRESS_N1;
	tx_header.RTR = CAN_RTR_REMOTE;
	tx_header.IDE = CAN_ID_STD;
	tx_header.DLC = sizeof(tx_buffer);

	if(HAL_CAN_AddTxMessage(&hcan1, &tx_header, tx_buffer, &pTxMailbox) != HAL_OK)
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

	uint8_t recived[8];
	memset(&recived,0,8);

	//while(!HAL_CAN_GetRxFifoFillLevel(&hcan1, CAN_RX_FIFO0));

	if(HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO0, &rx_header, recived) != HAL_OK)
	{
		Error_Handler();
	}

	char msg[128];
	if(rx_header.StdId == CAN_ADDRESS_N2 && rx_header.RTR == CAN_RTR_DATA)
	{
		// From N1 to N2
		uint8_t led_idx = recived[0];
		sprintf(msg, "[N2]\tMessage received. Led %i\r\n", led_idx);

		SwitchLeds(led_idx);
	}
	else if(rx_header.StdId == CAN_ADDRESS_N1 && rx_header.RTR == CAN_RTR_REMOTE)
	{
		// Remote from N1 to N2
		sprintf(msg, "[N2]\tData request received.");
		SendResponse(rx_header.StdId);
	}
	else if(rx_header.StdId == CAN_ADDRESS_N1 && rx_header.RTR == CAN_RTR_DATA)
	{
		sprintf(msg, "[N1]\tData reply received.");
		// Reply By N2 to N1
	}


	HAL_UART_Transmit(&uart_2, (uint8_t *)msg, strlen(msg), HAL_MAX_DELAY);

}

void SwitchLeds(uint8_t led_idx)
{
	switch(led_idx)
	{
		case 1:
			SwitchOffLeds();
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_SET);
			break;
		case 2:
			SwitchOffLeds();
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_SET);
			break;
		case 3:
			SwitchOffLeds();
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET);
			break;
		case 4:
			SwitchOffLeds();
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_SET);
			break;
	}
}

void SwitchOffLeds(void)
{
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_RESET);
}

void SendResponse(uint32_t stdId)
{
	CAN_TxHeaderTypeDef tx_header;

	uint8_t tx_data[2] = {0x51, 0x61};
	uint32_t pTxMailbox;

	tx_header.StdId = stdId;
	tx_header.RTR = CAN_RTR_DATA;
	tx_header.IDE = CAN_ID_STD;
	tx_header.DLC = sizeof(tx_data);

	HAL_CAN_AddTxMessage(&hcan1, &tx_header, tx_data, &pTxMailbox);
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

