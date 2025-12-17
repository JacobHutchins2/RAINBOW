#include <stdint.h>
#include "bcm2835_addr.h"
#include "mmio.h"
#include "printk.h"
#include "delay.h"
#include "gpio.h"

void pwm_init(void) {

    // GPIO18 setting alt5
    gpio_set_alt(18, 2);

    // Disable PWM
    bcm2835_write(PWM_CTL, 0);
    delay(150);

    // Kill PWM clock
    bcm2835_write(CM_PWMCTL, CM_PASSWORD | (1 << 5));
    delay(150);

    // Set clock divider 19.2 MHz / 2<<12 = 10 kHz
    bcm2835_write(CM_PWMDIV, CM_PASSWORD | (2 << 12));      // tested on oscope. running @ 9.375kHz
    delay(150);

    // Enable PWM clock OSC enabled
    bcm2835_write(CM_PWMCTL, CM_PASSWORD | 0x11);
    delay(150);

    // Set range
    bcm2835_write(PWM_RNG1, 1024);  // 10k / 1024 ~ 10Hz
    delay(150);

    // Enable PWM channel 1
    bcm2835_write(PWM_CTL, PWM_CTL_PWEN1 | PWM_CTL_MSEN1);
}

void pwm_set_duty(uint32_t duty){
    if (duty > 1024) duty = 1024;
    bcm2835_write(PWM_DAT1, duty);
}