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
#include "ili9341.h"
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
    //spi_init();
    //tft_init();
    spi_init();
    //tft_gpio_init();

    //tft_init();
    #if 0
    tft_set_window(0, 0, 239, 319);

    uint8_t red[2] = { 0xF8, 0x00 };
    gpio_write(TFT_DC, 1);

    for (int i = 0; i < 240 * 320; i++) {
        spi_write_stream(red, 2);
    }
    printk("Hit the infinite wait.\n");
    while (1);

    tft_fill(0x0000); // black

    tft_draw_string(10, 10, "HELLO BARE METAL", 0xFFFF, 0x0000);
    #endif
    /*
            Testing Display Driver
    */
    // init lcd
    lcd_init();
    
    /*lcd_set_cursor(0, 0);
    lcd_print("Hello");

    lcd_set_cursor(1, 0);
    lcd_print("Bare Metal!");


    lcd_cmd(0x80);        // line 1
    lcd_data('H');
    lcd_data('i');*/
    //while (1);
    //buttons_test();
    // enter shell/testing environment
    rainbow_shell();
    printk("Why am I here?\n");     // debugging
    while(1){

    }

}