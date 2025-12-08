#include <stdint.h>

#include "i2c.h"
#include "bcm2835_addr.h"
#include "mmio.h"
#include "printk.h"
#include "delay.h"

void i2c_init(void) {
    // Set to ALT0 for I2C1 
	uint32_t old;

	// GPIO 2 
	old = bcm2835_read(GPIO_FSEL0);
	old &= ~(7 <<6 );  // Clear bits 8-6 
	old |= (4 << 6);   // Set to ALT0 
	bcm2835_write(GPIO_FSEL0, old);

	// GPIO 3 
	old = bcm2835_read(GPIO_FSEL0);
	old &= ~(7 << 9);  // Clear bits 11-9 
	old |= (4 << 9);   // Set to ALT0 
	bcm2835_write(GPIO_FSEL0, old);

	bcm2835_write(GPIO_PUD, GPIO_PUD_PULLUP);	// Enable pull-up resistors
	delay(150);
	bcm2835_write(GPIO_PUDCLK0, (1<<2)|(1<<3));	// Clock the control signal into GPIO 2 and 3
	delay(150);
	bcm2835_write(GPIO_PUD, GPIO_PUD_DISABLE);	// Remove the control signal
	bcm2835_write(GPIO_PUDCLK0, 0);	     // Remove the clock
    
    // Set I2C clock divider for 100kHz (250MHz clock)
    bcm2835_write(I2C_DIV, 2500);
    // Enable I2C
    bcm2835_write(I2C_C, I2C_C_CLEAR); // Clear FIFO
    bcm2835_write(I2C_C, I2C_C_I2CEN); 

    printk("The program made it here!\n");        // debugging
    return;
}

int i2c_write(uint8_t addr, const uint8_t *data, uint32_t len) {
    printk("Begginning of i2c_write\n");        // debugging
    bcm2835_write(I2C_A, addr); // Set slave address
    delay(150);
    bcm2835_write(I2C_C, I2C_C_CLEAR); // Clear FIFO
    bcm2835_write(I2C_S, I2C_S_CLKT | I2C_S_ERR | I2C_S_DONE); // Clear status flags
    
    bcm2835_write(I2C_DLEN, len); // Set data length
    

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
    printk("End of i2c_write\n");        // debugging
    return 0;
}

int i2c_read(uint8_t addr, uint8_t *data, uint32_t len) {
    printk("Begginning of i2c_read\n");        // debugging
    bcm2835_write(I2C_S, I2C_S_CLKT | I2C_S_ERR | I2C_S_DONE); // Clear status flags
    bcm2835_write(I2C_A, addr); // Set slave address
    bcm2835_write(I2C_DLEN, len); // Set data length
    //bcm2835_write(I2C_C, I2C_C_CLEAR | I2C_C_READ); // Clear FIFO and set to read mode

    //bcm2835_write(I2C_C, I2C_C_I2CEN | I2C_C_ST); // Start transfer
    bcm2835_write(I2C_C, I2C_C_CLEAR);   // Clear FIFO first
    // Start repeated-start READ
    bcm2835_write(I2C_C, I2C_C_I2CEN | I2C_C_ST | I2C_C_READ);

    uint32_t index = 0;
    while (1) {
        //printk("Am I stuck here forever?\n"); //yes you are
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
    printk("End of i2c_read\n");        // debugging
    return 0;
}

/*
uint8_t reg = 0x00;
		int r = i2c_write(0x36, &reg, 1);
		printk("i2c_write returned %d\n", r);

		uint8_t dummy = 0;
		int rv = i2c_write(0x36, &dummy, 1);

		if (rv == -1)
			printk("0x36 NACK\n");
		else
			printk("0x36 ACK\n");
*/

int i2c_scan(void) {
    /*printk("Start scan.\n");
    for (uint8_t addr = 0x30; addr <= 0x77; addr++) {
        int device_addr = i2c_write(addr, 0x0, 1);       // attempting to write to device
        delay(1500);
        if(device_addr == -1){
            printk("address: %x ---\n", addr);
        }
        else{
            printk("address: %x ACK\n", addr);
        }
        printk("The program is ending.\n");
    }
    return 0;*/
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
                if (!(status & (I2C_S_ERR | I2C_S_CLKT))) {
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

int i2c_write_read(uint8_t addr, const uint8_t *write_data, uint32_t write_len, uint8_t *read_data, uint32_t read_len) {
    printk("Beginning of i2c_write_read_combined\n");
    bcm2835_write(I2C_S, I2C_S_CLKT | I2C_S_ERR | I2C_S_DONE); // Clear status flags
    bcm2835_write(I2C_A, addr); // Set slave address

    //  Write 
    bcm2835_write(I2C_DLEN, write_len); 
    for (int i = 0; i < write_len; i++) {
        bcm2835_write(I2C_FIFO, write_data[i]);
    }
    bcm2835_write(I2C_C, I2C_C_I2CEN | I2C_C_ST); // Start transfer 

    // Wait for the write to be DONE, capture status flags
    while (!(bcm2835_read(I2C_S) & I2C_S_DONE)) {
        uint32_t status = bcm2835_read(I2C_S); // Read status once here
        if (status & (I2C_S_ERR)){
            printk("Phase 1 Error: NACK received during write phase (sending 0x0F/0x10).\n");
            return -1; // Error
        }
        if (status & I2C_S_CLKT) {
            printk("Phase 1 Error: Clock timeout during write phase.\n");
            return -1;
        }
    }
    printk("Phase 1 complete (ACKed).\n"); // Debug 

    delay(0x120000);
    // Read 
    // Clear DONE flag 
    
    //bcm2835_write(I2C_S, I2C_S_DONE); 
    
    // Set up for read
    bcm2835_write(I2C_DLEN, read_len);
    
    bcm2835_write(I2C_C, I2C_C_I2CEN | I2C_C_ST | I2C_C_READ);

    uint32_t index = 0;
    while (1) {
        uint32_t status = bcm2835_read(I2C_S);
        // Read data in FIFO
        while ((status & I2C_S_RXR) && (index < read_len)) {
            read_data[index++] = bcm2835_read(I2C_FIFO);
            status = bcm2835_read(I2C_S);
        }
        if (status & I2C_S_DONE) {
            break; // Transfer complete
        }
        if (status & I2C_S_ERR) { 
            printk("Phase 2 Error: NACK received during read phase (after Repeated Start).\n");
            return -1; // Error
        }
        if (status & I2C_S_CLKT) {
             printk("Phase 2 Error: Clock timeout during read phase.\n");
            return -1;
        }
    }
    printk("Phase 2 complete.\n"); // Debug

    printk("End of i2c_write_read_combined\n");
    return 0;
}

