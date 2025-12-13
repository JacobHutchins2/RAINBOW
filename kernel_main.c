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
#include "tft.h"
#include "gpio.h"
#include "buttons.h"
#include "pwm.h"
// inlcude header of header files

uint32_t io_base = 0x20000000;
uint32_t act_led_gpio = 47;


//The start of the end.
void kernel_main(){

    //gpio_write(5, 1);   // Turn on backlight

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
    //spi_test_byte(0x2A);   // send ILI9341 “Column Address Set”
    //while(1);
    tft_init();
    printk("Testing TFT Print\n");      // debugging
    printt(10, 10, "Testing print", 0xFFFF, 0x0000);
    // display data
    //moisture_display();

    // water output control
    pwm_init();
    printk("PWM Initialized.\n");
    pwm_set_duty(512);   // 50% duty cycle
    printk("PWM Duty Set to 50%%.\n");
    buttons_init();
    printk("Buttons Initialized.\n");
    //pump_control();
    spi_init();
    tft_gpio_init();
    tft_init();

    // Fill screen blue
    /*tft_write_cmd(0x2A);
    tft_write_data(0); tft_write_data(0);
    tft_write_data(0); tft_write_data(239);

    tft_write_cmd(0x2B);
    tft_write_data(0); tft_write_data(0);
    tft_write_data(1); tft_write_data(0x3F);

    tft_write_cmd(0x2C);

    uint8_t blue[2] = { 0x00, 0x1F };
    for (int i = 0; i < 240*320; i++) {
        tft_write_buf(blue, 2);
    }*/

    //while (1);

    // enter shell/testing environment
    rainbow_shell();
    printk("Why am I here?\n");     // debugging
    while(1){

    }

}