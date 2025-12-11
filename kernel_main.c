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
#include "printk.h"
#include "spi.h"
// inlcude header of header files

uint32_t io_base = 0x20000000;
uint32_t act_led_gpio = 47;


//The start of the end.
void kernel_main(){

    // hardware initialization / interrupts
    uart_init();
    printk("UART initialized.\n");
    // active LED start to show a heartbeat
    heartbeat_init();
    printk("Heartbeat started.\n");
    // Register base setup
    /* GPIO addresses setup in bcm2835_addr.c */

    // timer setup
    timer_init();
    printk("Timer Initialized.\n");
    //spi_init();
    enable_interrupts();
    printk("Interrupts Enabled.\n");

    // sensor initialization / control
    i2c_init();
    printk("i2c Initialized.\n");
    // data processing
    //moisture_data();
    // Add this at system initialization, before any 'read' commands
    

    spi_init();
    tft_gpio_init();
    tft_init();

    // display data
    //moisture_display();

    // water output control
    //pump_control();

    // enter shell/testing environment
    rainbow_shell();
    printk("Why am I here?\n");     // debugging
    while(1){

    }

}