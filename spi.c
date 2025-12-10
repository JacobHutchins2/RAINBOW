#include <stdint.h>
#include "bcm2835_addr.h"
#include "mmio.h"
#include "printk.h"
#include "delay.h"
#include "spi.h"

void spi_init(void) {

    // GPIO MAP FROM BCM2835 ARM PDF pg 102
    // 
    // GPIO 8 = CE0
    // GPIO 9 = MISO
    // GPIO 10 = MOSI
    // GPIO 11 = SCLK

    uint32_t sel0 = bcm2835_read(GPIO_FSEL0);
    uint32_t sel1 = bcm2835_read(GPIO_FSEL1);

    // GPIO 8, 9 are in FSEL0
    sel0 &= ~((7 << 24) | (7 << 27));    // clear
    sel0 |=  (4 << 24) | (4 << 27);      // ALT0

    // GPIO 10,11 are in FSEL1
    sel1 &= ~((7 << 0) | (7 << 3));                  // clear
    sel1 |=  (4 << 0) | (4 << 3);                    // ALT0

    bcm2835_write(GPIO_FSEL0, sel0);
    bcm2835_write(GPIO_FSEL1, sel1);

    // Reset SPI 
    bcm2835_write(SPI_CS, SPI_CS_CLEAR);             // clear Tx/Rx FIFOs

    // Set SPI clock
    bcm2835_write(SPI_CLK, 32);  // 250 MHz / 64 = ~8 MHz
    bcm2835_write(SPI_CS, SPI_CS_CLEAR | SPI_CS_SELECT0 | 0);         // reset SPI CS

}

void spi_write(uint8_t byte) {

    // Wait for TX FIFO space
    while(!(bcm2835_read(SPI_CS) & SPI_CS_TXD_OPEN));

    bcm2835_write(SPI_FIFO, byte);

    // Start transfer
    uint32_t cs = bcm2835_read(SPI_CS);
    bcm2835_write(SPI_CS, cs | SPI_CS_TRANSFER_ACTIVE);

    // Wait for DONE
    while(!(bcm2835_read(SPI_CS) & SPI_CS_DONE_DONE));

    // Clear TA bit
    cs = bcm2835_read(SPI_CS);
    bcm2835_write(SPI_CS, cs & ~SPI_CS_TRANSFER_ACTIVE);
}
