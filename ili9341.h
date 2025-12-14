#pragma once
#include <stdint.h>

// ILI9341 size
#define ILI9341_WIDTH   240
#define ILI9341_HEIGHT  320

// RGB565 helpers
#define RGB565(r,g,b) ( ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | ((b & 0xF8) >> 3) )

void ili9341_init(void);
void ili9341_set_rotation(uint8_t r);          // 0..3
void ili9341_set_addr_window(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
void ili9341_push_color565(uint16_t color, uint32_t count);
void ili9341_fill_screen(uint16_t color);
void ili9341_draw_pixel(uint16_t x, uint16_t y, uint16_t color);
