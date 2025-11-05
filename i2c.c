#include <stdint.h>
#include "i2c.h"
#include "bcm2835_addr.h"
#include "mmio.h"

int i2c_write(uint8_t addr, const uint8_t *data, uint32_t len) {

    bcm2835_write(I2C_S, I2C_S_CLKT | I2C_S_ERR | I2C_S_DONE); // Clear status flags

}

