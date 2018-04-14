#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#ifndef F_CPU
#define F_CPU 1000000
#endif

int main() {

    //long PORTB = 0x00010000;


    DDRB |= (1<<PB4) | (1<<PB5) |(1 << PB6);


    DDRE |= (1<< PE3);





    while (1){

        PORTB &= ~((1<<PB4) | (1<<PB5) | (1<<PB6));

        PORTE &= ~(1<<PE3);
        _delay_ms(500);

        PORTB |= (1<<PB4)|(1<<PB5)|(1<<PB6);
        PORTE |= (1<<PE3);
        _delay_ms(500);

    }





    return 0;
}