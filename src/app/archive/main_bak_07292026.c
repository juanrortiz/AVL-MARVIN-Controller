#include <stdint.h>
#include <sys/types.h>
#include "stm32f446xx.h"
#include "init.h"
// #include "uart.h"
void __libc_init_array(void) {}

volatile uint32_t msTicks = 0;           // Timer
volatile uint16_t adc_value[2] = {0, 0}; // {Y-Axis, X-Axis}

static uint32_t throttle = 0;          // Speed of motor
static uint32_t current_throttle = 0;  // Ramp-controlled throttle
static uint16_t idle_offset = 0;       // Offset for idle motor
static uint16_t steer = 400;           // Min Speed of inside wheel 60%
static uint8_t reverse = 1;            // Reverse biased
static uint8_t turn_right = 1;         // Right direction biased

// static uint8_t lastButtonState = 1;
// static uint32_t lastPress = 0;


void TIM2_IRQHandler(void)
{
  if (TIM2->SR & TIM_SR_UIF)
  {
    TIM2->SR &= ~TIM_SR_UIF;
    msTicks++;
  }
}

// static uint8_t led2State = 0;   /* move to file scope with your other state vars */

// void EXTI9_5_IRQHandler(void)
// {
//   if (EXTI->PR & (1U << 7))
//   {
//     EXTI->PR |= (1U << 7);

//     static uint32_t lastPress = 0;
//     if ((msTicks - lastPress) < 300)  /* ignore if pressed within 50ms */
//       return;
//     lastPress = msTicks;

//     if (led2State) {
//       GPIOA->BSRR = (1U << (10 + 16));
//       led2State = 0;
//     } else {
//       GPIOA->BSRR = (1U << 10);
//       led2State = 1;
//     }
//   }
// }

uint32_t rampSpeed(uint32_t target, uint32_t current, uint32_t max_step) {
    if (current < target) {
        current += max_step;
        if (current > target) current = target;   /* don't overshoot */
    } else if (current > target) {
        current -= max_step;
        if (current < target) current = target;   /* don't undershoot */
    }
    return current;
}

void driveControl(void){
  
  current_throttle = throttle; // rampSpeed(throttle, current_throttle, 5); /* max_step = 5 per cycle */

  uint16_t leftMotor = current_throttle;
  uint16_t rightMotor = current_throttle;

  if (turn_right) {
      rightMotor = (current_throttle * (1000 - steer)) / 1000;
  } else {
      leftMotor = (current_throttle * (1000 - steer)) / 1000;
  }

  TIM3->CCR1 = leftMotor;
  TIM3->CCR2 = rightMotor;
      
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

  if (idle_offset < 100){
    
    GPIOB->BSRR = (1U << (10 + 16)); // Clear AIN1
    GPIOA->BSRR = (1U << (8 + 16));  // Clear AIN2
    
    GPIOB->BSRR = (1U << (5 + 16));  // Clear BIN1
    GPIOB->BSRR = (1U << (3 + 16));  // Clear BIN2
    
    return;
  } // Idle statement
  
  /* Throttle control */
  if (reverse){
    GPIOA->BSRR = (1U << (8 + 16));  // Clear AIN2
    GPIOB->BSRR = (1U << 10);        // Set AIN1

    GPIOB->BSRR = (1U << (3 + 16));  // Clear BIN2
    GPIOB->BSRR = (1U << 5);         // Set BIN1
    
  }
  else {
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
