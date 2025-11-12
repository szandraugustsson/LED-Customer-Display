#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h> 
#include <stdio.h> //printf()
#include <stdbool.h>
#include <string.h> //strlen()
#include <stdlib.h> //rand()
#include "lcd.h"
#include "uart.h"
#include "customer.h"
#include "type.h"


void typeAnimation() {
    init_serial();
    HD44780 lcd;
    lcd.Initialize(); // Initialize the LCD

    //create customers and pick random message
    Customer user[5];
    createCustomers(user);
    int sum = totalPaid(user);
    int userToPresent = -1;
    userToPresent = randomCustomer(user, sum, userToPresent);
    int textIndex = rand() % user[userToPresent].messagesCount;

    const char* msg = user[userToPresent].message[textIndex].message;
    printf("Type Animation for: %d | Text id: %d\n", userToPresent, textIndex);

    lcd.Clear();
    lcd.GoTo(0, 0);

    int charCount = 0;
    while (*msg) {
        lcd.WriteData(*msg++);
        charCount++;
        _delay_ms(200); // Delay between characters

        if (charCount >= 16) {
            lcd.GoTo(0, 1); // Move to second line after 16 chars
            charCount = 0; // Reset count for second line
            lcd.WriteData(*msg++);
            charCount++;
            _delay_ms(200);
        }
    }

    _delay_ms(3000); // Wait before clearing
    lcd.Clear();

}