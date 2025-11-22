#include <stdio.h> //sprintf()
#include <string.h> //strlen()
#include <util/delay.h> //_delay_ms()
#include <time.h> // time elapsed´
#include <util/delay.h>
#include "lcd.h"
#include "customer.h"
#include "texteffects.h"

#define DISPLEN 16

void scrollText(HD44780 *lcd, char *txt){

    // SCROLL FUNCTION + delay
    int cnt = 0;

    // two 'complete scrolls'
    while (cnt < 2){
        cnt++;

        char txt_part[45];
        
        // scroll to txt ends (roll out of screen)
        int lengthOfText = strlen(txt); //skapade en variabel, så att for-loopen inte behöver räkna längden på texten varje gång den körs
        for (int i = 0; i < (lengthOfText + DISPLEN + 1); i++){
            if (i <= DISPLEN){
                snprintf(txt_part, i + 1, txt);
                lcd->GoTo(DISPLEN - i, 0);
                lcd->WriteText((char *)txt_part);    
            } 
            else {
                snprintf(txt_part, DISPLEN + 1, txt + i - DISPLEN);
                lcd->Clear();
                lcd->WriteText((char *) txt_part);
            }
            _delay_ms(130);
        }
    }
}

void blinkText(HD44780 *lcd, char *txt){
    //upprepar 20 gånger: 0,5s synlig + 0,5s tom -> 1s per loop, 20s totalt)
    for (int i = 0; i < 20; i++){
        //först visar det texten
        lcd->Clear();
        lcd->GoTo(0,0);
        lcd->WriteText(txt);
        for (int j = 0; j < 10; j++){
            _delay_ms(50);
        }

        //sedan rensar det skärmen
        lcd->Clear();
        for (int k = 0; k < 10; k++){
            _delay_ms(50);
        }
    }
}

void createSpecChar(HD44780 *lcd){
    // Bitmaps for custom chars Å,Ä,Ö,å,ä,ö
    uint8_t AW[8] = { 0b00100,0b00000,0b01110,0b10001,0b11111,0b10001,0b00000,0b10001 }; // Å
    uint8_t AE[8] = { 0b01010,0b00000,0b01110,0b10001,0b11111,0b10001,0b00000,0b10001 }; // Ä
    uint8_t OO[8] = { 0b01010,0b00000,0b01110,0b10001,0b10001,0b10001,0b01110,0b00000 }; // Ö
    uint8_t aw[8] = { 0b00100,0b01010,0b00100,0b01110,0b00001,0b01111,0b10001,0b01111 }; // å
    uint8_t ae[8] = { 0b01010,0b00000,0b01110,0b00001,0b01111,0b10001,0b01111,0b00000 }; // ä
    uint8_t oo[8] = { 0b01010,0b00000,0b01110,0b10001,0b10001,0b10001,0b01110,0b00000 }; // ö
    
    // Custom chars stored in CGRAM slot 1,2,3,4,5,6 
    // Note: CGRAM Slot 0 is reserved for dynamic char animation
    lcd->CreateChar(1, AW); lcd->CreateChar(2, AE); lcd->CreateChar(3, OO); 
    lcd->CreateChar(4, aw); lcd->CreateChar(5, ae); lcd->CreateChar(6, oo);
}

void FixSpecChar(char *InStr){ // Applies mapping for Å,Ä,Ö,å,ä,ö
    
    int j = 0;
    char OutStr[strlen(InStr) + 1];
    memset(OutStr, 0, sizeof(OutStr));
    
    int lengthOfText = strlen(InStr); //samma gäller som tidigare
    for (int i = 0; i < lengthOfText; i++){   
        if      ((uint8_t)InStr[i] == 0xC3 && (uint8_t)InStr[i+1] == 0x85) {OutStr[j] = 1;j++; i++;} // Å
        else if ((uint8_t)InStr[i] == 0xC3 && (uint8_t)InStr[i+1] == 0x84) {OutStr[j] = 2;j++; i++;} // Ä
        else if ((uint8_t)InStr[i] == 0xC3 && (uint8_t)InStr[i+1] == 0x96) {OutStr[j] = 3;j++; i++;} // Ö
        else if ((uint8_t)InStr[i] == 0xC3 && (uint8_t)InStr[i+1] == 0xA5) {OutStr[j] = 4;j++; i++;} // å
        else if ((uint8_t)InStr[i] == 0xC3 && (uint8_t)InStr[i+1] == 0xA4) {OutStr[j] = 5;j++; i++;} // ä
        else if ((uint8_t)InStr[i] == 0xC3 && (uint8_t)InStr[i+1] == 0xB6) {OutStr[j] = 6;j++; i++;} // ö
        else
        {OutStr[j] = InStr[i];j++;} 
    }

    memset(InStr,0,strlen(InStr) + 1);
    strcpy(InStr,OutStr);
}

