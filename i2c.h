void i2c_init(void);
int i2c_write(uint8_t addr, const uint8_t *data, uint32_t len);
int i2c_read(uint8_t addr, uint8_t *data, uint32_t len);
int i2c_scan(void);
int i2c_write_read(uint8_t addr, const uint8_t *write_data, uint32_t write_len, uint8_t *read_data, uint32_t read_len);