/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;


UART_HandleTypeDef huart2;


UART_HandleTypeDef p_uart_handle;

I2C_HandleTypeDef p_i2c_handle;

//SPI_HandleTypeDef p_spi_handle;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/*void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_SPI1_Init(void);
static void MX_USART2_UART_Init(void);*/

void HAL_MspInit(void);
void SysClock_Init(void);
void USART_Init(void);
void DebugWrite(const char *message);
void GPIO_Init(void);
//void SPI_Init(void);
void I2C_Init(void);
void OLED_Init(void);

void HAL_MSPInit(void)
{
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);
	HAL_NVIC_SetPriority(MemoryManagement_IRQn, 0, 0);
	HAL_NVIC_SetPriority(UsageFault_IRQn, 0, 0);
	HAL_NVIC_SetPriority(BusFault_IRQn, 0, 0);

	SCB->SHCSR |= 0x7 << 16;
}

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();
  //HAL_MSPInit();
  //SysClock_Init();

  USART_Init();
  GPIO_Init();
  //SPI_Init();
  I2C_Init();

  OLED_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  //SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  //MX_GPIO_Init();
  //MX_I2C1_Init();
  //MX_SPI1_Init();
  //MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

void SysClock_Init(void)
{
	__HAL_RCC_SYSCFG_CLK_ENABLE();
	RCC_OscInitTypeDef p_rcc_osc_init;

	p_rcc_osc_init.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	p_rcc_osc_init.HSEState = RCC_HSE_ON;
	p_rcc_osc_init.PLL.PLLState = RCC_PLL_NONE;

	if(HAL_RCC_OscConfig(&p_rcc_osc_init) != HAL_OK)
	{
		Error_Handler();
	}

	RCC_ClkInitTypeDef p_rcc_clk_init;
	p_rcc_clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_HSE;
	p_rcc_clk_init.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	p_rcc_clk_init.AHBCLKDivider = RCC_SYSCLK_DIV1;
	p_rcc_clk_init.APB1CLKDivider = RCC_HCLK_DIV1;

	if(HAL_RCC_ClockConfig(&p_rcc_clk_init, FLASH_ACR_LATENCY_0WS) != HAL_OK)
	{
		Error_Handler();
	}

	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / 1000);
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

	uint32_t hclock = HAL_RCC_GetHCLKFreq();
}

void USART_Init()
{
	p_uart_handle.Instance = USART2;
	p_uart_handle.Init.BaudRate = 9600;
	p_uart_handle.Init.WordLength = UART_WORDLENGTH_8B;
	p_uart_handle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	p_uart_handle.Init.StopBits = UART_STOPBITS_1;
	p_uart_handle.Init.Parity = UART_PARITY_NONE;
	p_uart_handle.Init.Mode = UART_MODE_TX;

	HAL_UART_Init(&p_uart_handle);
}

void GPIO_Init(void)
{
	__HAL_RCC_GPIOA_CLK_ENABLE();

	GPIO_InitTypeDef h_gpio_init;
	h_gpio_init.Mode = GPIO_MODE_OUTPUT_PP;
	h_gpio_init.Pin = GPIO_PIN_1;
	h_gpio_init.Pull = GPIO_NOPULL;
	h_gpio_init.Speed = GPIO_SPEED_LOW;

	HAL_GPIO_Init(GPIOA, &h_gpio_init);

}

void I2C_Init(void)
{
	memset(&p_i2c_handle, 0, sizeof(p_i2c_handle));

	p_i2c_handle.Instance = I2C1;
	p_i2c_handle.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	p_i2c_handle.Init.ClockSpeed = 100000;
	//p_i2c_handle.Mode

	if(HAL_I2C_Init(&p_i2c_handle) != HAL_OK)
	{
		Error_Handler();
	}
}

/*
void SPI_Init(void)
{
	p_spi_handle.Instance = SPI1;

	p_spi_handle.Init.Mode = SPI_MODE_MASTER;
	p_spi_handle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
	p_spi_handle.Init.NSS = SPI_NSS_HARD_OUTPUT;

	if(HAL_SPI_Init(&p_spi_handle) != HAL_OK)
	{
		Error_Handler();
	}
}
*/