void GetBitmap(char inputChar, uint8_t slicedChar[8]) 
{   // Fetch corresponding bitmap for char A-Ö(or a-ö), and store it as 8-element-array, type alias uint8_t
    
    //A                                                                                         //O                                                                                                                             
    uint8_t AUp[8] = {0b01110, 0b10001, 0b10001, 0b11111, 0b10001, 0b10001, 0b10001, 0b00000}; uint8_t OUp[8] = {0b01110, 0b10001, 0b10001, 0b10001, 0b10001, 0b10001, 0b01110, 0b00000};
    uint8_t ALo[8] = {0b00000, 0b01110, 0b00001, 0b01111, 0b10001, 0b10011, 0b01101, 0b00000}; uint8_t OLo[8] = {0b00000, 0b00000, 0b01110, 0b10001, 0b10001, 0b10001, 0b01110, 0b00000};
    // B                                                                                         //P                                                                                                                               
    uint8_t BUp[8] = {0b11110, 0b10001, 0b10001, 0b11110, 0b10001, 0b10001, 0b11110, 0b00000}; uint8_t PUp[8] = {0b11110, 0b10001, 0b10001, 0b11110, 0b10000, 0b10000, 0b10000, 0b00000};
    uint8_t BLo[8] = {0b10000, 0b10000, 0b10110, 0b11001, 0b10001, 0b10001, 0b11110, 0b00000}; uint8_t PLo[8] = {0b00000, 0b00000, 0b11110, 0b10001, 0b10001, 0b11110, 0b10000, 0b10000};
    // C                                                                                         //Q                                         
    uint8_t CUp[8] = {0b01110, 0b10001, 0b10000, 0b10000, 0b10000, 0b10001, 0b01110, 0b00000}; uint8_t QUp[8] = {0b01110, 0b10001, 0b10001, 0b10001, 0b10101, 0b10010, 0b01101, 0b00000};
    uint8_t CLo[8] = {0b00000, 0b01110, 0b10001, 0b10000, 0b10000, 0b10001, 0b01110, 0b00000}; uint8_t QLo[8] = {0b00000, 0b00000, 0b01111, 0b10001, 0b10001, 0b01111, 0b00001, 0b00001};
    // D                                                                                         //R                                            
    uint8_t DUp[8] = {0b11110, 0b10001, 0b10001, 0b10001, 0b10001, 0b10001, 0b11110, 0b00000}; uint8_t RUp[8] = {0b11110, 0b10001, 0b10001, 0b11110, 0b10100, 0b10010, 0b10001, 0b00000};
    uint8_t DLo[8] = {0b00001, 0b00001, 0b01101, 0b10011, 0b10001, 0b10001, 0b01111, 0b00000}; uint8_t RLo[8] = {0b00000, 0b00000, 0b10110, 0b11001, 0b10000, 0b10000, 0b10000, 0b00000};
    // E                                                                                         //S                         
    uint8_t EUp[8] = {0b11111, 0b10000, 0b10000, 0b11110, 0b10000, 0b10000, 0b11111, 0b00000}; uint8_t SUp[8] = {0b01111, 0b10000, 0b10000, 0b01110, 0b00001, 0b00001, 0b11110, 0b00000};
    uint8_t ELo[8] = {0b00000, 0b01110, 0b10001, 0b11111, 0b10000, 0b10001, 0b01110, 0b00000}; uint8_t SLo[8] = {0b00000, 0b00000, 0b01110, 0b10000, 0b01110, 0b00001, 0b11110, 0b00000};
    // F                                                                                         //T           
    uint8_t FUp[8] = {0b11111, 0b10000, 0b10000, 0b11110, 0b10000, 0b10000, 0b10000, 0b00000}; uint8_t TUp[8] = {0b11111, 0b00100, 0b00100, 0b00100, 0b00100, 0b00100, 0b00100, 0b00000};
    uint8_t FLo[8] = {0b00110, 0b01001, 0b01000, 0b11100, 0b01000, 0b01000, 0b01000, 0b00000}; uint8_t TLo[8] = {0b01000, 0b01000, 0b11100, 0b01000, 0b01000, 0b01001, 0b00110, 0b00000};
    // G                                                                                         //U                               
    uint8_t GUp[8] = {0b01110, 0b10001, 0b10000, 0b10111, 0b10001, 0b10001, 0b01110, 0b00000}; uint8_t UUp[8] = {0b10001, 0b10001, 0b10001, 0b10001, 0b10001, 0b10001, 0b01110, 0b00000};
    uint8_t GLo[8] = {0b00000, 0b01111, 0b10001, 0b10001, 0b01111, 0b00001, 0b01110, 0b00000}; uint8_t ULo[8] = {0b00000, 0b00000, 0b10001, 0b10001, 0b10001, 0b10011, 0b01101, 0b00000};
    // H                                                                                         //V                                 
    uint8_t HUp[8] = {0b10001, 0b10001, 0b10001, 0b11111, 0b10001, 0b10001, 0b10001, 0b00000}; uint8_t VUp[8] = {0b10001, 0b10001, 0b10001, 0b10001, 0b01010, 0b01010, 0b00100, 0b00000};
    uint8_t HLo[8] = {0b10000, 0b10000, 0b10110, 0b11001, 0b10001, 0b10001, 0b10001, 0b00000}; uint8_t VLo[8] = {0b00000, 0b00000, 0b10001, 0b10001, 0b01010, 0b01010, 0b00100, 0b00000};
    // I                                                                                         //W                           
    uint8_t IUp[8] = {0b01110, 0b00100, 0b00100, 0b00100, 0b00100, 0b00100, 0b01110, 0b00000}; uint8_t WUp[8] = {0b10001, 0b10001, 0b10001, 0b10101, 0b10101, 0b10101, 0b01010, 0b00000};
    uint8_t ILo[8] = {0b00100, 0b00000, 0b01100, 0b00100, 0b00100, 0b00100, 0b01110, 0b00000}; uint8_t WLo[8] = {0b00000, 0b00000, 0b10001, 0b10001, 0b10101, 0b10101, 0b01010, 0b00000};
    // J                                                                                         //X                                 
    uint8_t JUp[8] = {0b00111, 0b00001, 0b00001, 0b00001, 0b10001, 0b10001, 0b01110, 0b00000}; uint8_t XUp[8] = {0b10001, 0b01010, 0b00100, 0b00100, 0b00100, 0b01010, 0b10001, 0b00000};
    uint8_t JLo[8] = {0b00010, 0b00000, 0b00110, 0b00010, 0b00010, 0b10010, 0b01100, 0b00000}; uint8_t XLo[8] = {0b00000, 0b00000, 0b10001, 0b01010, 0b00100, 0b01010, 0b10001, 0b00000};
    // K                                                                                         //Y                              
    uint8_t KUp[8] = {0b10001, 0b10010, 0b10100, 0b11000, 0b10100, 0b10010, 0b10001, 0b00000}; uint8_t YUp[8] = {0b10001, 0b01010, 0b00100, 0b00100, 0b00100, 0b00100, 0b00100, 0b00000};
    uint8_t KLo[8] = {0b10000, 0b10000, 0b10010, 0b10100, 0b11000, 0b10100, 0b10010, 0b00000}; uint8_t YLo[8] = {0b00000, 0b00000, 0b10001, 0b10001, 0b01111, 0b00001, 0b01110, 0b00000};
    // L                                                                                         //Z                                    
    uint8_t LUp[8] = {0b10000, 0b10000, 0b10000, 0b10000, 0b10000, 0b10000, 0b11111, 0b00000}; uint8_t ZUp[8] = {0b11111, 0b00001, 0b00010, 0b00100, 0b01000, 0b10000, 0b11111, 0b00000};
    uint8_t LLo[8] = {0b01100, 0b00100, 0b00100, 0b00100, 0b00100, 0b00100, 0b01110, 0b00000}; uint8_t ZLo[8] = {0b00000, 0b00000, 0b11111, 0b00010, 0b00100, 0b01000, 0b11111, 0b00000};                                                                                                 
    // M                                                                                         //Å
    uint8_t MUp[8] = {0b10001, 0b11011, 0b10101, 0b10101, 0b10001, 0b10001, 0b10001, 0b00000}; uint8_t AW[8] = {0b00100, 0b00000, 0b01110, 0b10001, 0b11111, 0b10001, 0b00000, 0b10001};
    uint8_t MLo[8] = {0b00000, 0b00000, 0b11010, 0b10101, 0b10101, 0b10101, 0b10101, 0b00000}; uint8_t aw[8] = {0b00100, 0b01010, 0b00100, 0b01110, 0b00001, 0b01111, 0b10001, 0b01111};
    // N                                                                                         //Ä  
    uint8_t NUp[8] = {0b10001, 0b10001, 0b11001, 0b10101, 0b10011, 0b10001, 0b10001, 0b00000}; uint8_t AE[8] = {0b01010, 0b00000, 0b01110, 0b10001, 0b11111, 0b10001, 0b00000, 0b10001};
    uint8_t NLo[8] = {0b00000, 0b00000, 0b10110, 0b11001, 0b10001, 0b10001, 0b10001, 0b00000}; uint8_t ae[8] = {0b01010, 0b00000, 0b01110, 0b00001, 0b01111, 0b10001, 0b01111, 0b00000};
                                                                                                 //Ö
                                                                                                uint8_t OO[8] = {0b01010, 0b00000, 0b01110, 0b10001, 0b10001, 0b10001, 0b01110, 0b00000};
    /*SPACE*/                                                                                   uint8_t oo[8] = {0b01010, 0b00000, 0b01110, 0b10001, 0b10001, 0b10001, 0b01110, 0b00000};
    uint8_t SPACE[8] = {0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000}; 
   
    switch (inputChar)
        {
        case 'A': memcpy(slicedChar, AUp, sizeof(AUp));break;   
        case 'a': memcpy(slicedChar, ALo, sizeof(ALo));break;    
        case 'B': memcpy(slicedChar, BUp, sizeof(BUp));break;   
        case 'b': memcpy(slicedChar, BLo, sizeof(BLo));break;  
        case 'C': memcpy(slicedChar, CUp, sizeof(CUp));break;   
        case 'c': memcpy(slicedChar, CLo, sizeof(CLo));break;  
        case 'D': memcpy(slicedChar, DUp, sizeof(DUp));break;   
        case 'd': memcpy(slicedChar, DLo, sizeof(DLo));break;  
        case 'E': memcpy(slicedChar, EUp, sizeof(EUp));break;   
        case 'e': memcpy(slicedChar, ELo, sizeof(ELo));break;  
        case 'F': memcpy(slicedChar, FUp, sizeof(FUp));break;   
        case 'f': memcpy(slicedChar, FLo, sizeof(FLo));break;  
        case 'G': memcpy(slicedChar, GUp, sizeof(GUp));break;   
        case 'g': memcpy(slicedChar, GLo, sizeof(GLo));break;  
        case 'H': memcpy(slicedChar, HUp, sizeof(HUp));break;   
        case 'h': memcpy(slicedChar, HLo, sizeof(HLo));break;  
        case 'I': memcpy(slicedChar, IUp, sizeof(IUp));break;   
        case 'i': memcpy(slicedChar, ILo, sizeof(ILo));break;    
        case 'J': memcpy(slicedChar, JUp, sizeof(JUp));break;   
        case 'j': memcpy(slicedChar, JLo, sizeof(JLo));break;   
        case 'K': memcpy(slicedChar, KUp, sizeof(KUp));break;   
        case 'k': memcpy(slicedChar, KLo, sizeof(KLo));break;   
        case 'L': memcpy(slicedChar, LUp, sizeof(LUp));break;   
        case 'l': memcpy(slicedChar, LLo, sizeof(LLo));break;   
        case 'M': memcpy(slicedChar, MUp, sizeof(MUp));break;   
        case 'm': memcpy(slicedChar, MLo, sizeof(MLo));break;  
        case 'N': memcpy(slicedChar, NUp, sizeof(NUp));break;
        case 'n': memcpy(slicedChar, NLo, sizeof(NLo));break;
        case 'O': memcpy(slicedChar, OUp, sizeof(OUp));break;
        case 'o': memcpy(slicedChar, OLo, sizeof(OLo));break;
        case 'P': memcpy(slicedChar, PUp, sizeof(PUp));break;
        case 'p': memcpy(slicedChar, PLo, sizeof(PLo));break;
        case 'Q': memcpy(slicedChar, QUp, sizeof(QUp));break;
        case 'q': memcpy(slicedChar, QLo, sizeof(QLo));break;
        case 'R': memcpy(slicedChar, RUp, sizeof(RUp));break;
        case 'r': memcpy(slicedChar, RLo, sizeof(RLo));break;
        case 'S': memcpy(slicedChar, SUp, sizeof(SUp));break;
        case 's': memcpy(slicedChar, SLo, sizeof(SLo));break;
        case 'T': memcpy(slicedChar, TUp, sizeof(TUp));break;
        case 't': memcpy(slicedChar, TLo, sizeof(TLo));break;
        case 'U': memcpy(slicedChar, UUp, sizeof(UUp));break;
        case 'u': memcpy(slicedChar, ULo, sizeof(ULo));break;
        case 'V': memcpy(slicedChar, VUp, sizeof(VUp));break;
        case 'v': memcpy(slicedChar, VLo, sizeof(VLo));break;
        case 'W': memcpy(slicedChar, WUp, sizeof(WUp));break;
        case 'w': memcpy(slicedChar, WLo, sizeof(WLo));break;
        case 'X': memcpy(slicedChar, XUp, sizeof(XUp));break;
        case 'x': memcpy(slicedChar, XLo, sizeof(XLo));break;
        case 'Y': memcpy(slicedChar, YUp, sizeof(YUp));break;
        case 'y': memcpy(slicedChar, YLo, sizeof(YLo));break;
        case 'Z': memcpy(slicedChar, ZUp, sizeof(ZUp));break;
        case 'z': memcpy(slicedChar, ZLo, sizeof(ZLo));break;  
        case 1: memcpy(slicedChar, AW, sizeof(AW));break;
        case 2: memcpy(slicedChar, aw, sizeof(aw));break;
        case 3: memcpy(slicedChar, AE, sizeof(AE));break;
        case 4: memcpy(slicedChar, ae, sizeof(ae));break;
        case 5: memcpy(slicedChar, OO, sizeof(OO));break;
        case 6: memcpy(slicedChar, oo, sizeof(oo));break;
        case ' ': memcpy(slicedChar, SPACE, sizeof(SPACE));break;
        default: memcpy(slicedChar, SPACE, sizeof(SPACE));break;
    }        
}


