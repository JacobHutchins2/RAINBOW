#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>
#include "display_io.h"

#define MAX_PRINT_SIZE 256

int printk(char *string, ...) {
    va_list ap;
    char buffer[MAX_PRINT_SIZE];    // final output buffer
    char int_buffer[10];           // temporary buffer for integer conversions
    int int_pointer = 0;
    int buffer_pointer = 0;
    int i;
    int x;
    uint32_t hex;
    char *s;

    va_start(ap, string);

    while (*string) {
        if (*string == '%') {
            string++;
            int pad = 0;  // for optional zero-padding like %03u

            // check for zero-padding number
            if (*string == '0') {
                string++;
                pad = 0;
                while (*string >= '0' && *string <= '9') {
                    pad = pad * 10 + (*string - '0');
                    string++;
                }
            }

            switch (*string) {
                case 'd': // signed decimal
                    string++;
                    x = va_arg(ap, int);
                    int_pointer = 9;
                    int is_negative = 0;
                    if (x < 0) {
                        is_negative = 1;
                        x = -x;
                    }
                    do {
                        int_buffer[int_pointer--] = (x % 10) + '0';
                        x /= 10;
                    } while (x != 0);
                    if (is_negative)
                        int_buffer[int_pointer--] = '-';
                    for (i = int_pointer + 1; i < 10; i++)
                        buffer[buffer_pointer++] = int_buffer[i];
                    break;

                case 'u': // unsigned decimal
                    string++;
                    x = va_arg(ap, unsigned int);
                    int_pointer = 9;
                    do {
                        int_buffer[int_pointer--] = (x % 10) + '0';
                        x /= 10;
                    } while (x != 0);
                    // apply padding if needed
                    while ((9 - int_pointer - 1) < pad) {
                        int_buffer[int_pointer--] = '0';
                    }
                    for (i = int_pointer + 1; i < 10; i++)
                        buffer[buffer_pointer++] = int_buffer[i];
                    break;

                case 'x': // hexadecimal
                    string++;
                    hex = va_arg(ap, uint32_t);
                    int_pointer = 9;
                    do {
                        if ((hex % 16) < 10)
                            int_buffer[int_pointer] = (hex % 16) + '0';
                        else
                            int_buffer[int_pointer] = (hex % 16) - 10 + 'a';
                        hex /= 16;
                        int_pointer--;
                    } while (hex != 0);
                    for (i = int_pointer + 1; i < 10; i++)
                        buffer[buffer_pointer++] = int_buffer[i];
                    break;

                case 'X': {  // uppercase hex with optional zero padding
                    string++;
                    unsigned int val = va_arg(ap, unsigned int);
                    int_pointer = 9;

                    do {
                        uint32_t digit = val & 0xF;
                        if (digit < 10) int_buffer[int_pointer] = digit + '0';
                        else int_buffer[int_pointer] = digit - 10 + 'A';
                        int_pointer--;
                        val >>= 4;
                    } while (val != 0);

                    // zero padding width `pad`
                    while ((9 - int_pointer - 1) < pad) {
                        int_buffer[int_pointer--] = '0';
                    }

                    for (i = int_pointer + 1; i < 10; i++)
                        buffer[buffer_pointer++] = int_buffer[i];

                    break;
                }

                
                case 's': // string
                    string++;
                    s = va_arg(ap, char *);
                    while (*s) buffer[buffer_pointer++] = *s++;
                    break;

                default: // unknown, print as-is
                    buffer[buffer_pointer++] = *string++;
                    break;
            }
        } else {
            buffer[buffer_pointer++] = *string++;
        }

        if (buffer_pointer >= MAX_PRINT_SIZE - 1)
            break; // prevent overflow
    }

    va_end(ap);

    serial_write(buffer, buffer_pointer);

    return buffer_pointer;
}