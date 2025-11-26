#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>
#include "mmio.h"
#include "bcm2835_addr.h"
#include "delay.h"
#include "serial.h"

#define MAX_PRINT_SIZE 256

int printk(char *string,...){
    va_list ap;

    char buffer[MAX_PRINT_SIZE];
	char int_buffer[10];
	int int_pointer=0;

	int buffer_pointer=0;
	int i;
	int x;
	uint32_t hex;
	char *s;

	va_start(ap, string);

	while(1) {
		if (*string==0) break;

		if (*string=='%') {
			string++;
			if (*string=='d') {
				string++;
				x=va_arg(ap, int);
				int_pointer=9;
				do {
					int_buffer[int_pointer]=(x%10)+'0';
					int_pointer--;
					x/=10;
				} while(x!=0);
				for(i=int_pointer+1;i<10;i++) {
					buffer[buffer_pointer]=int_buffer[i];
					buffer_pointer++;
				}

			}
			else if (*string=='x') {
				string++;
				hex=va_arg(ap, uint32_t);
				int_pointer=9;
				do {
					if ((hex%16)<10) {
						int_buffer[int_pointer]=(hex%16)+'0';
					}
					else {
						int_buffer[int_pointer]=(hex%16)-10+'a';
					}
					int_pointer--;
					hex/=16;
				} while(hex!=0);
				for(i=int_pointer+1;i<10;i++) {
					buffer[buffer_pointer]=int_buffer[i];
					buffer_pointer++;
				}
			}
			else if (*string=='s') {
				string++;
				s=va_arg(ap, char *);
				while(*s!=0) {
					buffer[buffer_pointer]=*s;
					buffer_pointer++;
					s++;
				}
			}

		}
		else {
			buffer[buffer_pointer]=*string;
			buffer_pointer++;
			string++;
		}
		if (buffer_pointer==MAX_PRINT_SIZE-1) break;
	}

	va_end(ap);

	uart_write(buffer,buffer_pointer);      // change to whatever display will be used

	return buffer_pointer;
}