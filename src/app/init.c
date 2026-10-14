#include <stdint.h>
#include "init.h"
#include "stm32f7xx_hal.h"

static UART_HandleTypeDef huart3;

void SysTick_Handler(void)
{
    HAL_IncTick();
}

void delay(uint32_t milliseconds)
{
    HAL_Delay(milliseconds);
}

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
    if (huart->Instance != USART3) {
        return;
    }

    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_USART3_CLK_ENABLE();

    GPIO_InitTypeDef gpio = {
        .Pin = GPIO_PIN_8 | GPIO_PIN_9,
        .Mode = GPIO_MODE_AF_PP,
        .Pull = GPIO_PULLUP,
        .Speed = GPIO_SPEED_FREQ_VERY_HIGH,
        .Alternate = GPIO_AF7_USART3
    };
    HAL_GPIO_Init(GPIOD, &gpio);
}

HAL_StatusTypeDef init_startup(void)
{
    GPIO_InitTypeDef led = {
        .Pin = GPIO_PIN_0,
        .Mode = GPIO_MODE_OUTPUT_PP,
        .Pull = GPIO_NOPULL,
        .Speed = GPIO_SPEED_FREQ_LOW
    };

    if (HAL_Init() != HAL_OK) {
        return HAL_ERROR;
    }

    __HAL_RCC_GPIOB_CLK_ENABLE();
    HAL_GPIO_Init(GPIOB, &led);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);

    huart3.Instance = USART3;
    huart3.Init.BaudRate = 115200;
    huart3.Init.WordLength = UART_WORDLENGTH_8B;
    huart3.Init.StopBits = UART_STOPBITS_1;
    huart3.Init.Parity = UART_PARITY_NONE;
    huart3.Init.Mode = UART_MODE_TX_RX;
    huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart3.Init.OverSampling = UART_OVERSAMPLING_16;

    if (HAL_UART_Init(&huart3) != HAL_OK) {
        return HAL_ERROR;
    }

    return serial_write((const uint8_t *)"boot\r\n", 6);
}

HAL_StatusTypeDef serial_write(const uint8_t *data, uint16_t length)
{
    return HAL_UART_Transmit(&huart3, (uint8_t *)data, length, HAL_MAX_DELAY);
}
