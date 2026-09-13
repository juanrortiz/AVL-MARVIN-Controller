#include <stdint.h>
#include "init.h"
#include "stm32f446xx.h"

void delay(volatile uint32_t count){
  while(count--);
}

void init_startup(){
   
}


