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
 * \brief GPIO input/output library allowing to write to and read from IO pins.
 * 
 * The library defines a set of `pin_` functions that operate on pin-defintions.
 * Pin definitions are also defined in this library but depend on the chip
 * setting that was used to compile the library. Pin-definitions are named
 * `PIN_Rxx`, where `xx` stands for the pin name according to the data sheet for
 * the used chip. Example: `PIN_RC2`.
 * 
 * Note that all pins are always defined, however, since the the PIC16(L)F1705
 * has less pins than the PIC16(L)F1709, some pin defintions will be set to 
 * NULL. Here a defintion checklist for the two supported PIC chips:
 * 
 * Pin         |  PIC16(L)F1705  | PIC16(L)F1709
 * ----------- | --------------  | -------------
 * PIN_RA0     | ✓               | ✓
 * PIN_RA1     | ✓               | ✓
 * PIN_RA2     | ✓               | ✓
 * PIN_RA3     | ✓               | ✓
 * PIN_RA4     | ✓               | ✓
 * PIN_RA5     | ✓               | ✓
 * PIN_RB4     | NULL            | ✓
 * PIN_RB5     | NULL            | ✓
 * PIN_RB6     | NULL            | ✓
 * PIN_RB7     | NULL            | ✓
 * PIN_RC0     | ✓               | ✓
 * PIN_RC1     | ✓               | ✓
 * PIN_RC2     | ✓               | ✓
 * PIN_RC3     | ✓               | ✓
 * PIN_RC4     | ✓               | ✓
 * PIN_RC5     | ✓               | ✓
 * PIN_RC6     | NULL            | ✓
 * PIN_RC7     | NULL            | ✓
 * 
 * For a working example on how to use the library, check the "blink" example,
 * in this repository.
 */

#ifndef IO_CONTROL_H
#define	IO_CONTROL_H

#include <stdint.h>
#include <stdbool.h>

//! See pin_set_input_mode())
#define PIN_INPUT_MODE_DIGITAL 0
//! See pin_set_input_mode())
#define PIN_INPUT_MODE_ANALOG 1


/**
 * \brief Record to store a collection of registers and masks related to a pin.
 * 
 * Pin data structure encoding all required data for interacting with
 * a microcontroller's IO pin. Not that any of the pointers can be NULL in case
 * that a certain function of feature is unsupported for the pin!
 */
typedef struct {
    //! The input PPS that can be used for modules to set this chip as input.
    uint8_t pin_pps; 
    //! The output PPS that can set such that the pin act as output for a given module.
    volatile unsigned char *out_src_pps_reg; 
    //! Mask for the pin to use with the referenced tris_reg.
    uint8_t pin_tris_bitmask;
    
    //! Pointer to the pin's tris register (can be NULL)
    volatile unsigned char *tris_reg;
    //! Pointer to the pin's port register (never NULL)
    volatile unsigned char *port_reg;
    //! Pointer to the pin's analog selection register (can be NULL)
    volatile unsigned char *ansel_reg;
    //! Pointer to the pin's output register (can be NULL)
    volatile unsigned char *latch_reg;
} PinDef;


/**
 * Configures a pin as input or output. Writes the corresponding TRISx register.
 * 
 * @param def
 *     Pin to configure.
 * @param output
 *     Set to true to make it an output, false to make it a digital input.
 */
void pin_set_pin_mode(const PinDef* def, bool output);

/**
 * Reads the input value from the the given pin. Returns the value the PORTx
 * register if the pin is configured as input. If the pin is configured as
 * output, the returned value will be the value of LATx, meaning the result
 * will be the value of the *intended* output (see \ref pin_set_pin_mode()).
 * 
 * @param def
 *     The pin to read the input state from.
 * @return 
 *     True if the input is high.
 */
bool pin_get_input(const PinDef* def);

/**
 * Sets the digital output to the given state (LATx register). Will only have
 * an effect if the pin has been set to output-mode before.
 * 
 * @param def
 *     The pin to write.
 * @param on
 *     True to set output high, false to set the output to low.
 */
void pin_set_output(const PinDef* def, bool on);

/**
 * Configures the input mode of a given port.
 * 
 * - PIN_INPUT_MODE_DIGITAL configures a pin as digital input.
 * - PIN_INPUT_MODE_ANALOG configures a pin as analog input (the default after reset).
 * 
 * Note that some chip modules require pins to be in a certain mode.
 * 
 * @param def
 *     The pin to set the input mode for.
 * @param input_mode
 *     Set to PIN_INPUT_MODE_ANALOG or PIN_INPUT_MODE_DIGITAL.
 */
void pin_set_input_mode(const PinDef* def, uint8_t input_mode);


//! PIN_RA0 defintion
extern const PinDef* PIN_RA0;
//! PIN_RA1 defintion
extern const PinDef* PIN_RA1;
//! PIN_RA2 defintion
extern const PinDef* PIN_RA2;
//! PIN_RA3 defintion
extern const PinDef* PIN_RA3;
//! PIN_RA4 defintion
extern const PinDef* PIN_RA4;
//! PIN_RA5 defintion
extern const PinDef* PIN_RA5;

//! PIN_RB4 defintion
extern const PinDef* PIN_RB4;
//! PIN_RB5 defintion
extern const PinDef* PIN_RB5;
//! PIN_RB6 defintion
extern const PinDef* PIN_RB6;
//! PIN_RB7 defintion
extern const PinDef* PIN_RB7;

//! PIN_RC0 defintion
extern const PinDef* PIN_RC0;
//! PIN_RC1 defintion
extern const PinDef* PIN_RC1;
//! PIN_RC2 defintion
extern const PinDef* PIN_RC2;
//! PIN_RC3 defintion
extern const PinDef* PIN_RC3;
//! PIN_RC4 defintion
extern const PinDef* PIN_RC4;
//! PIN_RC5 defintion
extern const PinDef* PIN_RC5;
//! PIN_RC6 defintion
extern const PinDef* PIN_RC6;
//! PIN_RC7 defintion
extern const PinDef* PIN_RC7;

#endif	/* IO_CONTROL_H */

