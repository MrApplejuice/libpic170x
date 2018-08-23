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

#include "io_control.h"

#include <string.h>

#include <xc.h>

#if defined(PIC16F1705) || defined(PIC16LF1705)
const PinDef c_PIN_RA0 = {0b00000000, &RA0PPS, 0b00000001, &TRISA, &PORTA, &ANSELA, &LATA};
const PinDef c_PIN_RA1 = {0b00000001, &RA1PPS, 0b00000010, &TRISA, &PORTA, &ANSELA, &LATA};
const PinDef c_PIN_RA2 = {0b00000010, &RA2PPS, 0b00000100, &TRISA, &PORTA, &ANSELA, &LATA};
const PinDef c_PIN_RA3 = {0b00000011, NULL,    0b00001000, NULL,   &PORTA, NULL,    NULL};
const PinDef c_PIN_RA4 = {0b00000100, &RA4PPS, 0b00010000, &TRISA, &PORTA, &ANSELA, &LATA};
const PinDef c_PIN_RA5 = {0b00000101, &RA5PPS, 0b00100000, &TRISA, &PORTA, NULL,    &LATA};

const PinDef c_PIN_RC0 = {0b00010000, &RC0PPS, 0b00000001, &TRISC, &PORTC, &ANSELC, &LATC};
const PinDef c_PIN_RC1 = {0b00010001, &RC1PPS, 0b00000010, &TRISC, &PORTC, &ANSELC, &LATC};
const PinDef c_PIN_RC2 = {0b00010010, &RC2PPS, 0b00000100, &TRISC, &PORTC, &ANSELC, &LATC};
const PinDef c_PIN_RC3 = {0b00010011, &RC3PPS, 0b00001000, &TRISC, &PORTC, &ANSELC, &LATC};
const PinDef c_PIN_RC4 = {0b00010100, &RC4PPS, 0b00010000, &TRISC, &PORTC, NULL,    &LATC};
const PinDef c_PIN_RC5 = {0b00010101, &RC5PPS, 0b00100000, &TRISC, &PORTC, NULL,    &LATC};


const PinDef* PIN_RA0 = &c_PIN_RA0;
const PinDef* PIN_RA1 = &c_PIN_RA1;
const PinDef* PIN_RA2 = &c_PIN_RA2;
const PinDef* PIN_RA3 = &c_PIN_RA3;
const PinDef* PIN_RA4 = &c_PIN_RA4;
const PinDef* PIN_RA5 = &c_PIN_RA5;

const PinDef* PIN_RB4 = NULL;
const PinDef* PIN_RB5 = NULL;
const PinDef* PIN_RB6 = NULL;
const PinDef* PIN_RB7 = NULL;

const PinDef* PIN_RC0 = &c_PIN_RC0;
const PinDef* PIN_RC1 = &c_PIN_RC1;
const PinDef* PIN_RC2 = &c_PIN_RC2;
const PinDef* PIN_RC3 = &c_PIN_RC3;
const PinDef* PIN_RC4 = &c_PIN_RC4;
const PinDef* PIN_RC5 = &c_PIN_RC5;
const PinDef* PIN_RC6 = NULL;
const PinDef* PIN_RC7 = NULL;
#elif defined(PIC16F1709) || defined(PIC16LF1709)
const PinDef c_PIN_RA0 = {0b00000000, &RA0PPS, 0b00000001, &TRISA, &PORTA, &ANSELA, &LATA};
const PinDef c_PIN_RA1 = {0b00000001, &RA1PPS, 0b00000010, &TRISA, &PORTA, &ANSELA, &LATA};
const PinDef c_PIN_RA2 = {0b00000010, &RA2PPS, 0b00000100, &TRISA, &PORTA, &ANSELA, &LATA};
const PinDef c_PIN_RA3 = {0b00000011, NULL,    0b00001000, NULL,   &PORTA, NULL,    NULL};
const PinDef c_PIN_RA4 = {0b00000100, &RA4PPS, 0b00010000, &TRISA, &PORTA, &ANSELA, &LATA};
const PinDef c_PIN_RA5 = {0b00000101, &RA5PPS, 0b00100000, &TRISA, &PORTA, NULL,    &LATA};

const PinDef c_PIN_RB4 = {0b00001100, &RB4PPS, 0b00010000, &TRISB, &PORTB, &ANSELB, &LATB};
const PinDef c_PIN_RB5 = {0b00001101, &RB5PPS, 0b00100000, &TRISB, &PORTB, &ANSELB, &LATB};
const PinDef c_PIN_RB6 = {0b00001110, &RB6PPS, 0b01000000, &TRISB, &PORTB, NULL,    &LATB};
const PinDef c_PIN_RB7 = {0b00001111, &RB7PPS, 0b10000000, &TRISB, &PORTB, NULL,    &LATB};

