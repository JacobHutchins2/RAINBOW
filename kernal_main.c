#include <stddef.h>
#include <stdint.h>

#include "bcm2835_addr.h"
#include "mmio.h"
#include "delay.h"
#include "active_led_init.h"

// inlcude header of header files

//The start of the end.
void kernal_main(uint32_t r0, uint32_t r1, uint32_t r2){

    // active LED start to show a heartbeat
    heartbeat();

    // Register base setup
    /* GPIO addresses setup in bcm2835_addr.c */

    // hardware initialization / interrupts
    uart_init();
    //spi_init();
    //interrupt_enable();
    // timer setup
    //timer_init();

    // sensor initialization / control
    i2c_init();

    // data processing
    //moisture_data();

    // display data
    //moisture_display();

    // water output control
    //pump_control();

    // enter shell/testing environment
    rainbow_shell();

    while(1){

    }

}