// Init sequence
static const uint8_t init1[] = {0x00, 0xAE, 0xD5, 0x80, 0xA8, 0x3F, 0xD3, 0x00, 0x40, 0x8D, 0x14, 0x20, 0x02, 0xA1, 0xC8, 0xDA, 0x12, 0x81, 0xCF, 0xD9, 0xF1, 0xDB, 0x40, 0x2E, 0xA4, 0xA6, 0xAF};
  //ssd1306_commandList(init1, sizeof(init1));
  //ssd1306_command1(HEIGHT - 1);

static const uint8_t init2[] = {0x00, 0xD3, 0x0, 0x0, 0x8D};
//  ssd1306_commandList(init2, sizeof(init2));

//  ssd1306_command1((vccstate == SSD1306_EXTERNALVCC) ? 0x10 : 0x14);

static const uint8_t init3[] = {0x00, 0x20, 0x00, 0xA0 | 0x1, 0xC8 };
//  ssd1306_commandList(init3, sizeof(init3));

static const uint8_t comPins = 0x12;
static const uint8_t comPin_init[] = {0x00, 0x12};


static const uint8_t contrast = 0xCF;
static const uint8_t contrast_init[] = {0x00, 0x9F};

static const uint8_t preCharge = 0xd9;
static const uint8_t preCharge_init[] = {0x00, 0xd9};

static const uint8_t unknown_init[] = {0x00, 0x22};


static const uint8_t init5[] = {0x00, 0xDB, 0x40, 0x2E, 0xA4, 0xA6, 0xAF};

//  uint8_t comPins = 0x02;
//  contrast = 0x8F;

void OLED_Init(void)
{
	DebugWrite("[OLED] RESET OFF\r\n");
	//HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);
	DebugWrite("[OLED] RESET ON\r\n");

	//HAL_Delay(10);

	//HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET);

	uint16_t dev_address = 0x3C << 1;

	//uint8_t p_data[] = {0xff, 0xef, 0xaa, 0x11, 0x55,0xff, 0xef, 0xaa, 0x11, 0x55,0xff, 0xef, 0xaa, 0x11, 0x55,0xff, 0xef, 0xaa, 0x11, 0x55};
	char *message_template = "Data sent: %i\r\n";
	char msg[128];


	if(HAL_I2C_Master_Transmit(&p_i2c_handle, dev_address, init1, sizeof(init1), HAL_MAX_DELAY) != HAL_OK)
	{
		Error_Handler();
	}
/*
	if(HAL_I2C_Master_Transmit(&p_i2c_handle, dev_address, init2, sizeof(init2), HAL_MAX_DELAY) != HAL_OK)
	{
		Error_Handler();
	}

	if(HAL_I2C_Master_Transmit(&p_i2c_handle, dev_address, init3, sizeof(init3), HAL_MAX_DELAY) != HAL_OK)
	{
		Error_Handler();
	}


	if(HAL_I2C_Master_Transmit(&p_i2c_handle, dev_address, comPin_init, sizeof(comPin_init), HAL_MAX_DELAY) != HAL_OK)
	{
		Error_Handler();
	}


	if(HAL_I2C_Master_Transmit(&p_i2c_handle, dev_address, contrast_init, sizeof(contrast_init), HAL_MAX_DELAY) != HAL_OK)
	{
		Error_Handler();
	}

	if(HAL_I2C_Master_Transmit(&p_i2c_handle, dev_address, preCharge_init, sizeof(preCharge_init), HAL_MAX_DELAY) != HAL_OK)
	{
		Error_Handler();
	}

	if(HAL_I2C_Master_Transmit(&p_i2c_handle, dev_address, unknown_init, sizeof(unknown_init), HAL_MAX_DELAY) != HAL_OK)
	{
		Error_Handler();
	}

	if(HAL_I2C_Master_Transmit(&p_i2c_handle, dev_address, init5, sizeof(init5), HAL_MAX_DELAY) != HAL_OK)
	{
		Error_Handler();
	}*/

	// Write data

	const uint8_t data_cmd[] = { 0x00, 0x10, 0x02, 0xB0 };
	const uint8_t data_data[] = {0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff };

	/*if(HAL_I2C_Master_Transmit(&p_i2c_handle, dev_address, data, sizeof(data), HAL_MAX_DELAY) != HAL_OK)
	{
		Error_Handler();
	}*/


	uint8_t current_byte = 1;
	if(HAL_I2C_Master_Transmit(&p_i2c_handle, dev_address, data_cmd, sizeof(data_cmd), HAL_MAX_DELAY) != HAL_OK)
	{
		Error_Handler();
	}
	while(1)
	{
		//if(HAL_SPI_Transmit(&p_spi_handle, &current_byte, sizeof(current_byte), HAL_MAX_DELAY) != HAL_OK)
		/*if(HAL_I2C_Master_Transmit(&p_i2c_handle, dev_address, &current_byte, sizeof(current_byte), HAL_MAX_DELAY) != HAL_OK)
		{
			Error_Handler();
		}*/


		if(HAL_I2C_Master_Transmit(&p_i2c_handle, dev_address, data_data, sizeof(data_data), HAL_MAX_DELAY) != HAL_OK)
		{
			Error_Handler();
		}

		sprintf(msg, message_template, current_byte);
		DebugWrite(msg);
		current_byte++;
		//HAL_Delay(10);
	}
}

