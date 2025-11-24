#include <stdint.h>

#include "i2c.h"
#include "bcm2835.h"
#include "mmio.h"
#include "printk.h"

void i2c_init(void) {
    // Set to ALT0 for I2C1 
	uint32_t old;

	// GPIO 2 
	old = bcm2835_read(GPIO_FSEL0);
	old &= ~(7<<6);  // Clear bits 8-6 
	old |= (4<<6);   // Set to ALT0 
	bcm2835_write(GPIO_FSEL0, old);

	// GPIO 3 
	old = bcm2835_read(GPIO_FSEL0);
	old &= ~(7<<9);  // Clear bits 11-9 
	old |= (4<<9);   // Set to ALT0 
	bcm2835_write(GPIO_FSEL0, old);

	bcm2835_write(GPIO_GPPUD, GPIO_PUD_PULLUP);	// Enable pull-up resistors
	usleep(150);
	bcm2835_write(GPIO_PUDCLK0, (1<<2)|(1<<3));	// Clock the control signal into GPIO 2 and 3
	usleep(150);
	bcm2835_write(GPIO_PUD, GPIO_PUD_DISABLE);	// Remove the control signal
	bcm2835_write(GPIO_PUDCLK0, 0);	     // Remove the clock
    
    // Set I2C clock divider for 100kHz (250MHz clock)
    bcm2835_write(I2C_DIV, 2500);
    // Enable I2C
    bcm2835_write(I2C_C, I2C_C_CLEAR); // Clear FIFO
    bcm2835_write(I2C_C, I2C_C_I2CEN); 

    printk("The program made it here!");        // debugging
    return 0;
}

int i2c_write(uint8_t addr, const uint8_t *data, uint32_t len) {
    
    bcm2835_write(I2C_S, I2C_S_CLKT | I2C_S_ERR | I2C_S_DONE); // Clear status flags
    bcm2835_write(I2C_A, addr); // Set slave address
    bcm2835_write(I2C_DLEN, len); // Set data length
    bcm2835_write(I2C_C, I2C_C_CLEAR); // Clear FIFO

    // Write to FIFO
    for (int i = 0; i < len; i++) {
        bcm2835_write(I2C_FIFO, data[i]);
    }

    bcm2835_write(I2C_C, I2C_C_I2CEN | I2C_C_ST); // Start transfer
    while (1) {
        uint32_t status = bcm2835_read(I2C_S);
        if (status & I2C_S_DONE) {
            break; // Transfer complete
        }
        if (status & (I2C_S_ERR | I2C_S_CLKT)) {
            return -1; // Error occurred
        }
    }
    return 0;
}

int i2c_read(uint8_t addr, uint8_t *data, uint32_t len) {
    
    bcm2835_write(I2C_S, I2C_S_CLKT | I2C_S_ERR | I2C_S_DONE); // Clear status flags
    bcm2835_write(I2C_A, addr); // Set slave address
    bcm2835_write(I2C_DLEN, len); // Set data length
    bcm2835_write(I2C_C, I2C_C_CLEAR | I2C_C_READ); // Clear FIFO and set to read mode

    bcm2835_write(I2C_C, I2C_C_I2CEN | I2C_C_ST); // Start transfer
    uint32_t index = 0;
    while (1) {
        uint32_t status = bcm2835_read(I2C_S);
        while ((status & I2C_S_RXR) && (index < len)) {
            data[index++] = bcm2835_read(I2C_FIFO); // Read from FIFO
            status = bcm2835_read(I2C_S);
        }
        if (status & I2C_S_DONE) {
            break; // Transfer complete
        }
        if (status & (I2C_S_ERR | I2C_S_CLKT)) {
            return -1; // Error occurred
        }
    }
    return 0;
}

int i2c_scan(void) {
    for (uint8_t addr = 0x03; addr <= 0x77; addr++) {
        //bcm2835_write(I2C_S, I2C_S_CLKT | I2C_S_ERR | I2C_S_DONE); // Clear status flags
        bcm2835_write(I2C_C, I2C_C_CLEAR);      // clearing FIFO
        bcm2835_write(I2C_A, addr); // Set slave address
        bcm2835_write(I2C_DLEN, 1); // data to send
        bcm2835_write(I2C_S, I2C_S_CLKT | I2C_S_ERR | I2C_S_DONE); //
        bcm2835_write(I2C_FIFO, 0); // Dummy data

        bcm2835_write(I2C_C, I2C_C_I2CEN | I2C_C_ST); // Start transfer
        while (1) {
            uint32_t status = bcm2835_read(I2C_S);
            if (status & I2C_S_DONE) {
                // No error, device acknowledged
                // ACK occurred if NO error bits are set
                if (!(s & (I2C_S_ERR | I2C_S_CLKT))) {
                    printk("Device found at 0x%02X\n", addr);
                }
                break;
            }
            if (status & (I2C_S_ERR | I2C_S_CLKT)) {
                // Error occurred, no device at this address
                break;
            }
        }
    }
    return 0;
}