void FadeInString(HD44780 *lcd, char *txt) 
{
    char *inputStr = txt;   
                       
    lcd->Clear();
    lcd->GoTo(0,0);
    //FixSpecChar(inputStr);

    int breakPoint;
    breakPoint = CleanBreak(inputStr); // Use breakPoint as rowbreak
    
    uint8_t slicedChar[8] = { 0 }; 

    //time_t start = time(NULL);

    for(int i = 0; i < strlen(inputStr); i++)   
        {   
            uint8_t tmpBit[8] = { 0 };
            GetBitmap(inputStr[i],slicedChar);      // Expects string[i], uint8_t[8]

            int Row;            // Position variable Y-axis

            for(int j = 0; j < 4; j++)              
                {    
                    tmpBit[j] = slicedChar[j];             // tmpBit recieves 1 pixelrow(per iteration) from slicedChar
                    tmpBit[j+1] = slicedChar[j+1];             // tmpBit recieves 1 pixelrow(per iteration) from slicedChar
                    lcd->CreateChar(0, tmpBit);               // Stores current state of char to CGRAM(custom char lib), slot 0             
                    if     (i<=breakPoint)  {Row = 0; lcd->GoTo(i,Row); }     // Forces WriteData to remain on current  
                    else if(i>breakPoint) {Row = 1; lcd->GoTo(i-(breakPoint+1),Row);}      // pos until all pixelrows are printed
                    lcd->WriteData(0);                           // Prints tmpBit
                }    

            if     (i<=breakPoint)  {Row = 0; lcd->GoTo(i,Row); } 
            else if(i>breakPoint) {Row = 1; lcd->GoTo(i-(breakPoint+1),Row); } 
            lcd->WriteData(inputStr[i]);  printf("%d",i);      // Prints char from CGROM(standard char memory)       
        }
        
    // time_t end = time(NULL);     
    
    // double secCnt = difftime(end, start);  // Total duration of print

    // double maxDur = 20.0;       // 20 sec

    // unsigned long remainingMs = (unsigned long)((maxDur - secCnt) * 1000);

    // for(unsigned int t = 0; t < (remainingMs) * 1000; t++) 
    // {_delay_ms(1);}

    // lcd->Clear();                                                          
} 


