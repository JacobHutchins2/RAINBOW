#include <stddef.h>
#include <stdint.h>

#include "shell.h"
#include "uart.h"
#include "mmio.h"
#include "bcm2835_addr.h"
#include "delay.h"

uint32_t rainbow_shell(void) {
    uint32_t ch;
    char buffer[256];
    int input_len = 0;

    /* Enter our "shell" */
    uart_puts("\r\nWelcome to the Rainbow Shell!\r\nType 'help' for a list of commands.\r\n> ");

    while (1) {
        ch = uart_getc();

        if (ch == '\r' || ch == '\n') { // Enter key
            buffer[input_len] = '\0'; // Null-terminate the string
            uart_puts("\r\n"); // New line

            // Process the input command
            if (strcmp(buffer, "help") == 0) {
                uart_puts("Supported commands:\r\n");
                uart_puts("  print: prints hello world\r\n");
                uart_puts("  clear: clears the screen\r\n");
                uart_puts("  time: shows uptime in seconds\r\n");
            } else if (strcmp(buffer, "print") == 0) {
                uart_puts("Hello World!\r\n");
            } else if (strcmp(buffer, "clear") == 0) {
                uart_puts("\033[2J\033[H"); // ANSI escape sequence to clear screen
            } else if (strcmp(buffer, "time") == 0) {
                uint32_t ticks = get_system_time(); // Assume this function gets system uptime
                char time_str[50];
                sprintf(time_str, "Uptime: %d seconds\r\n", ticks);
                uart_puts(time_str);
            } else {
                uart_puts("Unknown command. Type 'help' for a list of commands.\r\n");
            }

            // Reset for next input
            input_len = 0;
            uart_puts("> "); // Prompt
        } else if (ch == '\b' || ch == 127) { // Backspace key
            if (input_len > 0) {
                input_len--;
                uart_puts("\b \b"); // Erase character from terminal
            }
        } else if (input_len < sizeof(buffer) - 1) { // Regular character
            buffer[input_len++] = (char)ch;
            uart_send(ch); // Echo character
        }
    }

    return 0;
}