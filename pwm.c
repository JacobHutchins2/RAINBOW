#include <stdint.h>
#include "bcm2835_addr.h"
#include "mmio.h"
#include "printk.h"
#include "delay.h"
#include "gpio.h"

void pwm_init(void){

    gpio_set_alt(18, 2);   // Set GPIO18 to ALT5 PWM0
    // Stop PWM
    bcm2835_write(PWM_CTL, 0);
    delay(150);

    // Stop PWM clock
    bcm2835_write(CM_PWMCTL, CM_PASSWORD | (1 << 5));
    delay(150);

    // Set clock divider 100kHz
    bcm2835_write(CM_PWMDIV, CM_PASSWORD | (2500 << 12));

    // Start PWM clock
    bcm2835_write(CM_PWMCTL, CM_PASSWORD | 0x11);
    delay(150);

    // Set range
    bcm2835_write(PWM_RNG1, 1024);
    delay(150);

    // Enable PWM channel 1, mark-space mode
    bcm2835_write(PWM_CTL, (1 << 7) | (1 << 0));
}

void pwm_set_duty(uint32_t duty){
    if (duty > 1024) duty = 1024;
    bcm2835_write(PWM_DAT1, duty);
}