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
#include "gpio.h"

#define TFT_RST  25     // reset

int preset = 1;

int do_command(int cmd){

    printk("Entering Command staging\n");       //debugging

    //vars
    uint32_t ch;
	char buffer[256];
	int input_len = 0;
	int command_prompt = 0;
	int command_active = 0;
    static int OnOff = 1;
    int quit = 0;
    static int track_preset = 0;
    static int on_display = 0;

    const char* preset_buf[] = {
		"preset 1",
        "preset 2"
	};

    switch(cmd){

        //asm("");        // avoid optimization

        // display moisture level
        case 0:
            printk("Entering Case 0:\n");       //debugging
            delay_ms(100);
            while(!read_button(2)){
                get_sensor_data();
                delay_ms(1000);
            }
            printk("Leaving Case 0:\n");       //debugging
            delay_ms(607);       // perfect delay
            break;

        case 1:

            printk("Entering Case 1:\n");       //debugging
            //activating pump
            pwm_set_duty(1024);

            //printing on lcd
            lcd_cmd(0x01); // clear
            lcd_set_cursor(0, 0);
            lcd_print("Pump Activated");

            delay_ms(12500);
            pwm_set_duty(0);
            printk("Leaving Case 1:\n");       //debugging
            break;

        case 2:
            
            printk("Entering Case 2:\n");       //debugging
            //changing moisture sensing preset

            //print to lcd
            lcd_cmd(0x01); // clear
            lcd_set_cursor(0, 0);
            lcd_print("Presets:");
            delay_ms(150);
            lcd_set_cursor(1, 0);
            lcd_print(preset_buf[0]);

            while(!quit){
                
                asm("");        // avoid optimization

                if(on_display){

                    // show current command
                    lcd_cmd(0x01); // clear
                    lcd_set_cursor(0, 0);
                    lcd_print("Menu:");
                    lcd_set_cursor(1, 0);
                    lcd_print(preset_buf[track_preset]);

                    //resetting on_display
                    on_display = 0;
                }

                if(read_button(1)){

                    printk("button 2 clicked\n");	//debugging
                    // move back a command
                    if(track_preset > 0){
                        track_preset--;
                    }
                    else{
                        // wrap around
                        track_preset = 1;
                    }
                    on_display = 1;
                }

                if(read_button(2)){

                    printk("button 3 clicked\n");	//debugging
                    // do selected preset
                    switch(track_preset){
                        case 0:
                            //setting preset 1
                            printk("Setting preset 1\n");
                            lcd_cmd(0x01); // clear
                            lcd_set_cursor(0, 0);
                            lcd_print("Setting Preset 1");
                            preset = 1;
                        
                            delay_ms(1000);
                            quit = 0;
                            break;

                        case 1:
                            //setting preset 2
                            printk("Setting preset 2\n");
                            lcd_cmd(0x01); // clear
                            lcd_set_cursor(0, 0);
                            lcd_print("Setting Preset 2");
                            preset = 2;
                            
                            delay_ms(1000);
                            quit = 0;
                            break;
                    }

                    on_display = 1;
                }

                if(read_button(3)){

                    printk("button 4 clicked\n");
                    // move forward a command
                    if(track_preset < 2){
                        track_preset++;
                    }
                    else{
                        // wrap around
                        track_preset = 0;
                    }
                    on_display = 1;
                }
		        delay_ms(500);	//short debouncing delay
            }
            
            quit = 0;
            printk("Leaving Case 2:\n");       //debugging
            break;

        case 3:

            printk("Entering Case 3:\n");       //debugging
            //setting system time
            clock_set();
            printk("Leaving Case 3:\n");       //debugging
            break;

        case 4:

            // put tft to sleep
            printk("toggling tft display\n");       // debugging
            gpio_write(TFT_RST, OnOff);     //toggle
            OnOff = 0;
            gpio_write(20, 0);      // turn off
            delay_ms(800);  //small delay
            while(!read_button(2));
            gpio_write(TFT_RST, OnOff);     //toggle
            lcd_init();
            OnOff = 1;
            delay_ms(800);
            break;

        case 5:

            printk("Entering Case 5:\n");       //debugging
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

    printk("Leaving command staging\n");        //debugging
    return 0;
}