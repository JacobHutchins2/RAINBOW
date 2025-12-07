#include <stdint.h>
#include "bcm2835_addr.h"
#include "mmio.h"
#include "delay.h"
#include "act_led_init.h"
extern uint32_t act_led_gpio;
int act_on(void){
    uint32_t *gpio = (uint32_t *)GPIO_BASE;       // Points to GPIO base address
    gpio[GPIO_SET1] = (1 << (act_led_gpio - 32)); // Set GPIO45 high
    //delay(0x2200000);                  // Delay
    return 0;
}

int act_off(void){
    uint32_t *gpio = (uint32_t *)GPIO_BASE;       // Points to GPIO base address
    gpio[GPIO_CLR1] = (1 << (act_led_gpio - 32)); // Set GPIO45 low
    //delay(0x2200000);                  // Delay
    return 0;
}
void heartbeat_init(){

    uint32_t *gpio = (uint32_t *)GPIO_BASE;       // Points to GPIO base address

    gpio[GPIO_FSEL4] &= ~(0x7 << 21); // Clear bits for GPIO47
    gpio[GPIO_FSEL4] |=  (0x1 << 21);  // Set output high
/*
    while(1){
        gpio[GPIO_SET1] = (1 << (45 - 32)); // Set GPIO45 high
        delay(0x2200000);                  // Delay
        gpio[GPIO_CLR1] = (1 << (45 - 32)); // Set GPIO45 low
        delay(0x2200000);                  // Delay
    }
*/
    
}



