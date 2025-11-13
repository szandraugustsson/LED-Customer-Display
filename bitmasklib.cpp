#include <stdint.h>
#include <string.h> 
#include "lcd.h"
#include "bitmasklib.h"


    // Defining special character bitmask arrays
    uint8_t AW[8] = { 0b00100,0b00000,0b01110,0b10001,0b11111,0b10001,0b00000,0b10001 }; // Å
    uint8_t AE[8] = { 0b01010,0b00000,0b01110,0b10001,0b11111,0b10001,0b00000,0b10001 }; // Ä
    uint8_t OO[8] = { 0b01010,0b00000,0b01110,0b10001,0b10001,0b10001,0b01110,0b00000 }; // Ö
    uint8_t aw[8] = { 0b00100,0b01010,0b00100,0b01110,0b00001,0b01111,0b10001,0b01111 }; // å
    uint8_t ae[8] = { 0b01010,0b00000,0b01110,0b00001,0b01111,0b10001,0b01111,0b00000 }; // ä
    uint8_t oo[8] = { 0b01010,0b00000,0b01110,0b10001,0b10001,0b10001,0b01110,0b00000 }; // ö
    
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

//A                                                                                         //N 
uint8_t AUp[8] = { 0b01110, 0b10001, 0b10001, 0b11111, 0b10001, 0b10001, 0b10001, 0b00000 };uint8_t NUp[8] = { 0b10001, 0b10001, 0b11001, 0b10101, 0b10011, 0b10001, 0b10001, 0b00000 };
uint8_t ALo[8] = { 0b00000, 0b01110, 0b00001, 0b01111, 0b10001, 0b10011, 0b01101, 0b00000 };uint8_t NLo[8] = { 0b00000, 0b00000, 0b10110, 0b11001, 0b10001, 0b10001, 0b10001, 0b00000 };
//B                                                                                         //O                                           
uint8_t BUp[8] = { 0b11110, 0b10001, 0b10001, 0b11110, 0b10001, 0b10001, 0b11110, 0b00000 };uint8_t OUp[8] = { 0b01110, 0b10001, 0b10001, 0b10001, 0b10001, 0b10001, 0b01110, 0b00000 };
uint8_t BLo[8] = { 0b10000, 0b10000, 0b10110, 0b11001, 0b10001, 0b10001, 0b11110, 0b00000 };uint8_t OLo[8] = { 0b00000, 0b00000, 0b01110, 0b10001, 0b10001, 0b10001, 0b01110, 0b00000 };
//C                                                                                         //P                                              
uint8_t CUp[8] = { 0b01110, 0b10001, 0b10000, 0b10000, 0b10000, 0b10001, 0b01110, 0b00000 };uint8_t PUp[8] = { 0b11110, 0b10001, 0b10001, 0b11110, 0b10000, 0b10000, 0b10000, 0b00000 };
uint8_t CLo[8] = { 0b00000, 0b01110, 0b10001, 0b10000, 0b10000, 0b10001, 0b01110, 0b00000 };uint8_t PLo[8] = { 0b00000, 0b00000, 0b11110, 0b10001, 0b10001, 0b11110, 0b10000, 0b10000 };
//D                                                                                         //Q                                          
uint8_t DUp[8] = { 0b11110, 0b10001, 0b10001, 0b10001, 0b10001, 0b10001, 0b11110, 0b00000 };uint8_t QUp[8] = { 0b01110, 0b10001, 0b10001, 0b10001, 0b10101, 0b10010, 0b01101, 0b00000 };
uint8_t DLo[8] = { 0b00001, 0b00001, 0b01101, 0b10011, 0b10001, 0b10001, 0b01111, 0b00000 };uint8_t QLo[8] = { 0b00000, 0b00000, 0b01111, 0b10001, 0b10001, 0b01111, 0b00001, 0b00001 };
//E                                                                                         //R                                            
uint8_t EUp[8] = { 0b11111, 0b10000, 0b10000, 0b11110, 0b10000, 0b10000, 0b11111, 0b00000 };uint8_t RUp[8] = { 0b11110, 0b10001, 0b10001, 0b11110, 0b10100, 0b10010, 0b10001, 0b00000 };
uint8_t ELo[8] = { 0b00000, 0b01110, 0b10001, 0b11111, 0b10000, 0b10001, 0b01110, 0b00000 };uint8_t RLo[8] = { 0b00000, 0b00000, 0b10110, 0b11001, 0b10000, 0b10000, 0b10000, 0b00000 };
//F                                                                                         //S                         
uint8_t FUp[8] = { 0b11111, 0b10000, 0b10000, 0b11110, 0b10000, 0b10000, 0b10000, 0b00000 };uint8_t SUp[8] = { 0b01111, 0b10000, 0b10000, 0b01110, 0b00001, 0b00001, 0b11110, 0b00000 };
uint8_t FLo[8] = { 0b00110, 0b01001, 0b01000, 0b11100, 0b01000, 0b01000, 0b01000, 0b00000 };uint8_t SLo[8] = { 0b00000, 0b00000, 0b01110, 0b10000, 0b01110, 0b00001, 0b11110, 0b00000 };
//G                                                                                         //T            
uint8_t GUp[8] = { 0b01110, 0b10001, 0b10000, 0b10111, 0b10001, 0b10001, 0b01110, 0b00000 };uint8_t TUp[8] = { 0b11111, 0b00100, 0b00100, 0b00100, 0b00100, 0b00100, 0b00100, 0b00000 };
uint8_t GLo[8] = { 0b00000, 0b01111, 0b10001, 0b10001, 0b01111, 0b00001, 0b01110, 0b00000 };uint8_t TLo[8] = { 0b01000, 0b01000, 0b11100, 0b01000, 0b01000, 0b01001, 0b00110, 0b00000 };
//H                                                                                         //U                               
uint8_t HUp[8] = { 0b10001, 0b10001, 0b10001, 0b11111, 0b10001, 0b10001, 0b10001, 0b00000 };uint8_t UUp[8] = { 0b10001, 0b10001, 0b10001, 0b10001, 0b10001, 0b10001, 0b01110, 0b00000 };
uint8_t HLo[8] = { 0b10000, 0b10000, 0b10110, 0b11001, 0b10001, 0b10001, 0b10001, 0b00000 };uint8_t ULo[8] = { 0b00000, 0b00000, 0b10001, 0b10001, 0b10001, 0b10011, 0b01101, 0b00000 };
//I                                                                                         //V                                 
uint8_t IUp[8] = { 0b01110, 0b00100, 0b00100, 0b00100, 0b00100, 0b00100, 0b01110, 0b00000 };uint8_t VUp[8] = { 0b10001, 0b10001, 0b10001, 0b10001, 0b01010, 0b01010, 0b00100, 0b00000 };
uint8_t ILo[8] = { 0b00100, 0b00000, 0b01100, 0b00100, 0b00100, 0b00100, 0b01110, 0b00000 };uint8_t VLo[8] = { 0b00000, 0b00000, 0b10001, 0b10001, 0b01010, 0b01010, 0b00100, 0b00000 };
//J                                                                                         //W                           
uint8_t JUp[8] = { 0b00111, 0b00001, 0b00001, 0b00001, 0b10001, 0b10001, 0b01110, 0b00000 };uint8_t WUp[8] = { 0b10001, 0b10001, 0b10001, 0b10101, 0b10101, 0b10101, 0b01010, 0b00000 };
uint8_t JLo[8] = { 0b00010, 0b00000, 0b00110, 0b00010, 0b00010, 0b10010, 0b01100, 0b00000 };uint8_t WLo[8] = { 0b00000, 0b00000, 0b10001, 0b10001, 0b10101, 0b10101, 0b01010, 0b00000 };
//K                                                                                         //X                                 
uint8_t KUp[8] = { 0b10001, 0b10010, 0b10100, 0b11000, 0b10100, 0b10010, 0b10001, 0b00000 };uint8_t XUp[8] = { 0b10001, 0b01010, 0b00100, 0b00100, 0b00100, 0b01010, 0b10001, 0b00000 };
uint8_t KLo[8] = { 0b10000, 0b10000, 0b10010, 0b10100, 0b11000, 0b10100, 0b10010, 0b00000 };uint8_t XLo[8] = { 0b00000, 0b00000, 0b10001, 0b01010, 0b00100, 0b01010, 0b10001, 0b00000 };
//L                                                                                         //Y                              
uint8_t LUp[8] = { 0b10000, 0b10000, 0b10000, 0b10000, 0b10000, 0b10000, 0b11111, 0b00000 };uint8_t YUp[8] = { 0b10001, 0b01010, 0b00100, 0b00100, 0b00100, 0b00100, 0b00100, 0b00000 };
uint8_t LLo[8] = { 0b01100, 0b00100, 0b00100, 0b00100, 0b00100, 0b00100, 0b01110, 0b00000 };uint8_t YLo[8] = { 0b00000, 0b00000, 0b10001, 0b10001, 0b01111, 0b00001, 0b01110, 0b00000 };
//M                                                                                         //Z                                    
uint8_t MUp[8] = { 0b10001, 0b11011, 0b10101, 0b10101, 0b10001, 0b10001, 0b10001, 0b00000 };uint8_t ZUp[8] = { 0b11111, 0b00001, 0b00010, 0b00100, 0b01000, 0b10000, 0b11111, 0b00000 };
uint8_t MLo[8] = { 0b00000, 0b00000, 0b11010, 0b10101, 0b10101, 0b10101, 0b10101, 0b00000 };uint8_t ZLo[8] = { 0b00000, 0b00000, 0b11111, 0b00010, 0b00100, 0b01000, 0b11111, 0b00000 };
//SPACE
uint8_t SPACE[8] = { 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000 };


    uint8_t tmpBit[8] = { 0b00000,0b00000,0b00000,0b00000,0b00000,0b00000,0b00000,0b00000 }; // Blank custom char assigned to CGRAM-slot 0
    uint8_t slicedChar[8] = { 0b00000,0b00000,0b00000,0b00000,0b00000,0b00000,0b00000,0b00000 }; // Blank custom char assigned to CGRAM-slot 8

    char inputStr[] = "Hej";

    for(int i = 0; i < strlen(inputStr); i++)// om element 1 = A skickar vi hela A-Bitmapen till slicedchar
    {   
        if     (inputStr[i]  == 'A'){ memcpy(slicedChar, AUp, sizeof(AUp)); }  else if(inputStr[i]  == 'N'){ memcpy(slicedChar, NUp, sizeof(NUp)); } 
        else if(inputStr[i]  == 'a'){ memcpy(slicedChar, ALo, sizeof(ALo)); }  else if(inputStr[i]  == 'n'){ memcpy(slicedChar, NLo, sizeof(NLo)); }  
        else if(inputStr[i]  == 'B'){ memcpy(slicedChar, BUp, sizeof(BUp)); }  else if(inputStr[i]  == 'O'){ memcpy(slicedChar, OUp, sizeof(OUp)); } 
        else if(inputStr[i]  == 'b'){ memcpy(slicedChar, BLo, sizeof(BLo)); }  else if(inputStr[i]  == 'o'){ memcpy(slicedChar, OLo, sizeof(OLo)); }
        else if(inputStr[i]  == 'C'){ memcpy(slicedChar, CUp, sizeof(CUp)); }  else if(inputStr[i]  == 'P'){ memcpy(slicedChar, PUp, sizeof(PUp)); } 
        else if(inputStr[i]  == 'c'){ memcpy(slicedChar, CLo, sizeof(CLo)); }  else if(inputStr[i]  == 'p'){ memcpy(slicedChar, PLo, sizeof(PLo)); }
        else if(inputStr[i]  == 'D'){ memcpy(slicedChar, DUp, sizeof(DUp)); }  else if(inputStr[i]  == 'Q'){ memcpy(slicedChar, QUp, sizeof(QUp)); } 
        else if(inputStr[i]  == 'd'){ memcpy(slicedChar, DLo, sizeof(DLo)); }  else if(inputStr[i]  == 'q'){ memcpy(slicedChar, QLo, sizeof(QLo)); }
        else if(inputStr[i]  == 'E'){ memcpy(slicedChar, EUp, sizeof(EUp)); }  else if(inputStr[i]  == 'R'){ memcpy(slicedChar, RUp, sizeof(RUp)); } 
        else if(inputStr[i]  == 'e'){ memcpy(slicedChar, ELo, sizeof(ELo)); }  else if(inputStr[i]  == 'r'){ memcpy(slicedChar, RLo, sizeof(RLo)); }
        else if(inputStr[i]  == 'F'){ memcpy(slicedChar, FUp, sizeof(FUp)); }  else if(inputStr[i]  == 'S'){ memcpy(slicedChar, SUp, sizeof(SUp)); } 
        else if(inputStr[i]  == 'f'){ memcpy(slicedChar, FLo, sizeof(FLo)); }  else if(inputStr[i]  == 's'){ memcpy(slicedChar, SLo, sizeof(SLo)); }
        else if(inputStr[i]  == 'G'){ memcpy(slicedChar, GUp, sizeof(GUp)); }  else if(inputStr[i]  == 'T'){ memcpy(slicedChar, TUp, sizeof(TUp)); } 
        else if(inputStr[i]  == 'g'){ memcpy(slicedChar, GLo, sizeof(GLo)); }  else if(inputStr[i]  == 't'){ memcpy(slicedChar, TLo, sizeof(TLo)); }
        else if(inputStr[i]  == 'H'){ memcpy(slicedChar, HUp, sizeof(HUp)); }  else if(inputStr[i]  == 'U'){ memcpy(slicedChar, UUp, sizeof(UUp)); } 
        else if(inputStr[i]  == 'h'){ memcpy(slicedChar, HLo, sizeof(HLo)); }  else if(inputStr[i]  == 'u'){ memcpy(slicedChar, ULo, sizeof(ULo)); }
        else if(inputStr[i]  == 'I'){ memcpy(slicedChar, IUp, sizeof(IUp)); }  else if(inputStr[i]  == 'V'){ memcpy(slicedChar, VUp, sizeof(VUp)); } 
        else if(inputStr[i]  == 'i'){ memcpy(slicedChar, ILo, sizeof(ILo)); }  else if(inputStr[i]  == 'v'){ memcpy(slicedChar, VLo, sizeof(VLo)); }  
        else if(inputStr[i]  == 'J'){ memcpy(slicedChar, JUp, sizeof(JUp)); }  else if(inputStr[i]  == 'W'){ memcpy(slicedChar, WUp, sizeof(WUp)); } 
        else if(inputStr[i]  == 'j'){ memcpy(slicedChar, JLo, sizeof(JLo)); }  else if(inputStr[i]  == 'w'){ memcpy(slicedChar, WLo, sizeof(WLo)); } 
        else if(inputStr[i]  == 'K'){ memcpy(slicedChar, KUp, sizeof(KUp)); }  else if(inputStr[i]  == 'X'){ memcpy(slicedChar, XUp, sizeof(XUp)); } 
        else if(inputStr[i]  == 'k'){ memcpy(slicedChar, KLo, sizeof(KLo)); }  else if(inputStr[i]  == 'x'){ memcpy(slicedChar, XLo, sizeof(XLo)); } 
        else if(inputStr[i]  == 'L'){ memcpy(slicedChar, LUp, sizeof(LUp)); }  else if(inputStr[i]  == 'Y'){ memcpy(slicedChar, YUp, sizeof(YUp)); } 
        else if(inputStr[i]  == 'l'){ memcpy(slicedChar, LLo, sizeof(LLo)); }  else if(inputStr[i]  == 'y'){ memcpy(slicedChar, YLo, sizeof(YLo)); } 
        else if(inputStr[i]  == 'M'){ memcpy(slicedChar, MUp, sizeof(MUp)); }  else if(inputStr[i]  == 'Z'){ memcpy(slicedChar, ZUp, sizeof(ZUp)); } 
        else if(inputStr[i]  == 'm'){ memcpy(slicedChar, MLo, sizeof(MLo)); }  else if(inputStr[i]  == 'z'){ memcpy(slicedChar, ZLo, sizeof(ZLo)); } 
        
        else if(inputStr[i]  == ' '){ memcpy(slicedChar, SPACE, sizeof(SPACE)); }
         
        //uint8_t slicedChar[8];  // All 8 rows(array) of currently iterated char 

            for(int j = 0; j < 8; j++)
                {     
                tmpBit[j] = slicedChar[j];   // tmpBit tar emot 1 bitrow i taget från sliced char
                lcd.CreateChar(0, tmpBit);   // och printar 1 gång för varje rad den tar emot
                lcd.GoTo(i,0);
                lcd.WriteData(0); // 0 = tmpBit      // Varje iteration printar  tmpBit
                _delay_ms(250);
                }                     

        lcd.GoTo(i,0);
        lcd.WriteData(inputStr[i]);
        
                // CGRAM är global och behöver därför att jag ger ett annat värde till varje cell innan jag går vidare
    }
    
