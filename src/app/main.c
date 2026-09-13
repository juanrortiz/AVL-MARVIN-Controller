#include <stdint.h>
#include <sys/types.h>
#include "stm32f446xx.h"
#include "init.h"

void __libc_init_array(void) {}

int main(void){
    init_startup(); // Initialize

    while(1){
        // Loop
    }
  
}
