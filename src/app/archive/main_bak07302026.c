#include <stdint.h>
#include <sys/types.h>
#include "stm32f446xx.h"
#include "init.h"
// #include "uart.h"
void __libc_init_array(void) {}

volatile uint32_t msTicks = 0;           // Timer
volatile uint16_t adc_value[2] = {0, 0}; // {Y-Axis, X-Axis}

static uint32_t throttle = 0;          // Speed of motor
static int32_t current_throttle = 0;   // Ramp-controlled throttle
static uint16_t idle_offset = 0;       // Offset for idle motor
static uint16_t steer = 400;           // Min Speed of inside wheel 60%
static uint8_t reverse = 1;            // Reverse biased
static uint8_t turn_right = 1;         // Right direction biased

void TIM2_IRQHandler(void)
{
    if (TIM2->SR & TIM_SR_UIF)
    {
        TIM2->SR &= ~TIM_SR_UIF;
        msTicks++;
    }
}
uint32_t rampSpeed(int32_t target, int32_t current, int32_t max_step) {
    if (current < target) {
        current += max_step;
        if (current > target) current = target;   /* don't overshoot */
    } else if (current > target) {
        current -= max_step;
        if (current < target) current = target;   /* don't undershoot */
    }
    return current;
}

void driveControl(void) {

    if (adc_value[0] >= 2048)
    {
      idle_offset = adc_value[0] - 2048;
      throttle = ((idle_offset) * 999) / 2047;
      reverse = 1;   // Reverse
    }
    else
    {
      idle_offset = 2048 - adc_value[0];
      throttle = ((idle_offset) * 999) / 2048;
      reverse = 0;   // Reverse
    }

    if (adc_value[1] >= 2048)
    {
        turn_right = 1;
        steer = ((adc_value[1] - 2048) * 400) / 2047;
    }
    else
    {
        turn_right = 0;
        steer = ((2048 - adc_value[1]) * 400) / 2048;
    }

    int32_t signed_target = reverse ? -(int32_t)throttle : (int32_t)throttle;

    static uint32_t last_ramp_tick = 0;
    if (msTicks - last_ramp_tick >= 1) {
        last_ramp_tick = msTicks;
        current_throttle = rampSpeed(signed_target, current_throttle, 1);
    }

    /* split signed value back into magnitude + direction */
    uint32_t speed = (current_throttle < 0) ? -current_throttle : current_throttle;
    uint8_t moving_reverse = (current_throttle < 0) ? 1 : 0;

    uint16_t leftMotor = speed;
    uint16_t rightMotor = speed;

    if (turn_right) {
        rightMotor = (speed * (1000 - steer)) / 1000;
    } else {
        leftMotor = (speed * (1000 - steer)) / 1000;
    }

    TIM3->CCR1 = leftMotor;
    TIM3->CCR2 = rightMotor;

    if (speed < 100) {
        GPIOB->BSRR = (1U << (10 + 16)); // Clear AIN1
        GPIOA->BSRR = (1U << (8 + 16));  // Clear AIN2
        GPIOB->BSRR = (1U << (5 + 16));  // Clear BIN1
        GPIOB->BSRR = (1U << (3 + 16));  // Clear BIN2
        return;
    }

    if (moving_reverse) {
        GPIOA->BSRR = (1U << (8 + 16));  // Clear AIN2
        GPIOB->BSRR = (1U << 10);        // Set AIN1
        GPIOB->BSRR = (1U << (3 + 16));  // Clear BIN2
        GPIOB->BSRR = (1U << 5);         // Set BIN1
    } else {
        GPIOB->BSRR = (1U << (10 + 16)); // Clear AIN1
        GPIOA->BSRR = (1U << 8);         // Set AIN2
        GPIOB->BSRR = (1U << (5 + 16));  // Clear BIN1
        GPIOB->BSRR = (1U << 3);         // Set BIN2
    }
}

int main(void){
    init_startup(adc_value); // Initialize

    GPIOB->BSRR = (1U << 4);  // Initialize STBY High

    while(1){
      driveControl();        // Control function

    }
  
}
