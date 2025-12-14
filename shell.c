#include <stddef.h>
#include <stdint.h>

#include "shell.h"
#include "serial.h"
#include "mmio.h"
#include "bcm2835_addr.h"
#include "delay.h"
#include "i2c.h"
#include "string.h"
//#include "shell_commands.h"
#include "printk.h"
#include "buttons.h"
#include "pwm.h"



uint32_t rainbow_shell(void) {

	uint32_t ch;
	char buffer[256];
	int input_len = 0;
	int command_prompt = 0;
	int command_active = 0;
	
	/* Enter the "rainbow" */

	while(1){

		if(command_prompt == 0){
			printk("--> ");	// printing shell prompt
			command_prompt = 1;
		}
		//printk("I am here.\n");		//debuggging
		// detects button press
		if(read_button(0)){
			pwm_set_duty(256);   // 25% duty cycle
			printk("\nButton 1 pressed: Set PWM to .25 duty cycle.\n");
			delay(0x500000); // Simple debounce delay
			while(!button_pressed(0)); // wait for button release
			command_prompt = 0;
		}
		if(read_button(1)){
			printk("\nButton 2 pressed.\n");
			printk("Enter command.\n");
			command_active = 1;
			
			delay(0x500000); // Simple debounce delay
		}

		if(command_active == 1){
			
			
			//printk("And now I am here.\n");		//debugging
			ch = uart_getc();

			/* handle Enter being pressed */
			if (ch=='\n' || ch=='\r') {
				
				buffer[input_len] = '\0';		 //null terminated
				parse_input(buffer);
				
				buffer[0] = '\0';				 //clear buffer
				input_len = 0;
				command_active = 0;
				command_prompt = 0;
				continue;
			}

			else{
				uart_putc(ch);
				if (input_len < (int)(sizeof(buffer) - 1)) {		// Save to buffer if space remains
					buffer[input_len++] = ch;						// otherwise ignore excess characters
				}
			}
		}	
			
		
	}
	printk("Hey! You shouldn't be here!\n");
}
