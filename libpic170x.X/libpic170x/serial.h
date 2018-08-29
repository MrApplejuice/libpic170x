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
 * The serial library provides access to the PIC's UART and configures it for
 * serial communication. The libpic170x documentation contains a 
 * [guide on how to use the serial-library](@ref serial-guide).
 */

#ifndef SERIAL_H
#define	SERIAL_H

#include <stdint.h>
#include <stdbool.h>

#include "io_control.h"

//! Send and receive queue lengths for internal buffers.
#define SERIAL_QUEUE_LEN 32

/**
 * \brief Struct containing writable and readable buffers for reading and writing data to the the UART module.
 * 
 * This structure contains receive and send buffers that are processed by the
 * function serial_handle_queue(Serial* serial). Applications fill the 
 * send_queue field with data that should be sent, and can receive data by 
 * reading from the receive_queue.
 * 
 * If an error occurs, had_error is set to true.
 */
typedef struct {
    //! Receive queue for data receveived from the serial port.
    uint8_t receive_queue[SERIAL_QUEUE_LEN];
    //! Marks the number of valid read bytes in the receive_queue. Set to 0 to clear the receive_queue.
    uint8_t receive_len;
    
    //! Send queue that data can be written into that then are sent via the Serial transmission line.
    uint8_t send_queue[SERIAL_QUEUE_LEN];
    //! Number of valid bytes in the send_queue. Must be written by the main application.
    uint8_t send_len;
    //! Last transmitted byte. Updated by serial_handle_queue(Serial* serial). Set to 0 to reset.
    uint8_t send_index;
    
    //! Updated by serial_handle_queue(Serial* serial). Is set to True if an error occurs. Set to false to clear errors state.
    bool had_error;

    // private section
    //! Private reference to the receive pin **Do not touch!**
    const PinDef* __rc_pin;
    //! Private reference to the transmission pin **Do not touch!**
    const PinDef* __tx_pin;
} Serial;

/**
 * \brief Initialize the UART module for Serial communication.
 * 
 * The functions configures the selected pins as 
 * receive and/or transmission pins. The baud_rate specifies the target 
 * communication speed. The correct prescale values and counters for the 
 * communication frequency generator are dervied by comparing the target 
 * baud_rate with _XTAL_FREQ.
 * 
 * @param serial
 *     Pointer to the serial structure that should be initialized. Must be 
 *     defined by the main using application.
 * @param rc_pin
 *     Reference to the receive pin. This pin will be reconfigured as digital
 *     and assigned to the PIC's UART module. Can be NULL, in which case the
 *     UART module will not be capable of receiving data.
 * @param tx_pin
 *     Reference to the transmission pin. This pin will be reconfigured as
 *     digital output pin. If it is set to 
 * @param baud_rate
 *     The target baud-rate to configure the UART module for communication. The
 *     function will compute suitable prescale values based on _XTAL_FREQ. The
 *     actual achieved baud-rate will be returned by the function call. In case
 *     the baud-rate is too high, the function will fail and return zero!
 * @return
 *     The actual baud-rate that was configured. This can be used to check if
 *     the achieved tolerance is acceptable. If the returned value is zero, the
 *     intialization failed, probably because the baud-rate was too high. In
 *     this case, the configuration of pins will not be changed
 */
uint32_t serial_init(
        Serial* serial, 
        const PinDef* rc_pin,
        const PinDef* tx_pin,
        uint32_t baud_rate);

/**
 * \brief Receive and send queue handler for the UART module.
 * 
 * This function handles the send/receive queue allowing data to be sent or
 * received. It must be called repeatedly, possibly in the global interrupt 
 * callback to handle traffic.
 * 
 * It will consume data from the Serial structure's send_queue, and write it 
 * to the UART module, and write newly received characters into the 
 * receive_queue (depending on the initialization of receive and transmission 
 * pins). Therefore, while this function is running, the Serial-
 * strucutre may not change, for example, due to an interrupt handler.
 * 
 * Should an error occur, the function will set the had_error-flag of the Serial
 * structure. It is the caller's responsibility to clear this flag again. The
 * function will issue the required steps for clearing the error state from the
 * PIC's UART module, so subsequent calls to serial_handle_queue will succeed
 * again. There is no need to re-initialize the module if an error occurs.
 * 
 * @param serial
 *     The Serial structure used for the serial module containing all relevant
 *     buffers.
 */
void serial_handle_queue(Serial* serial);


#endif	/* UART_H */

