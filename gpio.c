#include <stdint.h>
#include "gpio.h"
#include "printk.h"
#include "mmio.h"
#include "bcm2835_addr.h"

// Read value from GPIO pin
uint32_t gpio_read(uint32_t pin){
    uint32_t reg = GPIO_LEV0 + ((pin / 32) * 4);
    return (bcm2835_read(reg) >> (pin % 32)) & 1;
}

// Write value to GPIO pin
void gpio_write(uint32_t pin, uint32_t value){
    uint32_t bit = 1 << (pin % 32);

    if (value)
        bcm2835_write(GPIO_SET0 + ((pin / 32) * 4), bit);
    else
        bcm2835_write(GPIO_CLR0 + ((pin / 32) * 4), bit);
}

/*
                ALT0 = 4
                ALT1 = 5
                ALT2 = 6
                ALT3 = 7
                ALT4 = 3
                ALT5 = 2
*/
// Set GPIO pin to alternate function
void gpio_set_alt(uint32_t pin, uint32_t alt){
    uint32_t reg   = GPIO_FSEL0 + ((pin / 10) * 4);
    uint32_t shift = (pin % 10) * 3;

    uint32_t val = bcm2835_read(reg);
    val &= ~(7 << shift);
    val |=  (alt << shift);
    bcm2835_write(reg, val);
}

// Set GPIO pin as input
void gpio_set_input(uint32_t pin){
    uint32_t reg   = GPIO_FSEL0 + ((pin / 10) * 4);
    uint32_t shift = (pin % 10) * 3;

    uint32_t val = bcm2835_read(reg);
    val &= ~(7 << shift);    // 000 = input
    bcm2835_write(reg, val);
}

// Set GPIO pin as output
void gpio_set_output(uint32_t pin){
    uint32_t reg   = GPIO_FSEL0 + ((pin / 10) * 4);
    uint32_t shift = (pin % 10) * 3;

    uint32_t val = bcm2835_read(reg);
    val &= ~(7 << shift);
    val |=  (1 << shift);    // 001 = output
    bcm2835_write(reg, val);
}

/*
            GPIO_PUD_DISABLE
            GPIO_PUD_PULLDOWN
            GPIO_PUD_PULLUP
*/
// Set GPIO pin pull-up/pull-down resistor
void gpio_set_pull(uint32_t pin, uint32_t pud){
    uint32_t clk = GPIO_PUDCLK0 + ((pin / 32) * 4);
    uint32_t bit = 1 << (pin % 32);

    bcm2835_write(GPIO_PUD, pud);
    delay(150);

    bcm2835_write(clk, bit);
    delay(150);

    bcm2835_write(GPIO_PUD, 0);
    bcm2835_write(clk, 0);
}
