/*	Author: houjiacheng
 *	Lab Section: 022
 *	Assignment: Lab 6  Exercise 1
 *	Exercise Description:
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

// Demo Link: https://drive.google.com/open?id=1Lql_Qij9TBVil9QJeKv-wHbkkTFLsV8q

#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

#include "timer.h"

enum States {Start, PB_0, PB_1, PB_2} state;

unsigned char tmpB;

void Tick() {

    // Transitions
    switch (state)
    {
    case Start:
        tmpB = 0;
        state = PB_0;
        break;

    case PB_0:
        tmpB = 0x01;
        state = PB_1;
        break;

    case PB_1:
        tmpB = 0x02;
        state = PB_2;
        break;

    case PB_2:
        tmpB = 0x04;
        state = PB_0;
        break;
    
    default:
        state = PB_0;
        break;
    } // Transitions


    // State actions
    switch (state) 
    {
    case Start:
        
        break;

    case PB_0:

        break;

    case PB_1:
        
        break;

    case PB_2:
        
        break;
    
    default:
        break;
    } // State actions

}



int main(void) {
    /* Insert DDR and PORT initializations */
    // DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x00;

    TimerSet(1000);
    TimerOn();

    /* Insert your solution below */

    tmpB = 0x00;
    state = Start;

    while (1) {
        Tick();
        PORTB = tmpB;

        while (!TimerFlag); // Wait 1000 ms


        TimerFlag = 0;
    }
    
    return 1;
}
