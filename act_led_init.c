#include <stdint.h>
#include "bcm2835_addr.h"
#include "mmio.h"
#include "delay.h"
#include "act_led_init.h"
#include "gpio.h"

extern uint32_t act_led_gpio;

int act_on(void){
    gpio_write(act_led_gpio, 1);                // ACT led on
    return 0;
}

int act_off(void){
    gpio_write(act_led_gpio, 0);              // ACT led off
    return 0;
}
void heartbeat_init(){

    gpio_set_output(act_led_gpio);  

    return;
}



