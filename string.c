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
		//uint8_t reg = 0x0F;

		uint8_t cmd[2] = { 0x0F, 0x10 };
		if(i2c_write(DEVICE_ADDRESS, cmd, 1) == -1){   // Point to moisture register
			printk("Error writing to moisture sensor.\n");
			uart_putc('\r');
		    uart_putc('\n');   // line feed
			return;
		}
		delay(0x220000);
        uint8_t buf[2];
		if(i2c_read(DEVICE_ADDRESS, buf, 2) == -1){
			printk("Error reading from moisture sensor.\n");
			uart_putc('\r');
		    uart_putc('\n');   // line feed
			return;
		}

		uint16_t moisture = (buf[0] << 8) | buf[1];
		//uint16_t moisture = (buf[0]) | buf[1];
		printk("\nMoisture reading: %u\n", moisture);

        uart_putc('\r');
		uart_putc('\n');   // line feed
    }

	// command to read moisture
	if(compare(string, "sense")){
		printk("\n");
		uint8_t cmd[2] = { 0x0F, 0x10 };
		uint8_t buf[2];
		if(i2c_write_read(DEVICE_ADDRESS, cmd, 2, buf, 2) == -1) {
        	printk("Error in combined I2C transfer.\n");
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

		uint8_t dummy = 0;
		int rv = i2c_write(0x36, &dummy, 1);

		if (rv == -1)
			printk("0x36 NACK\n");
		else
			printk("0x36 ACK\n");
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