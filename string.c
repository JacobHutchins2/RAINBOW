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
#include "spi.h"
#include "tft.h"

#define ESC "\033"			// ANSI Escape character
#define DEVICE_ADDRESS 0x36  // I2C address for Adafruit Stemma Soil Sensor

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
		printk("test: fills tft screen with red color\n");
		uart_putc('\r');
		uart_putc('\n');   // line feed
	}

	if(compare(string, "clear")){
		printk(ESC "[2J" ESC "[H"); 				// Send ANSI escape sequence to clear screen
		uart_putc('\r');
		uart_putc('\n');   // line feed
	}

    if(compare(string, "scan")){
		printk("\n");
        i2c_scan();     // looking at connected devices
        uart_putc('\r');
		uart_putc('\n');   // line feed
    }


	/*
	Adafruit Stemma Soil Sensor Registers
	https://learn.adafruit.com/adafruit-stemma-soil-sensor-i2c-capacitive-moisture-sensor/faq?
	*/
	if(compare(string, "read")){
		printk("\n");
		get_sensor_data();

        uart_putc('\r');
		uart_putc('\n');   // line feed
    }

	// command to read moisture
	if(compare(string, "blank")){
		printk("\n");
		
		uart_putc('\r');
		uart_putc('\n');   // line feed
	}

	if(compare(string, "test")){
		printk("\n");
        tft_fill_color(0xF800); // fill screen with red
		/*tft_write_cmd(0x2C); // Memory Write
		uint8_t red[2] = { 0xF8, 0x00 }; // RED
		for (int i = 0; i < 240*320; i++) {
			tft_write_buf(red, 2);
		}*/
		while(1);
        uart_putc('\r');
		uart_putc('\n');   // line feed
    }

	if (compare(string, "id")) {
    printk("\n");

    uint8_t reg = 0x03;
    i2c_write(0x70, &reg, 1);

    delay(1500);

    uint8_t id;
    int r = i2c_read(0x70, &id, 1);

    printk("ID read returned %d\n", r);
    printk("ID: 0x%02X\n", id);

    uart_putc('\r');
    uart_putc('\n');
	}

	else{
		uart_putc('\r');   // carriage return		//new line when not a command
		uart_putc('\n');   // line feed
	}
	//return 0;
}