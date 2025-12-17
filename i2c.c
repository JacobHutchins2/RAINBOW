#include <stdint.h>

#include "i2c.h"
#include "bcm2835_addr.h"
#include "mmio.h"
#include "printk.h"
#include "delay.h"

// Sensor function registers
#define SENSOR_ADDR 0x36
#define SENSOR_TOUCH_BASE    0x0F
#define SENSOR_TOUCH_FUNCTION 0x10
#define SENSOR_STATUS_BASE   0x00
#define SENSOR_STATUS_TEMP   0x04
#define LCD_ADDR 0x27

#if 0
#define LCD_D4  (1 << 0)
#define LCD_D5  (1 << 1)
#define LCD_D6  (1 << 2)
#define LCD_D7  (1 << 3)
#define LCD_EN  (1 << 4)
#define LCD_RW  (1 << 5)
#define LCD_RS  (1 << 6)
#define LCD_BL  (1 << 7)
#endif
#define LCD_RS  (1 << 0)
#define LCD_RW  (1 << 1)
#define LCD_EN  (1 << 2)
#define LCD_BL  (1 << 3)

void i2c_init(void) {
    // Set to ALT0 for I2C1 
	uint32_t temp_reg;

    bcm2835_write(I2C_C, 0);               // reset i2c control regs
	// GPIO 2 
	temp_reg = bcm2835_read(GPIO_FSEL0);
	temp_reg &= ~(7 <<6 );  // Clear bits 8-6 
	temp_reg |= (4 << 6);   // Set to ALT0 
	bcm2835_write(GPIO_FSEL0, temp_reg);

	// GPIO 3 
	temp_reg = bcm2835_read(GPIO_FSEL0);
	temp_reg &= ~(7 << 9);  // Clear bits 11-9 
	temp_reg |= (4 << 9);   // Set to ALT0 
	bcm2835_write(GPIO_FSEL0, temp_reg);

    delay(150);
	bcm2835_write(GPIO_PUDCLK0, (1<<2)|(1<<3));	// Clock the control signal into GPIO 2 and 3
	delay(150);
	bcm2835_write(GPIO_PUD, GPIO_PUD_DISABLE);	// Disable PUD
	bcm2835_write(GPIO_PUDCLK0, 0);	     // Reset the clock

    // Set I2C clock divider for 100kHz (700MHz clock)
    bcm2835_write(I2C_DIV, 7000);

    //printk("The program made it here!\n");        // debugging
    return;
}

int i2c_write(uint8_t addr, const uint8_t *data, uint32_t len) {
    //printk("Begginning of i2c_write\n");        // debugging
    bcm2835_write(I2C_A, addr); // Set slave address
    bcm2835_write(I2C_C, I2C_C_CLEAR); // Clear FIFO
    bcm2835_write(I2C_S, I2C_S_CLKT | I2C_S_ERR | I2C_S_DONE); // Clear status flags
    bcm2835_write(I2C_DLEN, len); // Set data length
    bcm2835_write(I2C_C, I2C_C_I2CEN | I2C_C_ST); // Start transfer

    // Write to FIFO
    for (int i = 0; i < len; i++) {
        while((bcm2835_read(I2C_S)&I2C_S_TXD)==0) {     // waiting for status and transfer
                asm("");        // avoid optimization
        }
        bcm2835_write(I2C_FIFO, data[i]);       // writing 
    }
    while ((bcm2835_read(I2C_S)&I2C_S_DONE) ==0) {  // waiting for status and DONE
                asm("");        // avoid optimization
    }
    
    //printk("End of i2c_write\n");        // debugging
    return 0;
}

int i2c_read(uint8_t addr, uint8_t *data, uint32_t len) {
    //printk("Begginning of i2c_read\n");        // debugging
    //printk("i2c reading 0x%x length %d\n", addr, len);

    bcm2835_write(I2C_A, addr); // Set slave address
    bcm2835_write(I2C_C, I2C_C_CLEAR); // clear fifo
    bcm2835_write(I2C_S, I2C_S_CLKT | I2C_S_ERR | I2C_S_DONE); // Clear status flags
    bcm2835_write(I2C_DLEN, len); // Set data length
    bcm2835_write(I2C_C, I2C_C_I2CEN | I2C_C_ST | I2C_C_READ); // Start transfer

    uint32_t index = 0;
    while((bcm2835_read(I2C_S) & I2C_S_DONE) == 0) {        // wait for status and DONE
        //printk("Am I stuck here forever?\n"); //yes you are
        asm("");        // avoid optimization

        index++;
        if (index == 0x220000) {
            printk("i2c read timeout1\n");      //debugging
            break;
        }
    }

    for(index = 0; index < len; index++){
        //int k = 0;
        while((bcm2835_read(I2C_S & I2C_S_RXD)) == 0) {     // wait for satus and received
            delay(150);
		    //k++;
        }
        data[index] = bcm2835_read(I2C_FIFO) & 0xff;
        //printk("        read 0x%x\n", data[i]);     // debugging
    }
    //printk("End of i2c_read\n");        // debugging
    return 0;
}


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

