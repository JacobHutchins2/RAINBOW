#include <stdint.h>
#include "ui.h"
#include "delay.h"
#include "i2c.h"
#include "pwm.h"
#include "timer.h"
#include "buttons.h"
#include "serial.h"
#include "string.h"
#include "printk.h"

int do_command(int cmd){

    printk("Entering Command staging\n");       //debugging

    //vars
    uint32_t ch;
	char buffer[256];
	int input_len = 0;
	int command_prompt = 0;
	int command_active = 0;

    switch(cmd){

        //asm("");        // avoid optimization

        // display moisture level
        case 0:
            printk("Entering Case 0:\n");       //debugging

            while(!read_button(2)){
                get_sensor_data();
            }
            printk("Leaving Case 0:\n");       //debugging
            delay_ms(67);       // perfect delay
            break;

        case 1:

            printk("Entering Case 1:\n");       //debugging
            //activating pump
            pwm_set_duty(1024);

            //printing on lcd
            lcd_cmd(0x01); // clear
            lcd_set_cursor(0, 0);
            lcd_print("Pump Activated");

            delay_ms(150);
            pwm_set_duty(0);
            printk("Leaving Case 1:\n");       //debugging
            break;

        case 2:
            
            printk("Entering Case 2:\n");       //debugging
            //changing moisture sensing preset
            // syscalls...

            printk("Leaving Case 2:\n");       //debugging
            break;

        case 3:

            printk("Entering Case 3:\n");       //debugging
            //setting system time
            clock_set();
            printk("Leaving Case 3:\n");       //debugging
            break;

        case 4:

            printk("Entering Case 4:\n");       //debugging
            //using commands on serial terminal (for debugging purposes)
            while(1){

                if(command_prompt == 0){
                    printk("--> ");	// printing shell prompt
                    command_prompt = 1;
                }
                //printk("I am here.\n");		//debuggging
                // detects button press
                if(read_button(1)){
                    pwm_set_duty(256);   // 25% duty cycle
                    printk("\nButton 1 pressed: Set PWM to .25 duty cycle.\n");
                    delay_ms(300); // Simple debounce delay
                    while(!button_pressed(0)); // wait for button release
                    command_prompt = 0;
                    pwm_set_duty(0);   // shut pwm off
                }
                if(read_button(2)){
                    printk("\nButton 2 pressed.\n");
                    printk("Enter command.\n");
                    command_active = 1;
                    
                    delay_ms(200); // Simple debounce delay
                }

                if(read_button(0)) break;   //leave debugging terminal

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
    }
    return 0;
}