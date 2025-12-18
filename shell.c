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
#include "interrupts.h"


extern int preset_flag;

uint32_t rainbow_shell(void) {

	// vars
	static int track_command = 0;
	static int on_display = 0;

	const char* cmd_buf[] = {
		"Read Moisture",
		"Cycle Pump",
		"Presets",
		"Set Time",
		"TFT Sleep",
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
				delay_ms(1000);		//second wait
				get_time();
			}
			delay_ms(1000);
		}

		if(read_button(1)){

			printk("button 2 clicked\n");	//debugging
			// move back a command
			if(track_command > 0){
				track_command--;
			}
			else{
				// wrap around
				track_command = 5;
			}
			on_display = 1;
		}

		if(read_button(2)){

			printk("button 3 clicked\n");	//debugging
			// do selected command
			printk("track_command: %d\n", track_command);	//debugging
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

		if(preset_flag){
			run_preset();
		}
		delay_ms(500);	//short debouncing delay
	}

	printk("Hey! You shouldn't be here!\n");	//debugging
}
