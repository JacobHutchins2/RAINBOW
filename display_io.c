#include <stddef.h>
#include "serial.h"

int serial_write(const void *buf, size_t len){
    return uart_write(buf, len);    // writes to serial port
}

int serial_read(void *buf, size_t len){
    int i;
    char *buffer = buf;
    //reading from serial port
    for(i = 0; i < len; i++){
        buffer[i] = uart_getc_noblock();
        if(buffer[i] <= 0){
            break;
        }
    }
    return i;
}