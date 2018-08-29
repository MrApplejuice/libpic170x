Serial library                   {#serial-guide}
==============

[TOC]

The serial library [serial.h](@ref serial.h) provides access to the PIC's UART module and allows it to be used for simple serial communication. Specifically, currently only simple RS232-based 8-bit protocols are supported (e.g. no parity bits, etc). The maximum transmission and receive voltages are given by the used chip types, which means 3.3V for the LF-pic variants and 5V for the F-variants.

Do not connect your chip to to 12V RS232 directly, it will fry your Microcontroller!


# Initialization

- Optional receive/Send only
- Baud-rate


# Sending/Receiving

- Internal buffers


# Example