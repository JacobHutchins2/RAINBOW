// bcm2835 peripheral base addresses for the Pi 1B+

/*====================================== GPIO ======================================*/

#define GPIO_BASE         0x200000

#define GPIO_FSEL0       (GPIO_BASE + 0x00)         // Function Select 0
#define GPIO_FSEL1       (GPIO_BASE + 0x04)         // Function Select 1
#define GPIO_FSEL2       (GPIO_BASE + 0x08)         // Function Select 2
#define GPIO_FSEL3       (GPIO_BASE + 0x0C)         // Function Select 3
#define GPIO_FSEL4       (GPIO_BASE + 0x10)         // Function Select 4
#define GPIO_FSEL5       (GPIO_BASE + 0x14)         // Function Select 5

#define GPIO_SET0        (GPIO_BASE + 0x1C)         // GPIO Pins 32-0   Set GPIO
#define GPIO_SET1        (GPIO_BASE + 0x20)         // GPIO Pins 54-33  Set GPIO

#define GPIO_CLR0        (GPIO_BASE + 0x28)         // GPIO Pins 32-0 Clear GPIO
#define GPIO_CLR1        (GPIO_BASE + 0x2C)         // GPIO Pins 54-33 Clear GPIO

#define GPIO_LEV0        (GPIO_BASE + 0x34)         // GPIO Pins 32-0   Level GPIO > return value on pins
#define GPIO_LEV1        (GPIO_BASE + 0x38)         // GPIO Pins 54-33  Level GPIO > return value on pins

#define GPIO_EDS0        (GPIO_BASE + 0x40)         // Event Detect Status
#define GPIO_EDS1        (GPIO_BASE + 0x44)         // Event Detect Status
#define GPIO_REN0        (GPIO_BASE + 0x4C)         // Rising Edge Detect Enable
#define GPIO_REN1        (GPIO_BASE + 0x50)         // Rising Edge Detect Enable
#define GPIO_FEN0        (GPIO_BASE + 0x58)         // Falling Edge Detect 
#define GPIO_FEN1        (GPIO_BASE + 0x5C)         // Falling Edge Detect 
#define GPIO_HEN0        (GPIO_BASE + 0x64)         // High level Detect
#define GPIO_HEN1        (GPIO_BASE + 0x68)         // High level Detect 
#define GPIO_LEN0        (GPIO_BASE + 0x70)         // Low level Detect 
#define GPIO_LEN1        (GPIO_BASE + 0x74)         // Low level Detect 
#define GPIO_AREN0       (GPIO_BASE + 0x7C)         // Async Rising Edge Detect
#define GPIO_AREN1       (GPIO_BASE + 0x80)         // Async Rising Edge Detect
#define GPIO_AFEN0       (GPIO_BASE + 0x88)         // Async Falling Edge Detect
#define GPIO_AFEN1       (GPIO_BASE + 0x8C)         // Async Falling Edge Detect

#define GPIO_PUD         (GPIO_BASE + 0x94)         // Pull-up/down Register
#define GPIO_PUD_DISABLE    0x00
#define GPIO_PUD_PULLDOWN   0x01
#define GPIO_PUD_PULLUP     0x02

#define GPIO_PUDCLK0     (GPIO_BASE + 0x98)         // Pull-up/down Clock Register 0
#define GPIO_PUDCLK1     (GPIO_BASE + 0x9C)         // Pull-up/down Clock Register 1

/*==================================================================================*/

/*======================================= I2C ======================================*/

// BCM2835 peripherals Pg28
// base for I2C peripheral BSC1
#define I2C_BASE	(0x7e804000)

#define I2C_C		(I2C_BASE + 0x00)  // Control
#define I2C_S		(I2C_BASE + 0x04)  // Status
#define I2C_DLEN	(I2C_BASE + 0x08)  // Data Length
#define I2C_A		(I2C_BASE + 0x0c)  // Slave Address
#define I2C_FIFO	(I2C_BASE + 0x10)  // Data FIFO
#define I2C_DIV		(I2C_BASE + 0x14)  // Clock Divider
#define I2C_DEL		(I2C_BASE + 0x18)  // Data Delay
#define I2C_CLKT	(I2C_BASE + 0x1c)  // Clock Stretch Timeout

#define I2C_C_I2CEN		(1 << 15) // I2C Enable
#define I2C_C_ST		(1 << 7)  // Start Transfer
#define I2C_C_CLEAR		(1 << 4)  // Clear FIFO
#define I2C_C_READ		(1 << 0)  // Read Transfer

#define I2C_S_TA		(1 << 0)  // Transfer Active
#define I2C_S_DONE		(1 << 1)  // Transfer Done
#define I2C_S_TXW		(1 << 2)  // TX FIFO needs writing
#define I2C_S_RXR		(1 << 3)  // RX FIFO needs reading
#define I2C_S_ERR		(1 << 8)  // ACK Error
#define I2C_S_CLKT		(1 << 9)  // Clock Stretch Timeout

/*==================================================================================*/