int CleanBreak(char *inputStr) // Prevents row break mid-word 
{
    int breakPoint;
        for (int k = 15; k > 0; k--)
            {
                if(inputStr[k] == ' '){breakPoint = k; break;}     
            }      
    return breakPoint; // Counts down from end of row 1 until 'space' is found and returns value 
}

void DiscoMan(HD44780 *lcd)
{                                                                                         // Bitmaps:
    uint8_t dManR[8] =  { 0b00110,0b00110,0b01001,0b01011,0b00110,0b00110,0b00101,0b01101 };    // Discoman right position(startposition)   
    uint8_t dManL[8] =  { 0b01100,0b01100,0b10010,0b10110,0b01100,0b01100,0b01010,0b11011 };    // Discoman left position     
    uint8_t dManOpen[8] =  { 0b00011,0b00011,0b01001,0b01011,0b00110,0b00110,0b00101,0b01101 }; // Discoman opening   
    uint8_t dManFin[8] =  { 0b00000,0b00000,0b00000,0b00000,0b00000,0b00000,0b00000,0b00000 };  // Discoman finale 
    uint8_t slicedChar[8] = { 0b00000,0b00000,0b00000,0b00000,0b00000,0b00000,0b00000,0b00000 }; // Blank bitmap for storing current j feed of pixelrows
                                                                   
                                                    
    char strSpace[] = "                                "; // Array of 32 spaces for iterating and printing spaces 
    lcd->Clear(); 
                               
    for(int i = 0; i < strlen(strSpace); i++)
        { printf("i count: %d \n",i);
                           
          int Rowz;  // Variable for controlling displayposition Y    
                                 
            for(int j = 0; j < 8; j++)
                {  
                            printf("j: %d ",j); // Errorhandler 
                                                            // slicedChar gets 1 pixel row from desired bitmap per iteration         
                    if                (i<=28 && i%2==0) { memcpy(&slicedChar[j], &dManR[j], sizeof(uint8_t)); }     
                    else if(i <  15          && i%2==1) { memcpy(&slicedChar[j], &dManOpen[j], sizeof(uint8_t)); }  
                    else if(i >= 15 && i<=28 && i%2==1) { memcpy(&slicedChar[j], &dManL[j], sizeof(uint8_t)); }    
                    else if(i> 28)                      { memcpy(&slicedChar[j], &dManFin[j], sizeof(uint8_t)); }      
                    
                    
                    lcd->CreateChar(0, slicedChar);   // Assign current iteration content to CGRAM(custom char mem) slot 0 
                    
                    if     (i%2==0) { Rowz = 0; lcd->GoTo(i+1,Rowz); }  // Move position 
                    else if(i%2==1) { Rowz = 1; lcd->GoTo(i+1,Rowz); } 
                    
                    lcd->WriteData(0);                 // Print slot 0
                }
                             
                                                                                       // Print in zic-zac pattern by:    
            if     (i%2==0) { Rowz = 0; lcd->GoTo(i,Rowz); lcd->WriteData(strSpace[i]);}     // writing space on top row when cell count = even 
            else if(i%2==1) { Rowz = 1; lcd->GoTo(i,Rowz); lcd->WriteData(strSpace[i]);}     // writing space on bottom row when cell count = odd        
        }
    lcd->Clear();         
}