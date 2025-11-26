#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>
#include "syscalls.h"
#include "shell_commands.h"

int putchar(int ch) {

	uint32_t result;

	char buffer[1];

	buffer[0]=ch;

	result=syscall3(SYSCALL_WRITE,STDOUT,(uint32_t)buffer,1);

	return result;

}

int getchar(void) {

	int32_t ch=0;
	char buffer[1];

	while(1) {
		/* Nonblocking for now */
		syscall3(SYSCALL_READ,STDIN,(uint32_t)buffer,1);

		ch=buffer[0];

		if (ch>0) break;

	}
	return ch;

}
