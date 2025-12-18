#include <stdint.h>
#include "bcm2835_addr.h"
#include "mmio.h"
#include "act_led_init.h"
#include "timer.h"
#include "printk.h"

// global var
int blink_en = 1;
int end_day = 0;
extern int preset;
extern int water;

//interrupt
void __attribute__((interrupt("IRQ"))) interrupt_handler(void) {

	
	uint32_t pending;

    pending = bcm2835_read(IRQ_BASIC_PENDING);

    //running on timer.c clock interrupt
    if(pending & IRQ_BASIC_PENDING_TIMER){
        bcm2835_write(TIMER_IRQ_CLEAR, 0);
        
        static int lit = 0;

        if(blink_en){
            if(lit){        // ACT blink on
                act_off();
                lit = 0;
            }
            else{
                act_on();  // ACT blink off
                lit = 1;
            }
        }
        tick_counter++;     // increment timer

        
        //tracking time
        if(tick_counter >= 60){
            minute++;

            tick_counter = 0;
            if(end_day == 1){
                hour = 0;       //hour reset
                end_day = 0;
            }
            if(minute >= 60){
                hour++;     // next hour

                minute = 0;     //resetting minute
                if(hour >= 24){

                    end_day = 1;    //day reset 
                    water = 1;
                }
            }
        }
    }
    else{
        printk("Unkown interrupt %x\n", pending);
    }
}


