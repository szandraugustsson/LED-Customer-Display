#include <stdio.h> //sprintf()
#include <string.h> //strlen()
#include <util/delay.h> //_delay_ms()
#include <time.h> // time elapsed
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
    uint8_t aw[8] = { 0b00100,0b00000,0b01110,0b00001,0b01111,0b10001,0b01111,0b00000 }; // å
    uint8_t ae[8] = { 0b01010,0b00000,0b01110,0b00001,0b01111,0b10001,0b01111,0b00000 }; // ä
    uint8_t oo[8] = { 0b01010,0b00000,0b01110,0b10001,0b10001,0b10001,0b01110,0b00000 }; // ö
    
    // Custom chars stored in CGRAM slot 1,2,3,4,5,6 
    // Note: CGRAM Slot 0 is reserved for dynamic char animation
    lcd->CreateChar(1, AW); lcd->CreateChar(2, AE); lcd->CreateChar(3, OO); 
    lcd->CreateChar(4, aw); lcd->CreateChar(5, ae); lcd->CreateChar(6, oo);
}

void fixSpecChar(char *InStr){ // Applies mapping for Å,Ä,Ö,å,ä,ö
    
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

const uint8_t* getBitmap(char inputChar) 
{   // Fetch corresponding bitmap for char A-Ö(or a-ö), and store it as 8-element-array, type alias uint8_t
    
    //A                                                                                         //O                                                                                                                             
    static const uint8_t AUp[8] = {0b01110, 0b10001, 0b10001, 0b11111, 0b10001, 0b10001, 0b10001, 0b00000}; static const uint8_t OUp[8] = {0b01110, 0b10001, 0b10001, 0b10001, 0b10001, 0b10001, 0b01110, 0b00000};
    static const uint8_t ALo[8] = {0b00000, 0b01110, 0b00001, 0b01111, 0b10001, 0b10011, 0b01101, 0b00000}; static const uint8_t OLo[8] = {0b00000, 0b00000, 0b01110, 0b10001, 0b10001, 0b10001, 0b01110, 0b00000};
    // B                                                                                         //P                                                                                                                               
    static const uint8_t BUp[8] = {0b11110, 0b10001, 0b10001, 0b11110, 0b10001, 0b10001, 0b11110, 0b00000}; static const uint8_t PUp[8] = {0b11110, 0b10001, 0b10001, 0b11110, 0b10000, 0b10000, 0b10000, 0b00000};
    static const uint8_t BLo[8] = {0b10000, 0b10000, 0b10110, 0b11001, 0b10001, 0b10001, 0b11110, 0b00000}; static const uint8_t PLo[8] = {0b00000, 0b00000, 0b11110, 0b10001, 0b10001, 0b11110, 0b10000, 0b10000};
    // C                                                                                         //Q                                         
    static const uint8_t CUp[8] = {0b01110, 0b10001, 0b10000, 0b10000, 0b10000, 0b10001, 0b01110, 0b00000}; static const uint8_t QUp[8] = {0b01110, 0b10001, 0b10001, 0b10001, 0b10101, 0b10010, 0b01101, 0b00000};
    static const uint8_t CLo[8] = {0b00000, 0b01110, 0b10001, 0b10000, 0b10000, 0b10001, 0b01110, 0b00000}; static const uint8_t QLo[8] = {0b00000, 0b00000, 0b01111, 0b10001, 0b10001, 0b01111, 0b00001, 0b00001};
    // D                                                                                         //R                                            
    static const uint8_t DUp[8] = {0b11110, 0b10001, 0b10001, 0b10001, 0b10001, 0b10001, 0b11110, 0b00000}; static const uint8_t RUp[8] = {0b11110, 0b10001, 0b10001, 0b11110, 0b10100, 0b10010, 0b10001, 0b00000};
    static const uint8_t DLo[8] = {0b00001, 0b00001, 0b01101, 0b10011, 0b10001, 0b10001, 0b01111, 0b00000}; static const uint8_t RLo[8] = {0b00000, 0b00000, 0b10110, 0b11001, 0b10000, 0b10000, 0b10000, 0b00000};
    // E                                                                                         //S                         
    static const uint8_t EUp[8] = {0b11111, 0b10000, 0b10000, 0b11110, 0b10000, 0b10000, 0b11111, 0b00000}; static const uint8_t SUp[8] = {0b01111, 0b10000, 0b10000, 0b01110, 0b00001, 0b00001, 0b11110, 0b00000};
    static const uint8_t ELo[8] = {0b00000, 0b01110, 0b10001, 0b11111, 0b10000, 0b10001, 0b01110, 0b00000}; static const uint8_t SLo[8] = {0b00000, 0b00000, 0b01110, 0b10000, 0b01110, 0b00001, 0b11110, 0b00000};
    // F                                                                                         //T           
    static const uint8_t FUp[8] = {0b11111, 0b10000, 0b10000, 0b11110, 0b10000, 0b10000, 0b10000, 0b00000}; static const uint8_t TUp[8] = {0b11111, 0b00100, 0b00100, 0b00100, 0b00100, 0b00100, 0b00100, 0b00000};
    static const uint8_t FLo[8] = {0b00110, 0b01001, 0b01000, 0b11100, 0b01000, 0b01000, 0b01000, 0b00000}; static const uint8_t TLo[8] = {0b01000, 0b01000, 0b11100, 0b01000, 0b01000, 0b01001, 0b00110, 0b00000};
    // G                                                                                         //U                               
    static const uint8_t GUp[8] = {0b01110, 0b10001, 0b10000, 0b10111, 0b10001, 0b10001, 0b01110, 0b00000}; static const uint8_t UUp[8] = {0b10001, 0b10001, 0b10001, 0b10001, 0b10001, 0b10001, 0b01110, 0b00000};
    static const uint8_t GLo[8] = {0b00000, 0b01111, 0b10001, 0b10001, 0b01111, 0b00001, 0b01110, 0b00000}; static const uint8_t ULo[8] = {0b00000, 0b00000, 0b10001, 0b10001, 0b10001, 0b10011, 0b01101, 0b00000};
    // H                                                                                         //V                                 
    static const uint8_t HUp[8] = {0b10001, 0b10001, 0b10001, 0b11111, 0b10001, 0b10001, 0b10001, 0b00000}; static const uint8_t VUp[8] = {0b10001, 0b10001, 0b10001, 0b10001, 0b01010, 0b01010, 0b00100, 0b00000};
    static const uint8_t HLo[8] = {0b10000, 0b10000, 0b10110, 0b11001, 0b10001, 0b10001, 0b10001, 0b00000}; static const uint8_t VLo[8] = {0b00000, 0b00000, 0b10001, 0b10001, 0b01010, 0b01010, 0b00100, 0b00000};
    // I                                                                                         //W                           
    static const uint8_t IUp[8] = {0b01110, 0b00100, 0b00100, 0b00100, 0b00100, 0b00100, 0b01110, 0b00000}; static const uint8_t WUp[8] = {0b10001, 0b10001, 0b10001, 0b10101, 0b10101, 0b10101, 0b01010, 0b00000};
    static const uint8_t ILo[8] = {0b00100, 0b00000, 0b01100, 0b00100, 0b00100, 0b00100, 0b01110, 0b00000}; static const uint8_t WLo[8] = {0b00000, 0b00000, 0b10001, 0b10001, 0b10101, 0b10101, 0b01010, 0b00000};
    // J                                                                                         //X                                 
    static const uint8_t JUp[8] = {0b00111, 0b00001, 0b00001, 0b00001, 0b10001, 0b10001, 0b01110, 0b00000}; static const uint8_t XUp[8] = {0b10001, 0b01010, 0b00100, 0b00100, 0b00100, 0b01010, 0b10001, 0b00000};
    static const uint8_t JLo[8] = {0b00010, 0b00000, 0b00110, 0b00010, 0b00010, 0b10010, 0b01100, 0b00000}; static const uint8_t XLo[8] = {0b00000, 0b00000, 0b10001, 0b01010, 0b00100, 0b01010, 0b10001, 0b00000};
    // K                                                                                         //Y                              
    static const uint8_t KUp[8] = {0b10001, 0b10010, 0b10100, 0b11000, 0b10100, 0b10010, 0b10001, 0b00000}; static const uint8_t YUp[8] = {0b10001, 0b01010, 0b00100, 0b00100, 0b00100, 0b00100, 0b00100, 0b00000};
    static const uint8_t KLo[8] = {0b10000, 0b10000, 0b10010, 0b10100, 0b11000, 0b10100, 0b10010, 0b00000}; static const uint8_t YLo[8] = {0b00000, 0b00000, 0b10001, 0b10001, 0b01111, 0b00001, 0b01110, 0b00000};
    // L                                                                                         //Z                                    
    static const uint8_t LUp[8] = {0b10000, 0b10000, 0b10000, 0b10000, 0b10000, 0b10000, 0b11111, 0b00000}; static const uint8_t ZUp[8] = {0b11111, 0b00001, 0b00010, 0b00100, 0b01000, 0b10000, 0b11111, 0b00000};
    static const uint8_t LLo[8] = {0b01100, 0b00100, 0b00100, 0b00100, 0b00100, 0b00100, 0b01110, 0b00000}; static const uint8_t ZLo[8] = {0b00000, 0b00000, 0b11111, 0b00010, 0b00100, 0b01000, 0b11111, 0b00000};                                                                                                 
    // M                                                                                         //Å
    static const uint8_t MUp[8] = {0b10001, 0b11011, 0b10101, 0b10101, 0b10001, 0b10001, 0b10001, 0b00000}; static const uint8_t AW[8] = {0b00100, 0b00000, 0b01110, 0b10001, 0b11111, 0b10001, 0b00000, 0b10001};
    static const uint8_t MLo[8] = {0b00000, 0b00000, 0b11010, 0b10101, 0b10101, 0b10101, 0b10101, 0b00000}; static const uint8_t aw[8] = {0b00100, 0b00000, 0b01110, 0b00001, 0b01111, 0b10001, 0b01111, 0b00000};
    // N                                                                                         //Ä  
    static const uint8_t NUp[8] = {0b10001, 0b10001, 0b11001, 0b10101, 0b10011, 0b10001, 0b10001, 0b00000}; static const uint8_t AE[8] = {0b01010, 0b00000, 0b01110, 0b10001, 0b11111, 0b10001, 0b00000, 0b10001};
    static const uint8_t NLo[8] = {0b00000, 0b00000, 0b10110, 0b11001, 0b10001, 0b10001, 0b10001, 0b00000}; static const uint8_t ae[8] = {0b01010, 0b00000, 0b01110, 0b00001, 0b01111, 0b10001, 0b01111, 0b00000};
                                                                                                 //Ö
                                                                                                static const uint8_t OO[8] = {0b01010, 0b00000, 0b01110, 0b10001, 0b10001, 0b10001, 0b01110, 0b00000};
    /*SPACE*/                                                                                   static const uint8_t oo[8] = {0b01010, 0b00000, 0b01110, 0b10001, 0b10001, 0b10001, 0b01110, 0b00000};
    static const uint8_t SPACE[8] = {0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000}; 
   
    switch (inputChar)
        {
        case 'A': return AUp;   
        case 'a': return ALo;    
        case 'B': return BUp;   
        case 'b': return BLo;  
        case 'C': return CUp;   
        case 'c': return CLo;  
        case 'D': return DUp;   
        case 'd': return DLo;  
        case 'E': return EUp;   
        case 'e': return ELo;  
        case 'F': return FUp;   
        case 'f': return FLo;  
        case 'G': return GUp;   
        case 'g': return GLo;  
        case 'H': return HUp;   
        case 'h': return HLo;  
        case 'I': return IUp;   
        case 'i': return ILo;    
        case 'J': return JUp;   
        case 'j': return JLo;   
        case 'K': return KUp;   
        case 'k': return KLo;   
        case 'L': return LUp;   
        case 'l': return LLo;   
        case 'M': return MUp;   
        case 'm': return MLo;  
        case 'N': return NUp;
        case 'n': return NLo;
        case 'O': return OUp;
        case 'o': return OLo;
        case 'P': return PUp;
        case 'p': return PLo;
        case 'Q': return QUp;
        case 'q': return QLo;
        case 'R': return RUp;
        case 'r': return RLo;
        case 'S': return SUp;
        case 's': return SLo;
        case 'T': return TUp;
        case 't': return TLo;
        case 'U': return UUp;
        case 'u': return ULo;
        case 'V': return VUp;
        case 'v': return VLo;
        case 'W': return WUp;
        case 'w': return WLo;
        case 'X': return XUp;
        case 'x': return XLo;
        case 'Y': return YUp;
        case 'y': return YLo;
        case 'Z': return ZUp;
        case 'z': return ZLo;  
        case 1: return AW;
        case 2: return aw;
        case 3: return AE;
        case 4: return ae;
        case 5: return OO;
        case 6: return oo;
        case ' ': return SPACE;
        default: return SPACE;
    }        
}

void fadeInString(HD44780 *lcd, char *txt) 
{                  
    lcd->Clear();
    lcd->GoTo(0,0);

    int breakPoint = cleanBreak(txt); // Use breakPoint as rowbreak

    int txtLen = strlen(txt);

    for(int i = 0; i < txtLen; i++)   
        {   
            uint8_t tmpBit[8] = { 0 };

            const uint8_t* slicedChar = getBitmap(txt[i]);      // Expects string[i], uint8_t[8]

            int Row;          // Position variable Y-axis

            for(int j = 0; j < 5; j++)              
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
            lcd->WriteData(txt[i]);      // Prints char from CGROM(standard char memory)       
        }
    
    unsigned int delay = 56 - txtLen;
    for(unsigned int t = 0; t < delay; t++) 
    {_delay_ms(301);}               
                                     
    lcd->Clear();                                                          
} 

int cleanBreak(char *txt) // Prevents row break mid-word 
{
    int breakPoint;
        for (int k = 15; k > 0; k--)
            {
                if(txt[k] == ' '){breakPoint = k; break;}     
            }      
    return breakPoint; // Counts down from end of row 1 until 'space' is found and returns value 
}

void discoMan(HD44780 *lcd)
{                                                                                         // Bitmaps:
    uint8_t dManR[8] =  { 0b00110,0b00110,0b01001,0b01011,0b00110,0b00110,0b00101,0b01101 };    // Discoman right position(startposition)   
    uint8_t dManL[8] =  { 0b01100,0b01100,0b10010,0b10110,0b01100,0b01100,0b01010,0b11011 };    // Discoman left position     
    uint8_t dManOpen[8] =  { 0b00011,0b00011,0b01001,0b01011,0b00110,0b00110,0b00101,0b01101 }; // Discoman opening   
    uint8_t dManFin[8] =  { 0b00000,0b00000,0b00000,0b00000,0b00000,0b00000,0b00000,0b00000 };  // Discoman finale 
    uint8_t slicedChar[8] = { 0b00000,0b00000,0b00000,0b00000,0b00000,0b00000,0b00000,0b00000 }; // Blank bitmap for storing current j feed of pixelrows
                                                                   
    lcd->Clear(); 
                                                    
    char strSpace[] = "                                "; // Array of 32 spaces for iterating and printing spaces 

    for(int i = 0; i < strlen(strSpace); i++)
        { //printf("i count: %d \n",i);
                           
          int Rowz;  // Variable for controlling displayposition Y    
                                 
            for(int j = 0; j < 8; j++)
                {  
                            //printf("j: %d ",j); // Errorhandler 
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

//Animation to "transition" into new text (it sucks and does not work)
void sweepAnimation(HD44780 &lcd) {
    const char sweepChar = '#';
    const char blankChar = ' ';

    //sweep forward
    for(int i = 0; i < 16; i++) {
        lcd.GoTo(i, 0);
        lcd.WriteData(sweepChar);

        lcd.GoTo(i, 1);
        lcd.WriteData(sweepChar);

        _delay_ms(20);
    }
    //sweep backward (clear)
    for(int i = 15; i >= 0; i--) {
        lcd.GoTo(i, 0);
        lcd.WriteData(blankChar);

        lcd.GoTo(i, 1);
        lcd.WriteData(blankChar);

        _delay_ms(20);
    }
}
void randomDelay() {
    int r = rand() % 3; //Pick random delay to simulate real typing
    switch(r) {
        case 0:
            return _delay_ms(70);
        case 1:
            return _delay_ms(100);
        case 2:
            return _delay_ms(130);
        default:
            return _delay_ms(100);
    }
}

//text-animation to simulate typing (version 3)
void typeAnimation(HD44780 &lcd, char* txt) {
    
    for (int repeat = 0; repeat < 2; repeat++){
        lcd.Clear();
        lcd.GoTo(0, 0);
        
        const char* c = txt;
        int col = 0;
        int row = 0;

        while (*c) {
            lcd.WriteData(*c);
            randomDelay();
            if (*c == ',' || *c == '.' || *c == '!' || *c == '?') {
                _delay_ms(500); // Longer delay at punctuations
            }
            col++;
            c++;
            if (col >= 16) {
                col = 0;
                row++;
                if (row >= 2) break;
                lcd.GoTo(0, row);
            }
        }
        _delay_ms(2000);
    }
}

// create a "split" presentation (for static text, <32 chars)
void splitTxt(HD44780 &lcd, char *txt){

    for (int i = 0; i<4; i++){
        // find the last space
        char *lastSpace = strrchr(txt, ' ');
        char firstPrint[33];

        lcd.GoTo(0, 0);
        snprintf(firstPrint, lastSpace-txt +1 , txt);
        lcd.WriteText((char *) firstPrint );
        _delay_ms(3000);
        lcd.Clear();

        lcd.WriteText((char *) lastSpace + 1);
        _delay_ms(2000);
    }
}

bool isEvenMin(){
    //först vill jag få den aktuella tiden:
    time_t currentTime = time(NULL); //den lagrar tiden i sekunder sen 1970
    int totalMinutes = currentTime / 60; //hur många minuter har gått totalt
    int totalHours = totalMinutes / 60; //hur många hela timmar har gått
    int minutes = totalMinutes % 60; //får minutvärdet i den aktuella timmen
    return (minutes % 2 == 0);
    //true - jämn
}