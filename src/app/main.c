#include "stm32f7xx_hal.h"
#include "init.h"
#include "can_app.h"

void __libc_init_array(void) {}

static void startup_error(void)
{
    while (1) {
        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);
        for (volatile uint32_t delay = 0U; delay < 1000000U; ++delay) {
        }
    }
}

int main(void)
{
    if (init_startup() != HAL_OK) {
        startup_error();
    }

    while (1) {
        if (serial_write((const uint8_t *)"Hello World\r\n", 13) != HAL_OK) {
            startup_error();
        }
        
        if (CAN1_Init() != HAL_OK || CAN1_TestLoopback() != HAL_OK) {
            startup_error();
        }
    
        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);
        delay(1000);
    }
}
