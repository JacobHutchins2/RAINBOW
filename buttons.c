#include <stdint.h>
#include "bcm2835_addr.h"
#include "mmio.h"
#include "printk.h"
#include "delay.h"
#include "gpio.h"

void buttons_init(void){
    uint32_t pins[] = {17, 27, 22, 23};

    for (int i = 0; i < 4; i++) {
        gpio_set_input(pins[i]);
        gpio_set_pull(pins[i], GPIO_PUD_DISABLE); // Disable pull-up/down resistors
    }

}

uint32_t read_button(uint32_t button_num){
    uint32_t pins[] = {17, 27, 22, 23};

    if (button_num >= 4) {
        return 0; // Invalid button number
    }

    return gpio_read(pins[button_num]) == 1; // Active high
}

uint32_t button_pressed(uint32_t pin){
    return gpio_read(pin) == 1;
}

void buttons_test(void){
    printk("Button Test: Press buttons to see their status.\n");
    while (1) {
        for (int i = 0; i < 4; i++) {
            if (read_button(i)) {
                printk("Button %d pressed!\n", i + 1);
            }
        }
        delay(0x20000); // Simple debounce delay
    }
}   
