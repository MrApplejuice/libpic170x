/*
   Copyright 2018 Paul Konstantin Gerke

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
 */

#include "libpic170x/timer0.h"
#include "libpic170x/freq.h"

#include <xc.h>

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
