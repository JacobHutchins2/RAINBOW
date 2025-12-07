#include <stddef.h>
#include <stdint.h>
#include "mmio.h"
#include "bcm2835_addr.h"
#include "delay.h"
#include "string.h"
#include "i2c.h"
#include "printk.h"
#include "shell_commands.h"

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

int parse_input(char *string) {

	if (compare(string,"help")) {		//help command
		printk("\nSupported commands:\n");
		printk("clear: clears the screen\n");
		printk("scan: runs an i2cdetect\n");
		putchar('\r');
		putchar('\n');   // line feed
	}

	if(compare(string, "clear")){
		printk(ESC "[2J" ESC "[H"); 				// Send ANSI escape sequence to clear screen
		putchar('\r');
		putchar('\n');   // line feed
	}

    if(compare(string, "scan")){
        i2c_scan();     // looking at connected devices
        putchar('\r');
		putchar('\n');   // line feed
    }

	else{
		putchar('\r');   // carriage return		//new line when not a command
		putchar('\n');   // line feed
	}
	return 0;
}