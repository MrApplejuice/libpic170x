#include "libpic170x/serial.h"

#include <string.h>

#include <xc.h>

#include "libpic170x/freq.h"

void serial_init(Serial* uart, const PinDef* rc_pin, const PinDef* tx_pin, uint32_t baud_rate) {
    uart->__rc_pin = rc_pin;
    uart->__tx_pin = tx_pin;
    
    SYNC = 0;
    {
        uint32_t rate;
        rate = _XTAL_FREQ / baud_rate / 64 - 1;
        if ((rate < 10) || (rate >= 255)) {
            // switch to a lower clock speed for better resolution
            BRG16 = 1;
            rate = _XTAL_FREQ / baud_rate / 16 - 1;
            
            if (rate < 20) {
                // Switch to an even lower clock speed for better resolution
                BRGH = 1;
                rate = _XTAL_FREQ / baud_rate / 4 - 1;
            }
        }
        
        SP1BRGH = (rate >> 8) & 0xFF;
        SP1BRGL = rate & 0xFF;
    }
    
    if (rc_pin != NULL) {
        memset(uart->receive_queue, 0, SERIAL_QUEUE_LEN);
        uart->receive_len = 0;
        
        pin_set_is_output(rc_pin, false);
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
            pin_set_is_output(tx_pin, true);
            *(tx_pin->out_src_pps_reg) = 0b00010100;

            // Enable sender
            TXEN = 1;
        }
    }
    
    uart->had_error = false;
    SPEN = 1;
}

void serial_handle_queue(Serial* uart) {
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
}

