#include <stdint.h>
#include "mmio.h"
#include "bcm2835_addr.h"
#include "timer.h"

// global counter
uint32_t tick_counter = 0;

int timer_init(void){
    uint32_t old;

    // disable clock for configuration
    old = bcm2835_read(TIMER_CONTROL);
    old &= ~(TIMER_CONTROL_ENABLE | TIMER_CONTROL_INT_ENABLE);

    bcm2835_write(TIMER_PREDIVIDER, 0xf9);      // divide to 1MHz

    bcm2835_write(TIMER_LOAD, 3906);        // load counter for around 1Hz

    // timer in 32 bit mode
    bcm2835_write(TIMER_CONTROL, TIMER_CONTROL_32BIT | TIMER_CONTROL_ENABLE | TIMER_CONTROL_INT_ENABLE | TIMER_CONTROL_PRESCALE_256);

    // enable timer interrupt
    bcm2835_write(IRQ_ENABLE_BASIC_IRQ, IRQ_ENABLE_BASIC_IRQ_ARM_TIMER);

    return 0;
}