#ifndef TEXTEFFECTS_H
#define TEXTEFFECTS_H

#include "lcd.h"
#include "customer.h"

void scrollText(HD44780 *lcd, char *txt);
void blinkText(HD44780 *lcd, char *txt);
void createSpecChar(HD44780 *lcd);
void fixSpecChar(char *InStr);
void getBitmap(char inputChar, uint8_t slicedChar[8]); 
void fadeInString(HD44780 *lcd, char *txt);
void discoMan(HD44780 *lcd);
int cleanBreak(char *inputStr);
void typeAnimation(HD44780 &lcd, char* txt);
void sweepAnimation(HD44780 &lcd);
const uint8_t* getBitmap(char inputChar);  
void splitTxt(HD44780 &lcd, char *txt);
bool isEvenMin();

#endif