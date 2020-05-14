/*	Author: houjiacheng
 *	Lab Section: 022
 *	Assignment: Lab 6  Exercise 2
 *	Exercise Description:
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

// Demo Link: https://drive.google.com/open?id=1qIiP44YXvJ5YkxWRpQDHDtJSsXYlXiea

#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

#include "timer.h"

const unsigned short TASKS_NUM = 2;
const unsigned long TIMER_PERIOD = 20;

unsigned char tmpA, tmpB;
unsigned char direction;

typedef struct task
{
    int state;
    unsigned long period;
    unsigned long elapsedTime;
    int(*TickFunction)(int);
} task;

task tasks[2];

void TimerISR() {
    unsigned char i;
    for (i = 0; i < TASKS_NUM; i++)
    {
        if (tasks[i].elapsedTime >= tasks[i].period)
        {
            tasks[i].state = tasks[i].TickFunction(tasks[i].state);
            tasks[i].elapsedTime = 0;
        }
        tasks[i].elapsedTime += TIMER_PERIOD;
    }
}

enum Tick_States {Start, PB_0, PB_1, PB_2, Press, Pause};



int Tick(int state) {

    // Transitions
    switch (state)
    {
    case Start:
        tmpB = 0x00;
        state = PB_0;
        break;

    case PB_0:
        tmpB = 0x01;
        state = tmpA ? Press : PB_1;
        break;

    case PB_1:
        tmpB = 0x02;
        state = tmpA ? Press : (!direction) ? PB_2 : PB_0;
        break;

    case PB_2:
        tmpB = 0x04;
        state = tmpA ? Press : PB_1;
        break;

    case Press:
        state = tmpA ? Press : Pause;
        break;
    
    case Pause:
        state = tmpA ? PB_0 : Pause;
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
        break;
    
    case Pause:
        break;   
    
    default:
        break;
    } // State actions

    return state;
}

int Output(int state) {
    PORTB = tmpB;
    return 1;
}


int main(void) {
    /* Insert DDR and PORT initializations */
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x00;

    unsigned char i = 0;
    tasks[i].state = PB_0;
    tasks[i].period = 300;
    tasks[i].elapsedTime = 0;
    tasks[i].TickFunction = &Tick;
    
    i++;
    tasks[i].state = 1;
    tasks[i].period = 300;
    tasks[i].elapsedTime = 0;
    tasks[i].TickFunction = &Output;

    TimerSet(TIMER_PERIOD);
    TimerOn();

    /* Insert your solution below */
    tmpB = 0x00;

    while (1) {
        tmpA = ~PINA & 0x01;
    }
    
    return 1;
}
