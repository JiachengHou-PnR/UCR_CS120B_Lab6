/*	Author: houjiacheng
 *	Lab Section: 022
 *	Assignment: Lab 6  Exercise 2
 *	Exercise Description:
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

// Demo Link: https://drive.google.com/open?id=1OcnblHwFaqFoBeraOiyLv-z1FCWQWRyl

#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

#include "timer.h"

enum States {Start, PB_0, PB_1, PB_2, Press, Pause} state;

unsigned char tmpA, tmpB;
unsigned char direction, pressed;

void Tick() {

    // Transitions
    switch (state)
    {
    case Start:
        tmpB = 0x00;
        pressed = 0x00;
        state = PB_0;
        break;

    case PB_0:
        tmpB = 0x01;
        if (pressed)
        {
            state = Press;
            pressed = 0;
        }
        else
        {
            state = PB_1;
        }
        
        break;

    case PB_1:
        tmpB = 0x02;
        if (pressed)
        {
            state = Press;
            pressed = 0;
        }
        else
        {
            state = (!direction) ? PB_2 : PB_0;
        }
        
        break;

    case PB_2:
        tmpB = 0x04;
        if (pressed)
        {
            state = Press;
            pressed = 0;
        }
        else
        {
            state = PB_1;
        }
        
        break;

    case Press:
        //pressed = 0x00;
        if (pressed)
        {
            state = Press;
            pressed = 0;
        }
        else
        {
            state = Pause;
        }
        
        break;
    
    case Pause:
        //pressed = 0x00;
        if (pressed)
        {
            state = PB_0;
            pressed = 0;
        }
        else
        {
            state = Pause;
        }
        
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
        direction = 0;
        break;

    case PB_1:
        break;

    case PB_2:
        direction = 1;
        break;

    case Press:
        //pressed = 0;
        break;
    
    case Pause:
        //pressed = 0;
        break;   
    
    default:
        break;
    } // State actions

}

void Button() {
    pressed = tmpA ? 1 : pressed;
}

int main(void) {
    /* Insert DDR and PORT initializations */
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x00;

    unsigned long tick_elapsedTime = 0;
    const unsigned long TIMER_PERIOD = 3;

    TimerSet(TIMER_PERIOD);
    TimerOn();

    /* Insert your solution below */
    tmpB = 0x00;
    state = Start;

    while (1) {
        tmpA = ~PINA & 0x01;
        
        Button();

        if (tick_elapsedTime >= 300)
        {
            PORTB = tmpB | (pressed << 5);
            Tick();

            tick_elapsedTime = 0;
        }

        while (!TimerFlag); // Wait TIMER_PERIOD ms
        TimerFlag = 0;

         tick_elapsedTime += TIMER_PERIOD;
    }
    
    return 1;
}
