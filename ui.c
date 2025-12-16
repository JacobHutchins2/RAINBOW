#include <stdint.h>
#include "ui.h"
#include "delay.h"
#include "i2c.h"
#include "pwm.h"
#include "timer.h"

int do_command(int cmd){

    switch(cmd){

        // display moisture level
        case 0:
            get_sensor_data();

            while(!read_button(2))
            break;

        case 1:

            //activating pump
            pwm_set_duty(1024);

            //printing on lcd
            lcd_cmd(0x01); // clear
            lcd_set_cursor(0, 0);
            lcd_print("Pump Activated");

            delay_ms(150);
            pwm_set_duty(0);
            break;

        case 2:
            
            //changing moisture sensing preset
            // syscalls...

            break;

        case 3:

            //setting system time
            clock_set();
            break;

        case 4:

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

}