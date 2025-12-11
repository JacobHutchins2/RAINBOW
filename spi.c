#include <stdint.h>
#include "bcm2835_addr.h"
#include "mmio.h"
#include "printk.h"
#include "delay.h"
#include "spi.h"
#include "gpio.h"
#include "printk.h"

#define TFT_DC   24     // data command
#define TFT_RST  25     // reset
#define TFT_BL    5     // backlight


void spi_init(void) {

    // GPIO MAP FROM BCM2835 ARM PDF pg 102
    // 
    // GPIO 8 = CE0
    // GPIO 9 = MISO
    // GPIO 10 = MOSI
    // GPIO 11 = SCLK

    // GPIO 8, 9 are in FSEL0
    gpio_set_alt(8, 4);
    gpio_set_alt(9, 4);

    // GPIO 10,11 are in FSEL1
    gpio_set_alt(10, 4);
    gpio_set_alt(11, 4);


    // Reset SPI 
    bcm2835_write(SPI_CS, SPI_CS_CLEAR);             // clear Tx/Rx FIFOs

    // Set SPI clock
    bcm2835_write(SPI_CLK, 32);  // 250 MHz / 64 = ~8 MHz
    //bcm2835_write(SPI_CS, SPI_CS_CLEAR | SPI_CS_SELECT0 | 0);         // reset SPI CS
    bcm2835_write(SPI_CS, SPI_CS_SELECT0 | SPI_CS_TRANSFER_ACTIVE);     // start SPI
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

/*
These commands are specified starting on page 150 of the BCM2835 ARM Peripherals pdf
*/
// Initialize GPIOs for TFT display control
void tft_gpio_init(void){
    printk("tft gpio initializing\n");  // debugging
    gpio_set_output(TFT_DC);
    gpio_set_output(TFT_RST);
    //gpio_write(TFT_BL, 1);   // Turn on backlight
    gpio_write(13, 1);   // For controlling Q2 Mosfet connected to ground
}

// Write command to TFT display
void tft_write_cmd(uint8_t cmd) {
    //printk("Entering tft write command\n");     // debugging
    gpio_write(8, 0);          // command mode
    bcm2835_write(SPI_FIFO, cmd);   // write byte
    while (!(bcm2835_read(SPI_CS) & SPI_CS_DONE_DONE)){
        //printk("Am I Stuck here forever?\n");       // debugging
    }
    //printk("No. Leaving tft write command\n");      // debugging
}

// Write data to TFT display
void tft_write_data(uint8_t data) {
    printk("Entering tft write data\n");     // debugging
    gpio_write(8, 1);          // data mode
    bcm2835_write(SPI_FIFO, data);
    while (!(bcm2835_read(SPI_CS) & SPI_CS_DONE_DONE));
    printk("Exiting tft write data\n");      // debugging
}


// Write buf data to TFT display
void tft_write_buf(uint8_t *buf, int len) {
    gpio_write(8, 1);
    for(int i=0;i<len;i++){
        while (!(bcm2835_read(SPI_CS) & SPI_CS_TXD_OPEN));      // wait for TX FIFO space
        bcm2835_write(SPI_FIFO, buf[i]);        // write byte to FIFO 
    }
    while (!(bcm2835_read(SPI_CS) & SPI_CS_DONE_DONE));
}

void tft_init(void) {
    printk("Entering tft init.\n");      // debugging
    tft_write_cmd(0x01); // software reset
    delay(0x20000);

    tft_write_cmd(0x28); // display off
    tft_write_cmd(0x3A); 
    tft_write_data(0x55); // 16-bit color
    tft_write_cmd(0x36); 
    tft_write_data(0x48); // memory access ctrl
    
    tft_write_cmd(0x11); // sleep out
    delay(0x20000);

    tft_write_cmd(0x29); // Display on
}