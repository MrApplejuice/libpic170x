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

// PIC16LF1705 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1
#pragma config FOSC = INTOSC    // Oscillator Selection Bits (INTOSC oscillator: I/O function on CLKIN pin)
#pragma config WDTE = OFF       // Watchdog Timer Enable (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable (PWRT disabled)
#pragma config MCLRE = ON       // MCLR Pin Function Select (MCLR/VPP pin function is MCLR)
#pragma config CP = OFF         // Flash Program Memory Code Protection (Program memory code protection is disabled)
#pragma config BOREN = ON       // Brown-out Reset Enable (Brown-out Reset enabled)
#pragma config CLKOUTEN = OFF   // Clock Out Enable (CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin)
#pragma config IESO = ON        // Internal/External Switchover Mode (Internal/External Switchover Mode is enabled)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enable (Fail-Safe Clock Monitor is enabled)

// CONFIG2
#pragma config WRT = OFF        // Flash Memory Self-Write Protection (Write protection off)
#pragma config PPS1WAY = ON     // Peripheral Pin Select one-way control (The PPSLOCK bit cannot be cleared once it is set by software)
#pragma config ZCDDIS = ON      // Zero-cross detect disable (Zero-cross detect circuit is disabled at POR)
#pragma config PLLEN = ON       // Phase Lock Loop enable (4x PLL is always enabled)
#pragma config STVREN = ON      // Stack Overflow/Underflow Reset Enable (Stack Overflow or Underflow will cause a Reset)
#pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)
#pragma config LPBOR = OFF      // Low-Power Brown Out Reset (Low-Power BOR is disabled)
#pragma config LVP = ON         // Low-Voltage Programming Enable (Low-voltage programming enabled)


#include <stdbool.h>

#include <xc.h>

#define _XTAL_FREQ 8000000

#include <freq.h>
#include <timer0.h>
#include <io_control.h>

/**
 * Interrupt handler for updating timer0.
 */
void interrupt int_handler() {
    timer0_ih(NULL);
}

/**
 * The main implements a "blink" for an LED that is conencted to pin RC0. The
 * connected LED will approximately blink once every second.
 */
int main() {
    // Set OSCCON bits from library definitions
    OSCCON = OSCCON_BITS;
    
    // Intilaize timer0 and enable interrupts
    timer0_init(NULL);
    GIE = 1;

    // Configure RC0 as output
    pin_set_pin_mode(PIN_RC0, true);
    pin_set_output(PIN_RC0, false);
    
    while (1) {
        // Check if 1 scond has expired. Disable intterupts during the 
        // check&modify operation.
        GIE = 0;
        if (pic170x_timer0.ms > 1000) {
            // If 1 second has expired, toggle the output signal on/off.
            pin_set_output(PIN_RC0, !pin_get_input(PIN_RC0));
            pic170x_timer0.ms -= 1000;
        }
        GIE = 1;
    }
}
