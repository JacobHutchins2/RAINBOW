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
#define I2C_BASE	(0x804000)

#define I2C_C		(I2C_BASE + 0x00)  // Control
#define I2C_S		(I2C_BASE + 0x04)  // Status
#define I2C_DLEN	(I2C_BASE + 0x08)  // Data Length
#define I2C_A		(I2C_BASE + 0x0c)  // Slave Address
#define I2C_FIFO	(I2C_BASE + 0x10)  // Data FIFO
#define I2C_DIV		(I2C_BASE + 0x14)  // Clock Divider
#define I2C_DEL		(I2C_BASE + 0x18)  // Data Delay
#define I2C_CLKT	(I2C_BASE + 0x1c)  // Clock Stretch Timeout

// control register
#define I2C_C_I2CEN		(1 << 15) // I2C Enable
#define I2C_C_ST		(1 << 7)  // Start Transfer
#define I2C_C_CLEAR		((1 << 5) | (1 << 4))  // Clear FIFO
#define I2C_C_READ		(1 << 0)  // Read Transfer

// status register
#define I2C_S_TA		(1 << 0)  // Transfer Active
#define I2C_S_DONE		(1 << 1)  // Transfer Done
#define I2C_S_TXW		(1 << 2)  // TX FIFO needs writing
#define I2C_S_RXR		(1 << 3)  // RX FIFO needs reading
#define I2C_S_TXD		(1 << 4)  // TX FIFO can accept data
#define I2C_S_RXD		(1 << 5)  // RX FIFO contains data
#define I2C_S_ERR		(1 << 8)  // ACK Error
#define I2C_S_CLKT		(1 << 9)  // Clock Stretch Timeout

/*==================================================================================*/

/*======================================= IRQ ======================================*/
// base address for interrupt controller

#define IRQ_BASE        (0xb000)       // IRQ base address

#define IRQ_BASIC_PENDING        (IRQ_BASE + 0x200) // Basic Pending
#define IRQ_BASIC_PENDING_TIMER  (1 << 0)        // Timer interrupt
#define IRQ_BASIC_PENDING_IRQ    (1 << 19)       // IRQ interrupt

#define IRQ_ENABLE_IRQ1    (IRQ_BASE + 0x210) // Enable IRQs 1-31
#define IRQ_ENABLE_BASIC_IRQ   (IRQ_BASE + 0x218) // Enable Basic IRQs
#define IRQ_DISABLE_IRQ1   (IRQ_BASE + 0x21C) // Disable IRQs 1-31
#define IRQ_DISABLE_BASIC  (IRQ_BASE + 0x224) // Disable Basic IRQs

#define IRQ0_PENDING0		(IRQ_BASE+0x200)    // IRQ Pending 0
#define IRQ0_PENDING1		(IRQ_BASE+0x204)    // IRQ Pending 1
#define IRQ_DISABLE_BASIC_IRQ	(IRQ_BASE+0x224)

#define IRQ0_SET_EN_0		(IRQ_BASE+0x210)    // IRQ Set Enable 0
#define IRQ0_SET_EN_1		(IRQ_BASE+0x214)    // IRQ Set Enable 1

#define IRQ0_CLR_EN_0		(IRQ_BASE+0x220)    // IRQ Clear Enable 0
#define IRQ0_CLR_EN_1		(IRQ_BASE+0x224)    // IRQ Clear Enable 1

#define IRQ0_STATUS0		(IRQ_BASE+0x230)    // IRQ Status 0
#define IRQ0_STATUS1		(IRQ_BASE+0x234)    // IRQ Status 1

#define IRQ_ENABLE_BASIC_IRQ_ARM_TIMER		(1<<0)

/*==================================================================================*/

/*======================================= UART ======================================*/
// base address for UART0 peripheral
// good old serial
#define UART0_BASE      (0x201000)      // UART0 base address

#define UART0_DR        (UART0_BASE + 0x00) // Data Register
#define UART0_RSRECR    (UART0_BASE + 0x04) // Receive

#define UART0_FR        (UART0_BASE + 0x18) // Flag Register
#define UART0_FR_TXFE   (1 << 7)        // Transmit FIFO Empty
#define UART0_FR_RXFF   (1 << 6)        // Receive FIFO Full
#define UART0_FR_TXFF   (1 << 5)        // Transmit FIFO Full
#define UART0_FR_RXFE   (1 << 4)        // Receive FIFO Empty
#define UART0_FR_BUSY   (1 << 3)        // UART Busy

