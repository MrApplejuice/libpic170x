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
 * \file main.c
 * \brief Serial communication example demonstrating the use of the [serial library](@ref serial-guide).
 * 
 * Check the [examples page](@ref examples-page) for details.
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

#include <string.h>
#include <stdbool.h>

//! Project is configured for 8 MHz operation
#define _XTAL_FREQ 8000000

// Include all relevant libpic170x libraries
#include <libpic170x/freq.h>
#include <libpic170x/io_control.h>
#include <libpic170x/serial.h>

// Map output port names to functional port names
#define SEND_PIN PIN_RC5
#define RECV_PIN PIN_RC3
#define SEND_LED_PIN PIN_RC1
#define RECV_LED_PIN PIN_RC2

// Variables used by the main function as well as the interrupt handler
Serial serial;
char received_character;

/**
 * \brief Interrupt handler for updating timer0.
 * 
 * Interrupt handler processing the receive queue of the serial interface. The
 * first byte of the receive queue will be copied into the globa variable
 * `received_character` which is picked up by the program's main loop.
 */
void interrupt int_handler() {
    if (RCIF) {
        // Handle serial interface code. This will clear RCIF and TXIF!
        serial_handle_queue(&serial);
        
        // Check if we received some characters - if yes, extract the first
        // received chracter and save in in the global received_character
        // variable.
        if (serial.receive_len > 0) {
            received_character = serial.receive_queue[0];
            serial.receive_len = 0; // Clears the receive buffer
        }
    }
}

/**
 * \brief Main entrypoint.
 * 
 */
int main() {
    OSCCON = OSCCON_BITS;
    
    // Intialize library
    if (!libpic170x_check_library_build_arguments()) {
        while (true) {} 
    }
    
    // Initializes the serial interface at 9600 baud
    serial_init(&serial, RECV_PIN, SEND_PIN, 9600);
    
    // Configure LED pins to act as outputs
    pin_set_pin_mode(SEND_LED_PIN, true);
    pin_set_output(SEND_LED_PIN, false);
    pin_set_pin_mode(RECV_LED_PIN, true);
    pin_set_output(RECV_LED_PIN, false);

    // Clear the char-received buffer
    received_character = '\0';
    
    // Load the test byte into the output buffer. This will be sent when
    // serial_handle_queue() is called the next time
    serial.send_queue[0] = 0xE2; // Test byte
    serial.send_len = 1;         // Only send one byte
    
    // We will use interrupts for handling received characters enable
    RCIE = 1; // ... receive interrupts
    PEIE = 1; // ... peripheral interrupt handlin (for the EUSART module)
    GIE = 1;  // ... global interrupt flag
    // After this the interrupt-handler routine will be called when some 
    // character is received by the EUSART module.
    
    // Manually call serial_handle_queue() once to flush the write buffer.
    serial_handle_queue(&serial);

    // Now wait for received characters
    while (true) {
        // Check if we received something that is != 0
        if (received_character) {
            // When handling the serial buffers that could be modifed by
            // interrupts again, it is good practice to disable interrupts
            // until we handled our buffer changes.
            GIE = 0;
            
            // If we received the correct character, flash the receive-LED
            if (received_character == 0xE2) {
                pin_set_output(RECV_LED_PIN, true);
                __delay_ms(500);
                pin_set_output(RECV_LED_PIN, false);
            }
            
            // Load the received character in the send buffer again.
            serial.send_queue[0] = received_character;
            serial.send_index = 0; // this must be reset - we sent a single byte before so this will be 1.
            serial.send_len = 1;
            
            // Reset the received character.
            received_character = '\0';
            
            GIE = 1; // Serial buffer modifications have been done - we can enable interrupts again.
            
            // Send the next byte, flash the send-LED to indicate that the 
            // next byte is sent.
            pin_set_output(SEND_LED_PIN, true);
            serial_handle_queue(&serial); // Call handle queue to 
            __delay_ms(500);
            pin_set_output(SEND_LED_PIN, false);
        }
    }
}
