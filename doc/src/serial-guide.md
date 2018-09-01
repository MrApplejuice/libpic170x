Serial library                   {#serial-guide}
==============

[TOC]

The serial library [serial.h](@ref serial.h) provides access to the PIC's UART module and allows it to be used for simple serial communication. Specifically, currently only simple RS232-based 8-bit protocols are supported (e.g. no parity bits, etc). The maximum transmission and receive voltages are given by the used chip types, which means 3.3V for the LF-pic variants and 5V for the F-variants.

Do not connect your chip to to 12V RS232 directly, it will fry your Microcontroller!


# Initialization

To initialize the serial communication module, you call the serial_init() function to initialize a fresh Serial structure:

~~~~~~~~~~~~~~~~~{.c}
Serial serial;
int real_baudrate = serial_init(&serial, PIN_RA4, PIN_RA5, 9600);

// Check if the baud rate lies withing acceptable paramters
if ((read_baudrate < 9400) || (read_baudrate > 9800)) {
    while (true) {} // Do not continue, fail-state
}
~~~~~~~~~~~~~~~~~

In the example, serial_init() will setup pin RA4 as receive pin and RA5 as transmission pin. Any of the two arguments can be set to NULL to allow transmission or receiving only.

The communication frequency rate on a PIC is derived from the main processor clock (_XTAL_FREQ), and therefore cannot necessarily be configured to exactactly match the required baud rate. The initialization function will attempt to find a prescaler-divider configuration which matches the required baud rate as closely as possible. Since it is likely impossible to derive an exact match, the function will return the actual baud rate as a function result. The caller should check if the returned baud rate falls within an acceptable parameter range and continue accordingly.

**Note**: serial_init() can fail to derive a working prescaler-divider combination altogether, in which case the funtion will return zero. In this case, the UART module will not be initialized at all:

~~~~~~~~~~~~~~~~~{.c}
Serial serial;
int real_baudrate = serial_init(&serial, PIN_RA4, PIN_RA5, 1000000000); // <- baud_rate way too high!
if (!real_baudrate) {
    // PIN_RA4 and PIN_RA5 were not reconfigured here!
    
    while (true) {}
}
~~~~~~~~~~~~~~~~~


# Sending/Receiving

- Internal buffers


# Example