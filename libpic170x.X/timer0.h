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

/**
 * \file timer0.h
 * \author Paul Konstantin Gerke
 * \date 2018-07-20
 * \brief Coarse timing library for measuring times in the background.
 * 
 * Timer0 configures timer0 of a PIC16(L)F170x chip as continuous ms 
 * counter. The values of the continuous counter are stored in the global
 * structure pic170x_timer0.
 * 
 * The counter requires the function timer0_ih to be called in the PIC's 
 * interrupt handler. The timer0_ih function will take care of updating the
 * counter values according to the used _XTAL_FREQ settings. Please note
 * that this library assumes that the configured frequency of the PIC does
 * not change over time and remains constant while the program runs.
 * 
 * The timer is configured with a 256 prescaler, which leads to very
 * high increments of the counter values (for a 8 MHz-configured chip it will
 * be around 32-33 ms). This makes the timer values suitable for rough timing
 * keeping only.
 * 
 */

#ifndef TIMER0_H
#define	TIMER0_H

#include <stdint.h>

/**
 * \struct Timer0;
 * Provides access to the timer counter values. Values are writeable if a reset
 * of the counter is desired.
 */
typedef struct {
    /** ms portion of the counter. This will wrap to zero when the uint32_t
     * is exhausted */
    uint32_t ms;
    /**
     * Microsecond porition of the counter. Will always be <= 1000.
     */
    uint16_t us;
} Timer0;

/**
 * Internal default pic170x_timer0 structure that will be used by the functions
 * timer0_init() and timer0_ih() if no alternative is specified.
 */
extern Timer0 pic170x_timer0;

/**
 * Initializes timer0 and the given timer0 structure. This function 
 * reconfigures the PIC registers:
 * 
 * - TMR0CS
 * - OPTION_REG
 * - TMR0IE
 * 
 * 
 * @param timer0
 *     
 * The timer0 structure that should be initialized. If NULL, it will default
 * to use the pic170x_timer0.
 */
void timer0_init(Timer0 *timer0);

/**
 * Interrupt handler. Increment the counter of the given structure as a reaction
 * to a timer0 event. 
 * 
 * The function resets TMR0IF.
 * 
 * 
 * @param timer0
 * 
 * The structure for which to increment the timer value. If NULL this 
 * defaults to pic170x_timer0.
 */
void timer0_ih(Timer0* timer0);

#endif	/* TIMER0_H */

