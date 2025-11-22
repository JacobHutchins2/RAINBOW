#include <stddef.h>
#include <stdint.h>
#include "bcm2835_periph.h"
#include "mmio.h"
#include "delay.h"
#include "serial.h"

void uart_init(void){
    uint32_t old;
    bcm2835_write(UART0_CR, 0x0);       // disable UART
    old = bcm2835_read(GPIO_SEL1);
    old &= ~(0x7 << 12);
    old |= (4 << 12);
    old &= ~(0x7 << 15);
    old |= (4 << 15);
    bcm2835_write(GPIO_FSEL1, old);

    bcm2835_write(GPIO_PUD, GPIO_PUD_DISABLE);
}