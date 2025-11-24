#include <stddef.h>
#include <stdint.h>
#include "mmio.h"
#include "bcm2835_addr.h"
#include "delay.h"
#include "string.h"

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

static int parse_input(char *string) {

	if (compare(string,"help")) {		//help command
		printf("\nSupported commands:\n");
		printf("  print: prints hello world\n");
		printf("  clear: clears the screen\n");
		printf("  time: shows uptime in seconds\n");
		putchar('\r');
		putchar('\n');   // line feed
	}

	if(compare(string, "clear")){
		printf(ESC "[2J" ESC "[H"); 				// Send ANSI escape sequence to clear screen
		putchar('\r');
		putchar('\n');   // line feed
	}

	else{
		putchar('\r');   // carriage return		//new line when not a command
		putchar('\n');   // line feed
	}
	return 0;
}