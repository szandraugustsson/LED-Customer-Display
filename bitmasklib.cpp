#include <stdint.h>
#include "lcd.h"
#include "bitmasklib.h"


    // Defining special character bitmask arrays
    const uint8_t AW[8] = { 0b00100,0b00000,0b01110,0b10001,0b11111,0b10001,0b00000,0b10001 }; // Å
    const uint8_t AE[8] = { 0b01010,0b00000,0b01110,0b10001,0b11111,0b10001,0b00000,0b10001 }; // Ä
    const uint8_t OO[8] = { 0b01010,0b00000,0b01110,0b10001,0b10001,0b10001,0b01110,0b00000 }; // Ö
    const uint8_t aw[8] = { 0b00100,0b01010,0b00100,0b01110,0b00001,0b01111,0b10001,0b01111 }; // å
    const uint8_t ae[8] = { 0b01010,0b00000,0b01110,0b00001,0b01111,0b10001,0b01111,0b00000 }; // ä
    const uint8_t oo[8] = { 0b01010,0b00000,0b01110,0b10001,0b10001,0b10001,0b01110,0b00000 }; // ö
    
    /* Tecken sparas till CGRAM(Character Generator RAM) som är 
    HD44780:s dedikerade minnesutrymme(8 slots maxcap)
    för custom chars, dem tilldelas plats 1,2,3,4,5,6*/

void RegChar2CGRAM(HD44780 &lcd)
    {
        lcd.CreateChar(1, AW); 
        lcd.CreateChar(2, AE); 
        lcd.CreateChar(3, OO); 
        lcd.CreateChar(4, aw); 
        lcd.CreateChar(5, ae); 
        lcd.CreateChar(6, oo);
    }


#include <string.h> 


void FixSpecChar(char *InStr)
{
    int j = 0;
    char OutStr[strlen(InStr)];
    memset(OutStr,0,sizeof(OutStr));
    for (int i = 0; i < strlen(InStr); i++)
    {   
    if      ((uint8_t)InStr[i] == 0xC3 && (uint8_t)InStr[i+1] == 0x85) {OutStr[j] = 1;j++; i++;} // Å
    else if ((uint8_t)InStr[i] == 0xC3 && (uint8_t)InStr[i+1] == 0x84) {OutStr[j] = 2;j++; i++;} // Ä
    else if ((uint8_t)InStr[i] == 0xC3 && (uint8_t)InStr[i+1] == 0x96) {OutStr[j] = 3;j++; i++;} // Ö
    else if ((uint8_t)InStr[i] == 0xC3 && (uint8_t)InStr[i+1] == 0xA5) {OutStr[j] = 4;j++; i++;} // å
    else if ((uint8_t)InStr[i] == 0xC3 && (uint8_t)InStr[i+1] == 0xA4) {OutStr[j] = 5;j++; i++;} // ä
    else if ((uint8_t)InStr[i] == 0xC3 && (uint8_t)InStr[i+1] == 0xB6) {OutStr[j] = 6;j++; i++;} // ö
    else
    {OutStr[j] = InStr[i];j++;} 
    }
    // Itererar över angiven sträng och jämför hex-tal sekvenser för att ersätta element med custom chars(å, ä, ö).
    // Man kan alltså kalla funktionen som FixSpecChar(Min-Sträng-Var)
    memset(InStr,0,sizeof(InStr));
    strcpy(InStr,OutStr);
}