#define UART0_ILPR      (UART0_BASE + 0x20) // IrDA Low-Power Register
#define UART0_IBRD      (UART0_BASE + 0x24) // Integer Baud
#define UART0_FBRD      (UART0_BASE + 0x28) // Fractional Baud

#define UART0_LCRH      (UART0_BASE + 0x2C) // Line Control
#define UART0_LCRH_SPS          (1 << 7)        // Stick Parity Select
#define UART0_LCRH_WLEN_5BIT	(0<<5)      // Word Length 5 bits
#define UART0_LCRH_WLEN_6BIT	(1<<5)      // Word Length 6 bits
#define UART0_LCRH_WLEN_7BIT	(2<<5)      // Word Length 7 bits
#define UART0_LCRH_WLEN_8BIT	(3<<5)      // Word Length 8 bits
#define UART0_LCRH_FEN          (1<<4)      // enable FIFOs
#define UART0_LCRH_STP2         (1<<3)      // enable 2 stop bits
#define UART0_LCRH_EPS          (1<<2)      // even parity selection
#define UART0_LCRH_PEN          (1<<1)      // parity enable
#define UART0_LCRH_BRK          (1<<0)      // break after next character

#define UART0_CR        (UART0_BASE + 0x30)
#define UART0_CR_CTSEN          (1<<15)     // enable CTS flow
#define UART0_CR_RTSEN          (1<<14)     // enable RTS flow
#define UART0_CR_RTS            (1<<11)     // request to send
#define UART0_CR_RXE            (1<<9)      // enable receive
#define UART0_CR_TXE            (1<<8)      // enable transmit
#define UART0_CR_LBE            (1<<7)      // enable loopback
#define UART0_CR_UARTEN         (1<<0)      // enable UART

#define UART0_IFLS	(UART0_BASE + 0x34)     // UART interrupt selector
#define UART0_IMSC  (UART0_BASE + 0x38)     // interrupt mask set/clear
#define UART0_IMSC_OE           (1<<10)     // raw interrupt overrun status
#define UART0_IMSC_BE           (1<<9)      // raw interrupt break status
#define UART0_IMSC_PE           (1<<8)      // raw interrupt parity status
#define UART0_IMSC_FE           (1<<7)      // raw interrupt framing status
#define UART0_IMSC_RT           (1<<6)      // receive timeout status
#define UART0_IMSC_TX           (1<<5)      // transmit interrupt status
#define UART0_IMSC_CTS          (1<<1)      // UART flow control status
#define UART0_RIS	(UART0_BASE + 0x3C)     // 
#define UART0_MIS	(UART0_BASE + 0x40)
#define UART0_ICR	(UART0_BASE + 0x44)

/*===================================================================================*/

/*======================================= TIMER ======================================*/
// base address for timer
#define TIMER_BASE  (0xb000)

#define TIMER_LOAD  (TIMER_BASE + 0x400)     // load value to count
#define TIMER_VALUE (TIMER_BASE + 0x404)     // currenttimer value

#define TIMER_CONTROL   (TIMER_BASE + 0x408)    // access timer control values
#define TIMER_CONTROL_ENABLE        (1<<7)      // enable counter
#define TIMER_CONTROL_INT_ENABLE    (1<<5)      // timer interrupt enable
#define TIMER_CONTROL_PRESCALE_256          (2<<2)      // counter scale
#define TIMER_CONTROL_32BIT         (1<<1)      // timer size

#define TIMER_IRQ_CLEAR (TIMER_BASE + 0x40c)    // timer interrupt clear
#define TIMER_PREDIVIDER (TIMER_BASE + 0x41c)   // counter divider
/*====================================================================================*/

/*======================================= SPI ======================================*/
// base address for SPI module
#define SPI_Base (0x204000)

#define SPI_CS     (SPI_Base + 0x0)             // SPI Master Control and Status
#define SPI_FIFO   (SPI_Base + 0x4)       // SPI Master Tx and Rx FIFOs
#define SPI_CLK    (SPI_Base + 0x8)       // SPI Master Clock Divider
#define SPI_DLEN   (SPI_Base + 0xc)       // SPI Master Data Length
#define SPI_LTOH   (SPI_Base + 0x10)      // SPI LOSSI mode TOH
#define SPI_DC     (SPI_Base + 0x14)      // SPI DMA DREQ Controls

