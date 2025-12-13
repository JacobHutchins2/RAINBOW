void spi_init(void);
void tft_gpio_init(void);
void tft_init(void);
void tft_write_data(uint8_t data);
void tft_write_cmd(uint8_t cmd);
void tft_write_buf(uint8_t *buf, int len);
void spi_write_stream(const uint8_t *buf, int len);
void spi_test_byte(uint8_t b);