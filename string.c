#include <stddef.h>
#include <stdint.h>
#include "mmio.h"
#include "bcm2835_addr.h"
#include "delay.h"
#include "string.h"
#include "i2c.h"
#include "printk.h"
//#include "shell_commands.h"
#include "serial.h"

#define ESC "\033"			// ANSI Escape character

                // Manual string comparison function
/*============================================================================*/
int compare(char *s1, char *s2) {
    while (*s1 && *s2) {
        if (*s1 != *s2){
            return 0;  // Characters don't match
        }
        s1++;
        s2++;
    }
    return (*s1 == '\0' && *s2 == '\0');  
}
/*============================================================================*/

void parse_input(char *string) {

	if (compare(string,"help")) {		//help command
		printk("\nSupported commands:\n");
		printk("clear: clears the screen\n");
		printk("scan: runs an i2cdetect\n");
		printk("read: reads moisture sensor value\n");
		uart_putc('\r');
		uart_putc('\n');   // line feed
	}

	if(compare(string, "clear")){
		printk(ESC "[2J" ESC "[H"); 				// Send ANSI escape sequence to clear screen
		uart_putc('\r');
		uart_putc('\n');   // line feed
	}

    if(compare(string, "scan")){
        i2c_scan();     // looking at connected devices
        uart_putc('\r');
		uart_putc('\n');   // line feed
    }

	if(compare(string, "read")){
		printk("\n");
		uint8_t reg = 0x0F;
		if(i2c_write(0x36, &reg, 1) == -1){   // Point to moisture register
			printk("Error writing to moisture sensor.\n");
			uart_putc('\r');
		    uart_putc('\n');   // line feed
			return;
		}
		//delay(2000);
        uint8_t buf[2];
		if(i2c_read(0x36, buf, 2) == -1){
			printk("Error reading from moisture sensor.\n");
			uart_putc('\r');
		    uart_putc('\n');   // line feed
			return;
		}

		uint16_t moisture = (buf[0] << 8) | buf[1];
		printk("\nMoisture reading: %u\n", moisture);

        uart_putc('\r');
		uart_putc('\n');   // line feed
    }

	if(compare(string, "test")){
		printk("\n");
        uint8_t reg = 0x00;
		int r = i2c_write(0x36, &reg, 1);
		printk("i2c_write returned %d\n", r);

        uart_putc('\r');
		uart_putc('\n');   // line feed
    }

	else{
		uart_putc('\r');   // carriage return		//new line when not a command
		uart_putc('\n');   // line feed
	}
	//return 0;
}