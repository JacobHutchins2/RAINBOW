#include <stddef.h>
#include <stdint.h>

#include "bcm2835_addr.h"
#include "mmio.h"
#include "delay.h"
#include "act_led_init.h"
#include "serial.h"
#include "i2c.h"
#include "shell.h"
#include "interrupts.h"
#include "timer.h"

// inlcude header of header files

uint32_t io_base = 0x20000000;
uint32_t act_led_gpio = 47;

//The start of the end.
void kernel_main(uint32_t r0, uint32_t r1, uint32_t r2){

    // active LED start to show a heartbeat
    heartbeat_init();

    // Register base setup
    /* GPIO addresses setup in bcm2835_addr.c */

    // hardware initialization / interrupts
    uart_init();
    // timer setup
    timer_init();
    //spi_init();
    enable_interrupts();

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