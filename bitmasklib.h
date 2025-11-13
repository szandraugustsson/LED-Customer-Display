#ifndef BITMASKLIB_H
#define BITMASKLIB_H

#include <stdint.h>

// Declaring special character bitmask arrays
extern const uint8_t AW[8];
extern const uint8_t AE[8];
extern const uint8_t OO[8];
extern const uint8_t aw[8];
extern const uint8_t ae[8];
extern const uint8_t oo[8];

void RegChar2CGRAM(HD44780 &lcd);
void FixSpecChar(char *InStr);

#endif

