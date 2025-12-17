#include <stdint.h>
#include "bcm2835_addr.h"
#include "mmio.h"
#include "printk.h"
#include "delay.h"
#include "spi.h"
#include "gpio.h"
#include "printk.h"
#include "font.h"

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
    bcm2835_write(SPI_CS, SPI_CS_CLEAR_RX | SPI_CS_CLEAR_TX | SPI_CS_SELECT0);

    // Set SPI clock
    bcm2835_write(SPI_CLK, 64);  // 250 MHz / 64 = ~8 MHz --> 100kHz
    // Mode 0 explicitly (CPOL=0, CPHA=0)
    uint32_t cs = bcm2835_read(SPI_CS);
    cs &= ~(SPI_CS_CPOL_HIGH | SPI_CS_CPHA_BEG);
    bcm2835_write(SPI_CS, cs);

    delay(0x10000);
}

#if 0
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
#endif


void spi_write_stream(const uint8_t *buf, int len) {
    uint32_t cs;

    // Clear FIFOs, select CS0, start transfer
    cs = bcm2835_read(SPI_CS);
    cs |= SPI_CS_CLEAR_TX | SPI_CS_CLEAR_RX | SPI_CS_SELECT0 | SPI_CS_TRANSFER_ACTIVE;
    cs &= ~(SPI_CS_CPOL_HIGH | SPI_CS_CPHA_BEG);
    bcm2835_write(SPI_CS, cs);

    for (int i = 0; i < len; i++) {

        // Wait for TX FIFO space
        while (!(bcm2835_read(SPI_CS) & SPI_CS_TXD_OPEN));

        // Write byte
        bcm2835_write(SPI_FIFO, buf[i]);

        // Drain RX FIFO (ILI9341 always returns a byte)
        while (bcm2835_read(SPI_CS) & SPI_CS_RXD_USED) {
            bcm2835_read(SPI_FIFO);
        }
    }

    // Wait for transfer complete
    while (!(bcm2835_read(SPI_CS) & SPI_CS_DONE_DONE));

    // Stop transfer
    cs = bcm2835_read(SPI_CS);
    cs &= ~SPI_CS_TRANSFER_ACTIVE;
    bcm2835_write(SPI_CS, cs);
}


/*
These commands are specified starting on page 150 of the BCM2835 ARM Peripherals pdf
*/
// Initialize GPIOs for TFT display control
void tft_gpio_init(void){
    printk("tft gpio initializing\n");  // debugging
    gpio_set_output(TFT_DC);
    gpio_set_output(TFT_RST);
    gpio_set_output(TFT_BL);
    gpio_write(TFT_BL, 1);   // Turn on backlight
    gpio_write(13, 1);   // For controlling Q2 Mosfet connected to ground
    gpio_write(TFT_RST, 1);
    delay_ms(50);
    gpio_write(TFT_RST, 0);
    delay_ms(150);
    
}

// Write command to TFT display
void tft_write_cmd(uint8_t cmd) {
    //printk("Entering tft write command\n");     // debugging
    gpio_write(TFT_DC, 0);          // command mode
   
    spi_write_stream(&cmd, 1);
}

// Write data to TFT display
void tft_write_data(uint8_t data) {
    //printk("Entering tft write data\n");     // debugging
    gpio_write(TFT_DC, 1);          // data mode

    spi_write_stream(&data, 1);
    
}


// Write buf data to TFT display
void tft_write_buf(uint8_t *buf, int len) {
    gpio_write(TFT_DC, 1);
    spi_write_stream(buf, len);

}

void tft_init(void) {
    printk("Entering tft init.\n");      // debugging
    tft_gpio_init();
    tft_write_cmd(0x01); // software reset
    delay_ms(150);

    tft_write_cmd(0x28); // display off
    tft_write_cmd(0x3A); 
    tft_write_data(0x55); // 16-bit color
    tft_write_cmd(0x36); 
    tft_write_data(0x48); // memory access ctrl
    
    tft_write_cmd(0x11); // sleep out
    delay_ms(150);

    tft_write_cmd(0x29); // Display on
    delay_ms(100);
}

/* ===== Set draw window ===== */
void tft_set_window(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {

    tft_write_cmd(0x2A);
    tft_write_data(x0 >> 8); 
    tft_write_data(x0);
    tft_write_data(x1 >> 8); 
    tft_write_data(x1);

    tft_write_cmd(0x2B);
    tft_write_data(y0 >> 8); 
    tft_write_data(y0);
    tft_write_data(y1 >> 8); 
    tft_write_data(y1);

    tft_write_cmd(0x2C);
}

/* ===== Fill screen ===== */
void tft_fill(uint16_t color) {

    tft_set_window(0, 0, 239, 319);

    uint8_t px[2] = { color >> 8, color & 0xFF };

    gpio_write(TFT_DC, 1);
    for (int i = 0; i < 240 * 320; i++)
        spi_write_stream(px, 2);
}

void tft_draw_char(uint16_t x, uint16_t y,
                   char c,
                   uint16_t fg,
                   uint16_t bg) {

    if (c < FONT_FIRST || c > FONT_LAST)
        c = '?';

    const uint8_t *glyph = FONTS[c - FONT_FIRST];

    tft_set_window(x, y, x + FONT_WIDTH - 1, y + FONT_HEIGHT - 1);

    uint8_t px[2];
    gpio_write(TFT_DC, 1);

    for (int row = 0; row < FONT_HEIGHT; row++) {
        for (int col = 0; col < FONT_WIDTH; col++) {

            uint16_t color =
                (glyph[col] & (1 << row)) ? fg : bg;

            px[0] = color >> 8;
            px[1] = color & 0xFF;
            spi_write_stream(px, 2);
        }
    }
}

void tft_draw_string(uint16_t x, uint16_t y, const char *s, uint16_t fg, uint16_t bg) {

    while (*s) {
        tft_draw_char(x, y, *s++, fg, bg);
        x += FONT_WIDTH + 1;
    }
}