// shows registers needed for the sensor
// https://learn.adafruit.com/adafruit-stemma-soil-sensor-i2c-capacitive-moisture-sensor/faq? 
//   SENSOR_TOUCH_BASE    0x0F
//   SENSOR_TOUCH_FUNCTION 0x10
//   SENSOR_STATUS_BASE   0x00
//   SENSOR_STATUS_TEMP   0x04

// fucntion that will take command registers and run them through read/writes
static int i2c_write_read(uint8_t base, uint8_t func, uint8_t *buf, uint32_t len) {
    uint8_t cmd[2] = { base, func };
    if (i2c_write(SENSOR_ADDR, cmd, 2) < 0) return -1;      //write
    delay(0x20000); // cycles wait for sensor
    if (i2c_read(SENSOR_ADDR, buf, len) < 0) return -1;     //read
    return 0;
}

// Read soil capacitance (moisture)
uint16_t sensor_moisture(void) {
    uint8_t buf[2];
    if (i2c_write_read(SENSOR_TOUCH_BASE, SENSOR_TOUCH_FUNCTION, buf, 2) < 0) return 0;
    uint16_t raw = (uint16_t)buf[0] << 8 | buf[1];
    printk("buf[0]: %u, buf[1]: %u\n", buf[0]<<8, buf[1]);
    return raw;
}

// Read temperature in °C
// Sends command registers for getting temp
static uint32_t sensor_temperature(void) {
    uint8_t buf[4];
    if (i2c_write_read(SENSOR_STATUS_BASE, SENSOR_STATUS_TEMP, buf, 4) < 0)
        return 0;

    uint32_t raw = ((uint32_t)buf[0] << 24) |
                   ((uint32_t)buf[1] << 16) |
                   ((uint32_t)buf[2] << 8) |
                    (uint32_t)buf[3];

    // Convert fixed-point to milli-Celsius 
    return (raw * 1000) >> 16;  // divide by 65536
}



/*======================================== DISPLAY CODE ========================================*/
static void lcd_i2c_write(uint8_t v){
    i2c_write(LCD_ADDR, &v, 1);
}

static void lcd_pulse(uint8_t data){
    lcd_i2c_write(data | LCD_EN);
    delay(150);
    lcd_i2c_write(data & ~LCD_EN);
    delay(150);
}

static void lcd_write4(uint8_t nibble, uint8_t rs){
    uint8_t data = LCD_BL | rs | (nibble << 4);
    lcd_pulse(data);
}

void lcd_cmd(uint8_t cmd){
    lcd_write4(cmd >> 4, 0);
    lcd_write4(cmd & 0x0F, 0);
    delay(150);
}

void lcd_data(uint8_t ch){
    lcd_write4(ch >> 4, LCD_RS);
    lcd_write4(ch & 0x0F, LCD_RS);
}

void lcd_init(void){
    printk("Entering lcd init.\n");
    delay_ms(150);  // wait after power-up

    // Init sequence
    lcd_write4(0x03, 0);
    delay_ms(10);
    lcd_write4(0x03, 0);
    delay_ms(10);
    lcd_write4(0x03, 0);
    delay_ms(10);
    lcd_write4(0x02, 0);  // 4-bit mode

    lcd_cmd(0x28); // 4-bit, 2-line, 5x8
    lcd_cmd(0x0C); // display ON, cursor OFF
    lcd_cmd(0x06); // entry mode
    lcd_cmd(0x01); // clear
    printk("Exiting lcd init.\n");
}

void lcd_set_cursor(uint8_t row, uint8_t col){
    //printk("Entering lcd set cursor.\n");
    uint8_t addr = (row == 0) ? 0x00 : 0x40;
    lcd_cmd(0x80 | (addr + col));
    //printk("Entering lcd set cursor.\n");
}

void lcd_print(const char *s){
    //printk("Entering lcd print.\n");
    while (*s)
        lcd_data(*s++);
    //printk("Exiting lcd print.\n");
}

void lcd_print_int(int value){
    //printk("Entering lcd int print.\n");
    char buf[12];   // enough for 32-bit int
    int i = 0;

    if (value == 0) {
        buf[i++] = '0';
    } 
    else{
        if (value < 0) {
            lcd_data('-');
            value = -value;
        }

        int start = i;
        while (value > 0) {
            buf[i++] = '0' + (value % 10);
            value /= 10;
        }

        // reverse digits
        for (int j = i - 1; j >= start; j--)
            lcd_data(buf[j]);

        return;
    }

    buf[i] = '\0';
    lcd_print(buf);
}
/*==============================================================================================*/

// getting sensor data 
void get_sensor_data(void) {
    
    uint32_t temp = sensor_temperature();
    uint16_t moisture = sensor_moisture();

    printk("Moisture: %u   Temp: %u.%03u C\n", moisture, (temp/1000)-6, temp%1000);     // prints to serial
    delay(0x150); //

    // printing moisture to lcd screen
    lcd_cmd(0x01); // clear
    lcd_set_cursor(0, 0);
    lcd_print("Menu:");
    lcd_set_cursor(1, 0);
    lcd_print("Moist: ");
    lcd_print_int(moisture);
    
}