const PinDef c_PIN_RC0 = {0b00010000, &RC0PPS, 0b00000001, &TRISC, &PORTC, &ANSELC, &LATC};
const PinDef c_PIN_RC1 = {0b00010001, &RC1PPS, 0b00000010, &TRISC, &PORTC, &ANSELC, &LATC};
const PinDef c_PIN_RC2 = {0b00010010, &RC2PPS, 0b00000100, &TRISC, &PORTC, &ANSELC, &LATC};
const PinDef c_PIN_RC3 = {0b00010011, &RC3PPS, 0b00001000, &TRISC, &PORTC, &ANSELC, &LATC};
const PinDef c_PIN_RC4 = {0b00010100, &RC4PPS, 0b00010000, &TRISC, &PORTC, NULL,    &LATC};
const PinDef c_PIN_RC5 = {0b00010101, &RC5PPS, 0b00100000, &TRISC, &PORTC, NULL,    &LATC};
const PinDef c_PIN_RC6 = {0b00010110, &RC6PPS, 0b01000000, &TRISC, &PORTC, &ANSELC, &LATC};
const PinDef c_PIN_RC7 = {0b00010111, &RC7PPS, 0b10000000, &TRISC, &PORTC, &ANSELC, &LATC};


const PinDef* PIN_RA0 = &c_PIN_RA0;
const PinDef* PIN_RA1 = &c_PIN_RA1;
const PinDef* PIN_RA2 = &c_PIN_RA2;
const PinDef* PIN_RA3 = &c_PIN_RA3;
const PinDef* PIN_RA4 = &c_PIN_RA4;
const PinDef* PIN_RA5 = &c_PIN_RA5;

const PinDef* PIN_RB4 = &c_PIN_RB4;
const PinDef* PIN_RB5 = &c_PIN_RB5;
const PinDef* PIN_RB6 = &c_PIN_RB6;
const PinDef* PIN_RB7 = &c_PIN_RB7;

const PinDef* PIN_RC0 = &c_PIN_RC0;
const PinDef* PIN_RC1 = &c_PIN_RC1;
const PinDef* PIN_RC2 = &c_PIN_RC2;
const PinDef* PIN_RC3 = &c_PIN_RC3;
const PinDef* PIN_RC4 = &c_PIN_RC4;
const PinDef* PIN_RC5 = &c_PIN_RC5;
const PinDef* PIN_RC6 = &c_PIN_RC6;
const PinDef* PIN_RC7 = &c_PIN_RC7;
#else
#error "No valid PIC chip defined"
#endif

void pin_set_pin_mode(const PinDef* def, bool output)  {
    if (!def || !def->tris_reg) {
        return;
    }
    
    if (output) { 
        *(def->tris_reg) = (uint8_t) (*(def->tris_reg) & (~def->pin_tris_bitmask)); 
    } else { 
        *(def->tris_reg) = (uint8_t) (*(def->tris_reg) | def->pin_tris_bitmask); 
    }
}

bool pin_get_input(const PinDef* def) {
    if (!def || (!def->port_reg)) {
        return 0;
    }
    volatile unsigned char* state_reg = 
        (def->tris_reg && ((*def->tris_reg & def->pin_tris_bitmask) == 0)) 
            ? def->latch_reg : def->port_reg;
    
    return (*state_reg & def->pin_tris_bitmask) != 0;
}

void pin_set_output(const PinDef* def, bool on) {
    if (!def || !def->latch_reg) {
        return;
    }
    
    if (on) {
        *(def->latch_reg) = (uint8_t) (*(def->latch_reg) | def->pin_tris_bitmask);
    } else {
        *(def->latch_reg) = (uint8_t) (*(def->latch_reg) & (~def->pin_tris_bitmask));
    }
}

void pin_set_input_mode(const PinDef* def, uint8_t input_mode) {
    if (!def || !def->ansel_reg) {
        return;
    }
    
    if (input_mode == PIN_INPUT_MODE_DIGITAL) {
        *(def->ansel_reg) = (uint8_t) (*(def->ansel_reg) & (~def->pin_tris_bitmask));
    } else {
        *(def->ansel_reg) = (uint8_t) (*(def->ansel_reg) | def->pin_tris_bitmask);
    }
}

