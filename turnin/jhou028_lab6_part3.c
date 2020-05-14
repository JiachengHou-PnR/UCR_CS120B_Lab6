/*	Author: houjiacheng
 *	Lab Section: 022
 *	Assignment: Lab 6  Exercise 3
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

const unsigned short TASKS_NUM = 5;

const unsigned long TIMER_PERIOD    = 100;
const unsigned long IO_PERIOD       = 100;
const unsigned long CNTU_PERIOD     = 200;
const unsigned long TICK_PERIOD     = 100;

typedef struct task
{
    int state;
    unsigned long period;
    unsigned long elapsedTime;
    int(*TickFunction)(int);
} task;

task tasks[5];

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

unsigned char tmpA0, tmpA1;
unsigned char dispNum;

enum Tick_States {Tick_Start, init, wait, increment, decrement, reset};
int Tick(int state) {

    switch (state)      // Transitions
    {
    case Tick_Start:
        state = init;
        break;

    case init:
        dispNum = 0;
        state = wait;
        break;

    case wait:
        if (tmpA0 && tmpA1)
        {
            state = reset;
        }
        else if (!tmpA0 && !tmpA1)
        {
            state = wait;
        }
        else if (tmpA0 && !tmpA1)
        {
            if (dispNum < 9) dispNum++;
            state = increment;
        }
        else // (!tmpA0 && tmpA1)
        {
            if (dispNum > 0) dispNum--;
            state = decrement;
        }
        break;        

    case increment:
        if (tmpA0 && tmpA1)
        {
            state = reset;
        }
        else if (tmpA0 && !tmpA1)
        {
            state = increment;
        }
        else // (!tmpA0)
        {
            state = wait;
        }
        break;
        
    case decrement:
        if (tmpA0 && tmpA1)
        {
            state = reset;
        }
        else if (!tmpA0 && tmpA1)
        {
            state = decrement;
        }
        else // (!tmpA1)
        {
            state = wait;
        }
        break;

    case reset:
        if (!tmpA0 && !tmpA1)
        {
            state = wait;
        }
        else // !(!tmpA0 && !tmpA1)
        {
            state = reset;
        }
        break;
    
    default:
        state = init;
        break;
    }

    switch (state)      // State Actions
    {
    case Tick_Start:     break;
    
    case init:      break;

    case wait:      break;

    case increment: break;
    
    case decrement: break;

    case reset:
        dispNum = 0;
        break;

    default:        break;
    }

    return state;
}

enum Incr_States {Inc_Wait, A0_press, Cntu_Inc};
unsigned char inc_cnt;

int CntuIncrement(int state) {
    switch (state) // Transition
    {
    case Inc_Wait:
        state = tmpA0 ? A0_press : Inc_Wait;
        break;

    case A0_press:
        if (!tmpA0 || tmpA1)
        {   
            state = Inc_Wait;
        }
        else if (inc_cnt < 5)
        {
            inc_cnt ++;
            state = A0_press;
        }
        else
        {
            if (dispNum < 9) dispNum++;
            state = Cntu_Inc;
        }

        break;

    case Cntu_Inc:
        state = Inc_Wait;
        break;
    
    default:
        break;
    } // Transition

    switch (state) // State Action
    {
    case Inc_Wait:
        inc_cnt = 0;
        break;

    case A0_press:
        break;

    case Cntu_Inc:
        break;
    
    default:
        break;
    } // State Action
    
    return state;
}

enum Decr_States {Dec_Wait, A1_press, Cntu_Dec};
unsigned char dec_cnt;

int CntuDecrement(int state) {
    switch (state) // Transition
    {
    case Dec_Wait:
        state = tmpA1 ? A1_press : Dec_Wait;
        break;

    case A1_press:
        if (!tmpA1  || tmpA0)
        {   
            state = Dec_Wait;
        }
        else if (dec_cnt < 5)
        {
            dec_cnt ++;
            state = A1_press;
        }
        else
        {
            if (dispNum > 0) dispNum--;
            state = Cntu_Dec;
        }

        break;

    case Cntu_Dec:
        state = Dec_Wait;
        break;
    
    default:
        break;
    } // Transition

    switch (state) // State Action
    {
    case Dec_Wait:
        dec_cnt = 0;
        break;

    case A1_press:
        break;

    case Cntu_Dec:
        break;
    
    default:
        break;
    } // State Action
    
    return state;
}

int Input(int state) {
    tmpA0 =  ~PINA & 0x01;
    tmpA1 = (~PINA >> 1) & 0x01;
    return 1;
}

int Output(int state) {
    PORTB = dispNum;
    return 1;
}

int main(void) {
    /* Insert DDR and PORT initializations */
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x00;

    tmpA0 =  ~PINA & 0x01;
    tmpA1 = (~PINA >> 1) & 0x01;

    unsigned char i = 0;
    tasks[i].state = Tick_Start;
    tasks[i].period = TICK_PERIOD;
    tasks[i].elapsedTime = 0;
    tasks[i].TickFunction = &Tick;

    i++;
    tasks[i].state = Inc_Wait;
    tasks[i].period = CNTU_PERIOD;
    tasks[i].elapsedTime = 0;
    tasks[i].TickFunction = &CntuIncrement;

    i++;
    tasks[i].state = Dec_Wait;
    tasks[i].period = CNTU_PERIOD;
    tasks[i].elapsedTime = 0;
    tasks[i].TickFunction = &CntuDecrement;

    i++;
    tasks[i].state = 1;
    tasks[i].period = IO_PERIOD;
    tasks[i].elapsedTime = 0;
    tasks[i].TickFunction = &Input;

    i++;
    tasks[i].state = 1;
    tasks[i].period = IO_PERIOD;
    tasks[i].elapsedTime = 0;
    tasks[i].TickFunction = &Output;
    
    TimerSet(TIMER_PERIOD);
    TimerOn();

    /* Insert your solution below */
    dispNum = 0x00;

    while (1) {}
    
    return 1;
}