void DebugWrite(const char *message)
{
	//char *msg;
	//int	sprintf (char *__restrict, const char *__restrict, ...)
	//sprintf(msg, message);
	if(HAL_UART_Transmit(&p_uart_handle, (uint8_t *)message, strlen(message), HAL_MAX_DELAY) != HAL_OK)
	{
		Error_Handler();
	}
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(CS_I2C_SPI_GPIO_Port, CS_I2C_SPI_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(OTG_FS_PowerSwitchOn_GPIO_Port, OTG_FS_PowerSwitchOn_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, LD4_Pin|LD3_Pin|LD5_Pin|LD6_Pin
                          |Audio_RST_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : CS_I2C_SPI_Pin */
  GPIO_InitStruct.Pin = CS_I2C_SPI_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(CS_I2C_SPI_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : OTG_FS_PowerSwitchOn_Pin */
  GPIO_InitStruct.Pin = OTG_FS_PowerSwitchOn_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(OTG_FS_PowerSwitchOn_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PDM_OUT_Pin */
  GPIO_InitStruct.Pin = PDM_OUT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
  HAL_GPIO_Init(PDM_OUT_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_EVT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : I2S3_WS_Pin */
  GPIO_InitStruct.Pin = I2S3_WS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF6_SPI3;
  HAL_GPIO_Init(I2S3_WS_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : BOOT1_Pin */
  GPIO_InitStruct.Pin = BOOT1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(BOOT1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : CLK_IN_Pin */
  GPIO_InitStruct.Pin = CLK_IN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
  HAL_GPIO_Init(CLK_IN_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LD4_Pin LD3_Pin LD5_Pin LD6_Pin
                           Audio_RST_Pin */
  GPIO_InitStruct.Pin = LD4_Pin|LD3_Pin|LD5_Pin|LD6_Pin
                          |Audio_RST_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : I2S3_MCK_Pin I2S3_SCK_Pin I2S3_SD_Pin */
  GPIO_InitStruct.Pin = I2S3_MCK_Pin|I2S3_SCK_Pin|I2S3_SD_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF6_SPI3;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : VBUS_FS_Pin */
  GPIO_InitStruct.Pin = VBUS_FS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(VBUS_FS_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : OTG_FS_ID_Pin OTG_FS_DM_Pin OTG_FS_DP_Pin */
  GPIO_InitStruct.Pin = OTG_FS_ID_Pin|OTG_FS_DM_Pin|OTG_FS_DP_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF10_OTG_FS;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : OTG_FS_OverCurrent_Pin */
  GPIO_InitStruct.Pin = OTG_FS_OverCurrent_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(OTG_FS_OverCurrent_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : MEMS_INT2_Pin */
  GPIO_InitStruct.Pin = MEMS_INT2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_EVT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(MEMS_INT2_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
