#include <stddef.h>
#include <stdint.h>
#include "bcm2835_addr.h"
#include "mmio.h"
#include "delay.h"
#include "serial.h"

void uart_init(void){
    uint32_t old;
    bcm2835_write(UART0_CR, 0x0);       // disable UART
    old = bcm2835_read(GPIO_FSEL1);
    old &= ~(0x7 << 12);
    old |= (4 << 12);
    old &= ~(0x7 << 15);
    old |= (4 << 15);
    bcm2835_write(GPIO_FSEL1, old);

    bcm2835_write(GPIO_PUD, GPIO_PUD_DISABLE);
    delay(150);

    bcm2835_write(GPIO_PUDCLK0, 0x0);
    bcm2835_write(UART0_IMSC, 0);       // masking interrupts
    bcm2835_write(UART0_ICR, 0x7FF);    // clear pending interrupts
    bcm2835_write(UART0_IBRD, 26);
    bcm2835_write(UART0_FBRD, 3);
    bcm2835_write(UART0_LCRH, UART0_LCRH_FEN | UART0_LCRH_WLEN_8BIT);
    bcm2835_write(UART0_CR, UART0_CR_UARTEN | UART0_CR_TXE | UART0_CR_RXE);         // UART enable receive/transmit
}

void uart_putc( unsigned char byte){
    while(bcm2835_read(UART0_FR) & UART0_FR_TXFF){}
    bcm2835_write(UART0_DR, byte);      // writes byte to data reg
}

unsigned char uart_getc(void){
    while(bcm2835_read(UART0_FR) & UART0_FR_RXFE){}     // waiting for FIFO 
    return bcm2835_read(UART0_DR);      // receiving data from uart
}

size_t uart_write(const unsigned char *buffer, size_t size){
    size_t i;

    for(i = 0; i < size; i++){
        uart_putc(buffer[1]);
        if(buffer[i] == '\n'){
            uart_putc('\r');
        }
    }
    return i;
}