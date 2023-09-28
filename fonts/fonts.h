#ifndef fonts_h
#define fonts_h
#include <avr/pgmspace.h>
#include "Arduino.h"



/* SYSTEM FONT */

typedef struct
{
  int width; // Character width in bits.
  int offset; // Offset in bytes into font bitmap.
}FONT_CHAR_INFO;

typedef struct
{
  int height; // Character height in bits.
  char start_char; // Start character.
  char end_char; // End character.
  uint8_t char_spacing;
  const FONT_CHAR_INFO *p_character_descriptor; // Character descriptor array.
  const uint8_t *p_character_bitmaps; // Character bitmap array.
}FONT_INFO;

#endif