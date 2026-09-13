#include <stdint.h>
#ifndef INIT_H
#define INIT_H
void delay(volatile uint32_t count);
void init_startup(volatile uint16_t *adc_ptr);
void test_startup(volatile uint16_t *adc_ptr);
#endif
