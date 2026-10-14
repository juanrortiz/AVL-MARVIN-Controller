#ifndef INIT_H
#define INIT_H
#include <stdint.h>
#include "stm32f7xx_hal_def.h"

void delay(uint32_t milliseconds);
HAL_StatusTypeDef init_startup(void);
HAL_StatusTypeDef serial_write(const uint8_t *data, uint16_t length);
#endif
