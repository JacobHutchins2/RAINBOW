#include <stddef.h>
#include <stdint.h>

#include "shell.h"
#include "serial.h"
#include "mmio.h"
#include "bcm2835_addr.h"
#include "delay.h"
#include "i2c.h"
#include "string.h"
#include "shell_commands.h"
#include "printk.h"

uint32_t rainbow_shell(void) {

	uint32_t ch;
	char buffer[256];
	int input_len = 0;
	
	/* Enter the "rainbow" */

	while(1) {

		printk("--> ");	// printing shell prompt

		while(1) {
			ch = getchar();

			/* handle Enter being pressed */
			if (ch=='\n' || ch=='\r') {
				
				buffer[input_len] = '\0';		 //null terminated
				parse_input(buffer);
				
				buffer[0] = '\0';				 //clear buffer
				input_len = 0;
				break;
			}

			else{
				putchar(ch);
				if (input_len < (int)(sizeof(buffer) - 1)) {		// Save to buffer if space remains
					buffer[input_len++] = ch;						// otherwise ignore excess characters
				}
			}
		}
	}
}