#define SPI_CS_SELECT0          (0<<0)              // SELECT SPI0 chip
#define SPI_CS_SELECT1          (1<<0)              // Select SPI1 chip
#define SPI_CS_SELECT2          (1<<1)              // Select SPI2 chip
#define SPI_CS_CPHA_BEG         (1<<2)             // Select First SCLK transition at beginning of data bit
#define SPI_CS_CPOL_HIGH        (1<<3)            // Rest state of clock is high
#define SPI_CS_CLEAR_TX         (1<<4)
#define SPI_CS_CLEAR_RX         (1<<5)
#define SPI_CS_CLEAR            ((1<<4) | (1<<5))      // Clear Tx + Rx FIFO 
#define SPI_CS_CSPOL_HIGH       (1<<6)           // Chip select lines are active high
#define SPI_CS_TRANSFER_ACTIVE  (1<<7)      // Transfer active
#define SPI_CS_DMAEN_ENABLE     (1<<8)         // Enable DMA operation
#define SPI_CS_INTD_ON          (1<<9)             // Generate interrupt when DONE = 1
#define SPI_CS_INTR_ON          (1<<10)             // Generate interrupt when RXR = 1
#define SPI_CS_ADCS_ON          (1<<11)             // Auto Deassert Chip Select at end of DMA transfer
#define SPI_CS_REN_WRITE        (1<<12)               // Intend to Write to peripheral
#define SPI_CS_LEN_LOSSI        (1<<13)           // Interface acts as LoSSI master
#define SPI_CS_DONE_DONE        (1<<16)           // transfer done
#define SPI_CS_RXD_USED         (1<<17)            // RX FIFO being used/not empty
#define SPI_CS_TXD_OPEN         (1<<18)            // TX FIFO has room open
#define SPI_CS_RXR_FULL         (1<<19)            // RX FIFO is more full
#define SPI_CS_RXF_FULL         (1<<20)            // RX FIFO is full
#define SPI_CS_CSPOL0_HIGH      (1<<21)            // Chip select 0 is active high
#define SPI_CS_CSPOL1_HIGH      (1<<22)            // Chip select 1 is active high
#define SPI_CS_CSPOL2_HIGH      (1<<23)            // Chip select 2 is active high
#define SPI_CS_DMA_LEN          (1<<24)             // Enable DMA mode in LoSSI mode
#define SPI_CS_LENLONG_32       (1<<25)          // Writing to FIFO will write 32 bit word
/*==================================================================================*/

/*======================================= PWM ======================================*/
#define PWM_BASE     0x20C000

#define PWM_CTL      (PWM_BASE + 0x00)  // PWM Control
#define PWM_STA      (PWM_BASE + 0x04)  // PWM Status
#define PWM_DMAC     (PWM_BASE + 0x08)  // PWM DMA Configuration
#define PWM_RNG1     (PWM_BASE + 0x10)  // PWM Channel
#define PWM_DAT1     (PWM_BASE + 0x14)  // PWM Channel 1 Data

#define PWM_CTL_PWEN1       (1 << 0)    // Channel 1 Enable
#define PWM_CTL_MODE1       (1 << 1)    // Channel 1 Mode
#define PWM_CTL_RPTL1       (1 << 2)    // Channel 1 Repeat Last Data
#define PWM_CTL_SBIT1       (1 << 3)    // Channel 1 Silence Bit
#define PWM_CTL_POLA1       (1 << 4)    // Channel 1 Polarity
#define PWM_CTL_USEF1       (1 << 5)    // Channel 1 use FIFO
#define PWM_CTL_CLRF1       (1 << 6)    // Channel 1 Clear FIFO
#define PWM_CTL_MSEN1       (1 << 7)    // Channel 1 M/S enable

#define PWM_STA_FULL1       (1 << 0)    // Channel 1 FIFO Full
#define PWM_STA_EMPT1       (1 << 1)    // Channel 1 FIFO Empty
#define PWM_STA_WERR1       (1 << 2)    // Channel 1 Write Error
#define PWM_STA_RERR1       (1 << 3)    // Channel 1 Read Error
#define PWM_STA_GAPO1       (1 << 4)    // Channel 1 Gap Occurred
#define PWM_STA_BERR        (1 << 8)    // Bus Error
#define PWM_STA_STA1        (1 << 9)    // Channel 1 Status

#define CLK_BASE        0x101000
#define CM_PWMCTL   (CLK_BASE + 0xA0)
#define CM_PWMDIV   (CLK_BASE + 0xA4)

#define CM_PASSWORD     0x5A000000
/*==================================================================================*/