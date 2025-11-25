#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h> 
#include <stdio.h> //printf()
#include <stdbool.h>
#include <string.h> //strlen()
#include <stdlib.h> //rand()
#include <time.h> //time
#include "lcd.h"
#include "uart.h"
#include "customer.h"
#include "texteffects.h"
 
// https://wokwi.com/projects/416241646559459329

#define LED_PIN 2
#define BUTTON_PIN 1

#define BIT_SET(a, b) ((a) |= (1ULL << (b)))
#define BIT_CLEAR(a,b) ((a) &= ~(1ULL<<(b)))
#define BIT_FLIP(a,b) ((a) ^= (1ULL<<(b)))
#define BIT_CHECK(a,b) (!!((a) & (1ULL<<(b)))) 
#define BUTTON_IS_CLICKED(PINB,BUTTON_PIN) !BIT_CHECK(PINB,BUTTON_PIN)
#define DISPLEN 16

uint16_t getRandomSeed (){ //uninitialized ADC value for random seed (AVR ADC produces a 10bit result, hence uint16_t (16bit))
    
    //read ADC0
    ADMUX = (1 << REFS0);                   //Select AVCC (Analog Voltage Supply for ADC) as reference, ADC0 as input
    ADCSRA = (1 << ADEN) | (1 << ADSC);     //set ADEN to 1; enables ADC, and start conversion
    while (ADCSRA & (1 << ADSC));           //Wait for conversion to finish
    uint16_t seed0 = ADC;                   //read result 
    
    //read ADC1
    ADMUX = (1 << REFS0) | 1;               //same as above but ADC1 (by adding "| 1")
    ADCSRA = (1 << ADEN) | (1 << ADSC);
    while (ADCSRA & (1 << ADSC));
    uint16_t seed1 = ADC;

    //Disable for good practice, will otherwise still consume power when idle and ensures no accidental ADC reads later.
    ADCSRA = 0;
    ADMUX = 0;

    uint16_t randomSeed = seed0 ^ seed1; //XOR to mix entropy, basically to mix 2 independent ADC readings for bettter RNG
    
    return randomSeed;
}


int main(void){

    init_serial();
    HD44780 lcd;
    lcd.Initialize(); // Initialize the LCD
    
    srand(getRandomSeed()); // Seed rand with ADC value
    printf("seed: %d\n", getRandomSeed());

    createSpecChar(&lcd);
     
    // create all (5) customers.
    Customer user[5];
    createCustomers(user);

    // get the sum of all payment (used for rand)
    int sum = totalPaid(user);

    int userToPresent = -1;
    
    //discoMan(&lcd);

    while(1){
        userToPresent = randomCustomer(user, sum, userToPresent);
        
        // get a random text (index)
        int textIndex = rand() % user[userToPresent].messagesCount;
        
        printf("Now presenting: %d | Text id: %d\n", userToPresent, textIndex);

        char *txt = user[userToPresent].message[textIndex].message;
      
        fixSpecChar(txt);

        lcd.Clear();

            if (userToPresent == 2){
                if (isEvenMin()){
                    txt = user[2].message[0].message;
                    scrollText(&lcd,txt);
                }
                else{
                    txt = user[2].message[1].message;
                    lcd.GoTo(0,0);
                    lcd.WriteText(txt);

                    for (int i = 0; i < 100; i++){
                    _delay_ms(200);
                    }
                }
            }
            else if (textIndex == 0 && (userToPresent == 0 || userToPresent == 1)){
                scrollText(&lcd, txt);
            }
            else if(userToPresent == 0 && textIndex == 2){
                blinkText(&lcd, txt);
            }
            else if (userToPresent == 3){
                fadeInString(&lcd, txt);
            }
            else if (userToPresent == 4 && textIndex == 0){
                typeAnimation(lcd, txt);
            }
            else if (strlen(txt) > 32){
                splitTxt(lcd, txt);
            }
            else{
                lcd.GoTo(0,0);
                lcd.WriteText(txt);

                for (int i = 0; i < 100; i++){
                _delay_ms(200);
            }
        } 
    }
    return 0;
}
