/*
 * msp.c
 *
 *  Created on: Oct 17, 2020
 *      Author: s0lid
 */

#include "msp.h"


void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_USART2_CLK_ENABLE();

	GPIO_InitTypeDef p_gpio_usart_pin;

	p_gpio_usart_pin.Mode = GPIO_MODE_AF_PP;
	p_gpio_usart_pin.Alternate = GPIO_AF7_USART2;
	p_gpio_usart_pin.Pull = GPIO_PULLUP;
	p_gpio_usart_pin.Speed = GPIO_SPEED_FREQ_LOW;

	p_gpio_usart_pin.Pin = GPIO_PIN_2;
	HAL_GPIO_Init(GPIOA, &p_gpio_usart_pin);

	p_gpio_usart_pin.Pin = GPIO_PIN_3;
	HAL_GPIO_Init(GPIOA, &p_gpio_usart_pin);
}
/*
void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
{
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_SPI1_CLK_ENABLE();

	GPIO_InitTypeDef p_gpio_spi_pin;
	p_gpio_spi_pin.Mode = GPIO_MODE_AF_PP;
	p_gpio_spi_pin.Alternate = GPIO_AF5_SPI1;
	p_gpio_spi_pin.Pull = GPIO_NOPULL;
	p_gpio_spi_pin.Speed = GPIO_SPEED_LOW;

	// CLK
	p_gpio_spi_pin.Pin = GPIO_PIN_5;
	HAL_GPIO_Init(GPIOA, &p_gpio_spi_pin);

	// MOSI
	p_gpio_spi_pin.Pin = GPIO_PIN_7;
	HAL_GPIO_Init(GPIOA, &p_gpio_spi_pin);
current_byte
	// NSS
	p_gpio_spi_pin.Pin = GPIO_PIN_4;
	HAL_GPIO_Init(GPIOA, &p_gpio_spi_pin);

}
*/

void HAL_I2C_MspInit(I2C_HandleTypeDef *hi2c)
{
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_I2C1_CLK_ENABLE();

	GPIO_InitTypeDef p_gpio_b;

	p_gpio_b.Alternate = GPIO_AF4_I2C1;
	p_gpio_b.Mode = GPIO_MODE_AF_OD;
	p_gpio_b.Pull = GPIO_PULLUP;
	p_gpio_b.Speed = GPIO_SPEED_LOW;

	//SCL -> 6
	p_gpio_b.Pin = GPIO_PIN_6;
	HAL_GPIO_Init(GPIOB, &p_gpio_b);

	//SDA -> 7
	p_gpio_b.Pin = GPIO_PIN_7;
	HAL_GPIO_Init(GPIOB, &p_gpio_b);
}
