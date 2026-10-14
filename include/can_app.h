#ifndef CAN_APP_H
#define CAN_APP_H

#include "stm32f7xx_hal.h"
#include "stm32f7xx_hal_can.h"

extern CAN_HandleTypeDef hcan1;

HAL_StatusTypeDef CAN1_Init(void);
HAL_StatusTypeDef CAN1_TestLoopback(void);

#endif
