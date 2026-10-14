#include "can_app.h"
#include "init.h"
#include <stdio.h>
#include <string.h>

CAN_HandleTypeDef hcan1;

static HAL_StatusTypeDef can_report(const char *message)
{
    return serial_write((const uint8_t *)message, (uint16_t)strlen(message));
}

HAL_StatusTypeDef CAN1_Init(void)
{
    GPIO_InitTypeDef gpio = {0};

    // Enable GPIOA and CAN1 clocks.
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_CAN1_CLK_ENABLE();

    // PA11 = CAN1_RX, PA12 = CAN1_TX, AF9.
    gpio.Pin       = GPIO_PIN_11 | GPIO_PIN_12;
    gpio.Mode      = GPIO_MODE_AF_PP;
    gpio.Pull      = GPIO_PULLUP;
    gpio.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
    gpio.Alternate = GPIO_AF9_CAN1;

    HAL_GPIO_Init(GPIOA, &gpio);

    // Configure CAN1.
    hcan1.Instance = CAN1;

    hcan1.Init.Mode = CAN_MODE_LOOPBACK;

    /* 16 MHz PCLK1 / (2 * (1 + 13 + 2)) = 500 kbit/s. */
    hcan1.Init.Prescaler = 2;
    hcan1.Init.SyncJumpWidth = CAN_SJW_1TQ;
    hcan1.Init.TimeSeg1 = CAN_BS1_13TQ;
    hcan1.Init.TimeSeg2 = CAN_BS2_2TQ;

    hcan1.Init.TimeTriggeredMode = DISABLE;
    hcan1.Init.AutoBusOff = DISABLE;
    hcan1.Init.AutoWakeUp = DISABLE;
    hcan1.Init.AutoRetransmission = DISABLE;
    hcan1.Init.ReceiveFifoLocked = DISABLE;
    hcan1.Init.TransmitFifoPriority = DISABLE;

    if (HAL_CAN_Init(&hcan1) != HAL_OK)
    {
        return HAL_ERROR;
    }

    // Accept every standard or extended CAN identifier.
    CAN_FilterTypeDef filter = {0};

    filter.FilterBank = 0;
    filter.FilterMode = CAN_FILTERMODE_IDMASK;
    filter.FilterScale = CAN_FILTERSCALE_32BIT;
    filter.FilterIdHigh = 0;
    filter.FilterIdLow = 0;
    filter.FilterMaskIdHigh = 0;
    filter.FilterMaskIdLow = 0;
    filter.FilterFIFOAssignment = CAN_FILTER_FIFO0;
    filter.FilterActivation = ENABLE;

    // Relevant for HAL/device variants with shared CAN1/CAN2 filters.
    filter.SlaveStartFilterBank = 14;

    if (HAL_CAN_ConfigFilter(&hcan1, &filter) != HAL_OK)
    {
        return HAL_ERROR;
    }

    if (HAL_CAN_Start(&hcan1) != HAL_OK)
    {
        return HAL_ERROR;
    }

    return HAL_OK;
}

HAL_StatusTypeDef CAN1_TestLoopback(void)
{
    CAN_TxHeaderTypeDef tx_header = {
        .StdId = 0x123U,
        .IDE = CAN_ID_STD,
        .RTR = CAN_RTR_DATA,
        .DLC = 8U,
        .TransmitGlobalTime = DISABLE
    };
    CAN_RxHeaderTypeDef rx_header = {0};
    uint8_t tx_data[8] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88};
    uint8_t rx_data[8] = {0};
    uint32_t mailbox;
    char message[128];

    if (HAL_CAN_AddTxMessage(&hcan1, &tx_header, tx_data, &mailbox) != HAL_OK) {
        return can_report("CAN TX enqueue failed\r\n");
    }

    uint32_t start = HAL_GetTick();
    while (HAL_CAN_GetRxFifoFillLevel(&hcan1, CAN_RX_FIFO0) == 0U) {
        if ((HAL_GetTick() - start) >= 100U) {
            return can_report("CAN RX timeout\r\n");
        }
    }

    if (HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO0, &rx_header, rx_data) != HAL_OK) {
        return can_report("CAN RX error\r\n");
    }

    int length = snprintf(
        message, sizeof(message),
        "CAN RX ID=0x%03lX DLC=%lu %02X %02X %02X %02X %02X %02X %02X %02X\r\n",
        (unsigned long)rx_header.StdId,
        (unsigned long)rx_header.DLC,
        rx_data[0], rx_data[1], rx_data[2], rx_data[3],
        rx_data[4], rx_data[5], rx_data[6], rx_data[7]);

    if (length < 0 || (size_t)length >= sizeof(message)) {
        return HAL_ERROR;
    }

    return serial_write((const uint8_t *)message, (uint16_t)length);
}
