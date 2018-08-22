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
 * \file io_control.h
 * \author Paul Konstantin Gerke
 * \date 2018-07-22
 * \brief GPIO input/output library allowing to write and read from IO pins.
 * 
 * 
 */

#ifndef IO_CONTROL_H
#define	IO_CONTROL_H

#include <stdint.h>
#include <stdbool.h>


#define PIN_INPUT_MODE_DIGITAL 0
#define PIN_INPUT_MODE_ANALOG 1


typedef struct {
    uint8_t pin_pps;
    volatile unsigned char *out_src_pps_reg;
    uint8_t pin_tris_bitmask;
    
    volatile unsigned char *tris_reg;
    volatile unsigned char *port_reg;
    volatile unsigned char *ansel_reg;
    volatile unsigned char *latch_reg;
} PinDef;


/**
 * Configures a pin as input or output. Writes the corresponding TRISx register.
 * 
 * @param def
 *     Pin definition.
 * @param output
 *     Set to true to make it an output, false to make it a digital input.
 */
void pin_set_pin_mode(const PinDef* def, bool output);

/**
 * Reads the input value from the the given pin. Returns the value the PORTx
 * register.
 * 
 * @param def
 *     The pin to read the input
 * @return 
 *     Non-zero value if input is high.
 */
uint8_t pin_get_state(const PinDef* def);

/**
 * Sets the digital output to the given state (LATx register)
 * 
 * @param def
 *     The pin to write.
 * @param on
 *     True to turn output on, false to turn output off
 */
void pin_set_output(const PinDef* def, bool on);

/**
 * Configures the input port as a digital or analog input.
 * 
 * @param def
 * @param input_mode
 *   Set to PIN_INPUT_MODE_ANALOG to make the pin an analog input, 
 *   set to PIN_INPUT_MODE_DIGITAL to make the pin a digital input.
 */
void pin_set_input_mode(const PinDef* def, uint8_t input_mode);


extern const PinDef* PIN_RA0;
extern const PinDef* PIN_RA1;
extern const PinDef* PIN_RA2;
extern const PinDef* PIN_RA3;
extern const PinDef* PIN_RA4;
extern const PinDef* PIN_RA5;

extern const PinDef* PIN_RB4;
extern const PinDef* PIN_RB5;
extern const PinDef* PIN_RB6;
extern const PinDef* PIN_RB7;

extern const PinDef* PIN_RC0;
extern const PinDef* PIN_RC1;
extern const PinDef* PIN_RC2;
extern const PinDef* PIN_RC3;
extern const PinDef* PIN_RC4;
extern const PinDef* PIN_RC5;
extern const PinDef* PIN_RC6;
extern const PinDef* PIN_RC7;

#endif	/* IO_CONTROL_H */

