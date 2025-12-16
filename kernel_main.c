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

    // init i2c
    i2c_init();
    printk("i2c Initialized.\n");

    // init lcd
    lcd_init();
    printk("lcd initialized.\n");
    lcd_set_cursor(0, 0);
    lcd_print("UART INITIALIZED.");
    lcd_set_cursor(0, 0);
    lcd_print("i2c INITIALIZED.");
    lcd_set_cursor(0, 0);
    lcd_print("LCD INITIALIZED.");

    // active LED start to show a heartbeat
    heartbeat_init();
    printk("Heartbeat started.\n");
    lcd_set_cursor(0, 0);
    lcd_print("HEARTBEAT INITIALIZED.");

    // timer setup
    timer_init();
    printk("Timer Initialized.\n");
    lcd_set_cursor(0, 0);
    lcd_print("TIMER INITIALIZED.");

    // init interrupts
    enable_interrupts();
    printk("Interrupts Enabled.\n");
    lcd_set_cursor(0, 0);
    lcd_print("INTR INITIALIZED.");

    // pwm init
    // water output control
    pwm_init();
    pwm_set_duty(0);    //making sure pump is off
    printk("PWM Initialized.\n");
    lcd_set_cursor(0, 0);
    lcd_print("PWM INITIALIZED.");

    // spi init
    spi_init();
    printk("SPI Initialized.\n");
    lcd_set_cursor(0, 0);
    lcd_print("SPI INITIALIZED.");
    lcd_cmd(0x01); // clear

    lcd_set_cursor(0, 0);
    lcd_print("Entering Shell.");
    delay_ms(150);
    lcd_cmd(0x01); // clear

    // enter shell/testing environment
    rainbow_shell();
    printk("Why am I here?\n");     // debugging
    lcd_set_cursor(0, 0);
    lcd_cmd(0x01); // clear
    lcd_print("ERROR W/ SHELL.");
    while(1){

    }

}