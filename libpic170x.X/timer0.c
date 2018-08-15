#include "timer0.h"

#include <xc.h>

#include "freq.h"

Timer0 pic170x_timer0;

void timer0_init(Timer0 *timer0) {
    if (!timer0) timer0 = &pic170x_timer0;
    
    // Enable timer mode
    TMR0CS = 0;
    
    // Enable prescaler 1:256
    OPTION_REG = (unsigned char) (OPTION_REG & 0b11010000) | TIMER0_PRESCALE_BITS;
    
    // Enable interrupts
    TMR0IE = 1;
    
    timer0->ms = 0;
    timer0->us = 0;
}

void timer0_ih(Timer0* timer0) {
    if (TMR0IF) {
        if (!timer0) timer0 = &pic170x_timer0;
        
#if TIMER0_MS_INC > 0
        timer0->ms += TIMER0_MS_INC;
#endif
        if ((timer0->us += TIMER0_US_INC) > 1000) {
            timer0->ms += timer0->us / 1000;
            timer0->us %= 1000;
        }
        TMR0IF = 0;
    }
}
