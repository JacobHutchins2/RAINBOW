#include <stdint.h>
#include "mmio.h"
#include "bcm2835_addr.h"
#include "timer.h"
#include "buttons.h"
#include "i2c.h"
#include "printk.h"
#include "delay.h"
#include "ui.h"
#include "pwm.h"

// global counter
uint32_t tick_counter = 0;
uint32_t hour = 0;
uint32_t minute = 0;

typedef enum {
    SET_HOUR,
    SET_MINUTE,
    SET_DONE
} time_set_t;

extern int preset;
int water = 1;

int timer_init(void){
    uint32_t old;

    // disable clock for configuration
    old = bcm2835_read(TIMER_CONTROL);
    old &= ~(TIMER_CONTROL_ENABLE | TIMER_CONTROL_INT_ENABLE);

    bcm2835_write(TIMER_PREDIVIDER, 0xf9);      // divide to 1MHz // 250MHz / 256

    bcm2835_write(TIMER_LOAD, 3906);        // load counter for around 1Hz

    // timer in 32 bit mode
    bcm2835_write(TIMER_CONTROL, TIMER_CONTROL_32BIT | TIMER_CONTROL_ENABLE | TIMER_CONTROL_INT_ENABLE | TIMER_CONTROL_PRESCALE_256);

    // enable timer interrupt
    bcm2835_write(IRQ_ENABLE_BASIC_IRQ, IRQ_ENABLE_BASIC_IRQ_ARM_TIMER);

    return 0;
}

void clock_set(void){

    printk("Entering Set clock\n");     //debugging

    static time_set_t state = SET_HOUR;

    delay_ms(100);  //short delay
    lcd_cmd(0x01); // clear
    lcd_set_cursor(0, 0);
    lcd_print("Set Clock");

    delay_ms(100);  //short delay
    lcd_cmd(0x01); // clear
    lcd_set_cursor(0, 0);
    lcd_print("Hour:   ");
    lcd_set_cursor(1, 0);
    lcd_print("Minute: ");
    
    // waiting for time to be inputted
    while(1){

        asm("");        // avoid optimization

        //var
        int quit = 0;
    
        // detecting buttons
        int up   = read_button(1); // UP
        int ok   = read_button(2); // OK
        int down = read_button(3); // DOWN

        //setting time
        switch (state) {

            //setting hour
            case SET_HOUR:
                if (up) {
                    hour = hour + 1;
                    lcd_set_cursor(0, 8);
                    lcd_print_int(hour);
                    delay_ms(10);
                }
                if (down) {
                    hour = (hour == 0) ? 23 : hour - 1;
                    lcd_set_cursor(0, 8);
                    lcd_print_int(hour);
                    delay_ms(10);
                }
                if (ok) {
                    state = SET_MINUTE;
                    printk("Now setting minute\n");        //debugging
                    delay_ms(200);
                }
                continue;

            //setting minute
            case SET_MINUTE:
                if (up) {
                    minute = (minute + 1);
                    lcd_set_cursor(1, 8);
                    lcd_print_int(minute);
                    delay_ms(10);
                }
                if (down) {
                    minute = (minute == 0) ? 59 : minute - 1;
                    lcd_set_cursor(1, 8);
                    lcd_print_int(minute);
                    delay_ms(10);
                }
                if (ok) {
                    state = SET_DONE;
                }
                continue;

            case SET_DONE:

                printk("In DONE state\n");      //debugging
                // exit time-setting mode
                // reset tick counter so time resumes cleanly
                tick_counter = 0;
                state = SET_HOUR;   // ready for next time-set
                quit = 1;
                break;
        }

        if(quit) break; //leave
        // small delay for button debounce
        delay_ms(200);

    }
    printk("Leaving Set clock\n");     //debugging
}

void get_time(void){

    // show current time
    lcd_cmd(0x01); // clear
    lcd_set_cursor(0, 0);
    lcd_print("Time:");
    lcd_set_cursor(1, 6);
    //print hour
    lcd_print_int(hour);
    //print minute
    lcd_print_int(minute);

    //printing time over serial
    printk("Time:\n     %d%d\n", hour, minute);
}

// for running the presets
void set_preset(void){
    // when the time is right.
    if((hour == 19) & ((minute == 0) | (minute = 59))){
        switch(preset){

            //preset 1 water at certain time and moisture level
            case 1:
                // retrieve moisture level
                uint16_t moisture = sensor_moisture();
                if(moisture < 600){
                    printk("Running Pump.\n");
                    pwm_set_duty(1024);
                    delay_ms(3000);
                    pwm_set_duty(0);
                    delay_ms(1000);
                }
                break;
            
            // preset 2, water at certain time
            case 2:
                if(water){
                    printk("Running Pump.\n");
                    pwm_set_duty(1024);
                    delay_ms(8000);
                    pwm_set_duty(0);
                    delay_ms(1000);
                    water = 0;
                }
                break;
        }
    }
}