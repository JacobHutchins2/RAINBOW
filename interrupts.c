#include <stdint.h>
#include "bcm2835_addr.h"
#include "mmio.h"
#include "act_led_init.h"
#include "timer.h"
#include "printk.h"

// global var
int blink_en = 1;

void __attribute__((interrupt("IRQ"))) interrupt_handler(void) {

	static int lit = 0;
	uint32_t pending;

    pending = bcm2835_read(IRQ_BASIC_PENDING);

    if(pending & IRQ_BASIC_PENDING_TIMER){
        bcm2835_write(TIMER_IRQ_CLEAR, 0x1);
    
        if(blink_en){
            if(lit){        // ACT blink on
                act_on();
                lit = 0;
            }
            else{
                act_off();  // ACT blink off
                lit = 1;
            }
        }
        tick_counter++;     // increment timer
    }
    else{
        printk("Unkown interrupt %x\n", pending);
    }
}

void __attribute__((interrupt("UNDEF"))) undefined_handler(void) {

	printk("Undefined instruction!\n");
	while(1);

}

void __attribute__((interrupt("ABORT"))) prefetch_handler(void) {

	printk("Unexpected prefetch handler interrupt!\n");
	while(1);

}

void __attribute__((interrupt("ABORT"))) data_handler(void) {

	printk("Unexpected data handler interrupt!\n");
	while(1);

}

void __attribute__((interrupt("FIQ"))) fiq_handler(void) {

	printk("Unexpected FIQ interrupt!\n");
	while(1);

}

void __attribute__((interrupt("SVC"))) reset_error(void) {

	printk("Reset triggered!\n");
	while(1);

}
