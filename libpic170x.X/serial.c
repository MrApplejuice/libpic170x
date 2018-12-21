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

#include "libpic170x/serial.h"

#include <string.h>

#include <xc.h>

#include "libpic170x/freq.h"

uint32_t serial_init(
        Serial* uart,
        const PinDef* rc_pin,
        const PinDef* tx_pin,
        uint32_t baud_rate) {
    uart->__rc_pin = rc_pin;
    uart->__tx_pin = tx_pin;
    
    SYNC = 0;
    {
        uint32_t rate;
        uint8_t used_scale_factor;
        
        used_scale_factor = 64;
        rate = _XTAL_FREQ / baud_rate / used_scale_factor - 1;
        if ((rate < 10) || (rate >= 255)) {
            // switch to a faster clock speed for better resolution
            BRG16 = 1;
            used_scale_factor = 16;
            rate = _XTAL_FREQ / baud_rate / used_scale_factor - 1;
        
            if (rate < 256) {
                // Switch to an even faster clock speed for better resolution
                BRGH = 1;
                used_scale_factor = 4;
                rate = _XTAL_FREQ / baud_rate / used_scale_factor - 1;
                
                if (rate <= 1) {
                    return 0; // way too fast!
                }
            }
        }

        SP1BRGH = (rate >> 8) & 0xFF;
        SP1BRGL = rate & 0xFF;
        baud_rate = _XTAL_FREQ / ((rate + 1) * used_scale_factor);
    }
    
    if (rc_pin != NULL) {
        memset(uart->receive_queue, 0, SERIAL_QUEUE_LEN);
        uart->receive_len = 0;
        
        pin_set_output(rc_pin, false);
        pin_set_input_mode(rc_pin, PIN_INPUT_MODE_DIGITAL);
        
        RXPPS = rc_pin->pin_pps;
        
        // Enable receiver mode
        CREN = 1;
    }
    if (tx_pin != NULL) {
        memset(uart->send_queue, 0, SERIAL_QUEUE_LEN);
        uart->send_index = 0;
        uart->send_len = 0;
        
        if (tx_pin->out_src_pps_reg == NULL) {
            uart->__tx_pin = NULL;
        } else {
            pin_set_output(tx_pin, true);
            *(tx_pin->out_src_pps_reg) = 0b00010100;

            // Enable sender
            TXEN = 1;
        }
    }
    
    uart->had_error = false;
    SPEN = 1;
    
    return baud_rate;
}

void serial_handle_queue(Serial* uart) {
    char oldGIE = GIE;
    GIE = 0;
    if (uart->__rc_pin) {
        if (OERR) {
            // Reset the rc_pin, discard input since it is probably corrupted
            // anyway
            CREN = 0;
            CREN = 1;
            uart->had_error = true;
        } else {
            while (RCIF) {
                if (FERR) {
                    // Reset FERR
                    unsigned char _ = RCREG; // Clear FERR, hopefully
                    uart->had_error = true;
                } else {
                    uint8_t data = RCREG;
                    if (uart->receive_len < SERIAL_QUEUE_LEN) {
                        uart->receive_queue[uart->receive_len++] = data;
                    } else {
                        uart->had_error = true;
                    }
                }
            }
        }
    }
    if (uart->__tx_pin) {
        if (uart->send_index < uart->send_len) {
            if (TXIF) {
                TXREG = (uint8_t) uart->send_queue[uart->send_index++];
            }
        }
    }
    GIE = oldGIE;
}

