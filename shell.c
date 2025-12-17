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
#include "timer.h"
#include "ui.h"



uint32_t rainbow_shell(void) {

	// vars
	int track_command = 0;
	int on_display = 0;

	const char* cmd_buf[] = {
		"Read Moisture",
		"Cycle Pump",
		"Presets",
		"Set Time",
		"Debug Mode"
	};
	
	/* Enter the "rainbow" */

	lcd_cmd(0x01); // clear
	lcd_set_cursor(0, 0);
    lcd_print("Menu:");
    delay_ms(150);
	lcd_set_cursor(1, 0);
    lcd_print("Read Moisture");

	printk("Entering button loop\n");		//debugging

	while(1){

		asm("");        // avoid optimization

		if(on_display){

			// show current command
			lcd_cmd(0x01); // clear
			lcd_set_cursor(0, 0);
			lcd_print("Menu:");
			lcd_set_cursor(1, 0);
			lcd_print(cmd_buf[track_command]);

			//resetting on_display
			on_display = 0;
		}

		if(read_button(0)){

			printk("Button 1 clicked\n");	//debugging
			// see time
			
			while(!read_button(2)){		//click ok to go back to menu
				get_time();
			}
			delay_ms(100);
		}

		if(read_button(1)){

			printk("button 2 clicked\n");	//debugging
			// move back a command
			if(track_command > 0){
				track_command--;
			}
			else{
				// wrap around
				track_command = 4;
			}
			on_display = 1;
		}

		if(read_button(2)){

			printk("button 3 clicked\n");	//debugging
			// do selected command
			do_command(track_command);

			on_display = 1;
		}

		if(read_button(3)){

			printk("button 4 clicked\n");
			// move forward a command
			if(track_command < 5){
				track_command++;
			}
			else{
				// wrap around
				track_command = 0;
			}
			on_display = 1;
		}
		delay_ms(20);	//short debouncing delay
	}

	printk("Hey! You shouldn't be here!\n");	//debugging
}
