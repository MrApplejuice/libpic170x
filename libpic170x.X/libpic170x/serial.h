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
 * \file serial.h
 * \brief Serial interface library allowing the use of the PIC-UART module.
 * 
 * 
 */

#ifndef UART_H
#define	UART_H

#include <stdint.h>
#include <stdbool.h>

#include "io_control.h"

//! Send and receive queue lengths for internal buffers.
#define SERIAL_QUEUE_LEN 32

typedef struct {
    uint8_t receive_queue[SERIAL_QUEUE_LEN];
    uint8_t receive_len;
    
    uint8_t send_queue[SERIAL_QUEUE_LEN];
    uint8_t send_len, send_index;
    
    bool had_error;

    // private section
    const PinDef* __rc_pin;
    const PinDef* __tx_pin;
} Serial;

/**
 * Initialize the uart configuring the given pins as receive and/or transmission
 * pins. The baud_rate specifies the target communication speed. The correct
 * prescale values and counters for the communication frequency generator are
 * dervied by comparing the target baud_rate with _XTAL_FREQ.
 * 
 * @param serial
 * @param rc_pin
 * @param tx_pin
 * @param baud_rate
 */
void serial_init(Serial* serial, const PinDef* rc_pin, const PinDef* tx_pin, uint32_t baud_rate);

/**
 * This function handles the send/receive queue allowing data to be sent or
 * received. It must be called regularly, possibly in the global interrupt 
 * callback to handle traffic.
 * 
 * @param serial
 */
void serial_handle_queue(Serial* serial);


#endif	/* UART_H */

