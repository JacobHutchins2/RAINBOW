#include <stddef.h>
#include <stdint.h>
#include "syscalls.h"
#include "timer.h"
#include "printk.h"
#include "interrupts.h"
#include "display_io.h"
#include "serial.h"

extern int blink_en;

uint32_t __attribute__((interrupt("SWI"))) swi_handler(
	uint32_t r0, uint32_t r1, uint32_t r2, uint32_t r3) {

	register long r7 asm ("r7");
	uint32_t result = 0;
	char *ptr;

    switch(r7) {
		case SYSCALL_READ:
			ptr = (char *)r1;
 
			if (r0 == 0) {
				result = serial_read(ptr,(size_t)r2);
			}
			else {
				printk("Attempting to read from unsupported fd %d\n",r0);
				result=-1;
			}

			break;

		case SYSCALL_WRITE:
			ptr=(char *)r1;

			if ((r0 == 1) || (r0 == 2)) {
				result = serial_write(ptr, (size_t)r2);
  
			}
			else {
				printk("Attempting to write unsupported fd %d\n",r0);
				result = -1;
			}
			break;

		case SYSCALL_BLINK:
			if (r0 == 0) {
				printk("DISABLING BLINK\n");
				blink_en = 0;
			}
			else {
				printk("ENABLING_BLINK\n");
				blink_en = 1;
			}
			break;

		case SYSCALL_TIME:
			if(r0 != 0){
				int *int_ptr; 			//point to int
				int_ptr = (int *)r0;	//set pointer to point to address in r0
				*int_ptr = tick_counter;	//set value at address to tick counter
				result = 0;
			}
			else{
				result = -1;			//error if null pointer
			}
			break;			

		default:
			printk("Unknown syscall %d\n",r7);
			break;
	}
	
    return result;
}