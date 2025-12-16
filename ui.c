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

    }

}