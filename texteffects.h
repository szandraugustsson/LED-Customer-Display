#ifndef TEXTEFFECTS_H
#define TEXTEFFECTS_H

#include "lcd.h"
#include "customer.h"

void scrollText(HD44780 *lcd, char *txt);
void blinkText(HD44780 *lcd, char *txt);
void createSpecChar(HD44780 *lcd);
void FixSpecChar(char *InStr);
void GetBitmap(char inputChar, uint8_t slicedChar[8]); 
void FadeInString(HD44780 *lcd, char *txt);
void DiscoMan(HD44780 *lcd);
int CleanBreak(char *inputStr);

#endif