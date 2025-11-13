#include <stdint.h>
#include "bcm2835_addr.h"
#include "mmio.h"
#include "delay.h"
#include "act_led_init.h"

int heartbeat(){

    gpio = (uint32_t *)GPIO_BASE;       // Points to GPIO base address

    gpio[GPFSEL4] &= ~(0x7 << 21); // Clear bits for GPIO45
    gpio[GPFSEL4] |=  (0x1 << 21);  // Set GPIO45 as output

    while(1){
        gpio[GPSET1] = (1 << (45 - 32)); // Set GPIO45 high
        delay(0x2200000);                  // Delay
        gpio[GPCLR1] = (1 << (45 - 32)); // Set GPIO45 low
        delay(0x2200000);                  // Delay
    }

    return 0;
}

