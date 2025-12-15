#ifndef __FONT_H__
#define __FONT_H__

  // Characters definition
  // -----------------------------------
  // number of columns for chars
  #define CHARS_COLS_LENGTH  5
  // number of rows for chars
  #define CHARS_ROWS_LENGTH  8
  #define FONT_WIDTH   5
  #define FONT_HEIGHT  8
  #define FONT_FIRST   0x20
  #define FONT_LAST    0x7F
  // @const Characters
  extern const uint8_t FONTS[][CHARS_COLS_LENGTH];

#endif