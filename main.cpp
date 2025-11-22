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

int main(void){

    init_serial();
    HD44780 lcd;
    lcd.Initialize(); // Initialize the LCD
    srand(time(NULL)); // Seed rand with sys clock to avoid same rand seq 
    createSpecChar(&lcd);
    
    // create all (5) customers.
    Customer user[5];
    createCustomers(user);

    // get the sum of all payment (used for rand)
    int sum = totalPaid(user);

    int userToPresent = -1;
    DiscoMan(&lcd);
    while(1){
        userToPresent = randomCustomer(user, sum, userToPresent);
        
        // get a random text (index)
        int textIndex = rand() % user[userToPresent].messagesCount;

        char *txt = user[userToPresent].message[textIndex].message;
        
        FixSpecChar(txt);
        lcd.Clear();

        printf("Now presenting: %d | Text id: %d\n", userToPresent, textIndex);

        if (textIndex == 0 && (userToPresent == 0 || userToPresent == 1 || userToPresent == 2)){
            scrollText(&lcd, txt);
        }
        else if(userToPresent == 0 && textIndex == 2){
            blinkText(&lcd, txt);
        }
        else if (userToPresent == 3){
            FadeInString(&lcd, txt);
        }
        else{
            lcd.GoTo(0,0);
            lcd.WriteText(txt);

            for (int i = 0; i < 100; i++){
                _delay_ms(50);
            }
        }
    }
    return 0;
}
