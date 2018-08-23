Pin IO library                    {#pinio-guide}
==============

[TOC]

The PinIO library io_control.h allows easy control of a PIC's GPIO pins. It also provides definitions that are internally used by other libraries for interacting with pins. The current set of features is:

- Switching pin netween input and output mode
- Reading input/output states of individual pins
- Writing output values

# Reading pin data

When a PIC is reset, all pins are set to "input, high impedance mode". Therefore, reading data from individual pins is simply done by invoking the [pin_get_input(PinDef*)](@ref pin_get_input) function. A [PinDef*](@ref PinDef) defines the pin for which the input state should be retrieved. All pins of supported PICs are defines as contants of the form `PIN_RA0`, referring to pin RA0 on a supported PIC. If a pin is not present on a given PIC, like, for example, `PIN_RB4` on a PIC16LF1705, uses of that variable will still be valid but calls to any function in io_control.h for that pin will result in NOP.

~~~~~~~~~~~~~~~~~~~~~~~~~{.c}
// ...
#include <io_control.h>

int main() {
    bool pin_state = pin_get_input(PIN_RA0);
    // True will mean that the pin was high, false means the pin was low
}
~~~~~~~~~~~~~~~~~~~~~~~~~

# Switching pin modes

To swtich a pin from input to low-impedance output mode, you can use the function [pin_set_pin_mode()](@ref pin_set_pin_mode). The function takes two arguments, the first being the pin to set, the second being the mode, where `true` indicates that the pin should be configured in output mode, and `false` that the pin should be configured in input mode.

~~~~~~~~~~~~~~~~~~~~~~~{.c}

// ...
#include <io_control.h>

int main() {
    // switch pin RA1 to output-mode
    pin_set_pin_mode(PIN_RA1, true);
}

~~~~~~~~~~~~~~~~~~~~~~~

Additionally, pins can be switched between analog and digital input modes. By default all pins are configured to use analog input mode, but for digital signals it is generally advisable to switch pins to ditigal input mode, and even required if an input pin is meant to be used with any other digital subsystem like the UART module. Switching the input mode can be done using the function [pin_set_input_mode()](@ref pin_set_input_mode). The function can set the input mode a pin to:

- `PIN_INPUT_MODE_DIGITAL` digital mode
- `PIN_INPUT_MODE_ANALOG` analog mode (the default)

When intending to read digital input signals, a better example of the first code snipped shown on this page would therefore be:

~~~~~~~~~~~~~~~~~~~~~~~~~{.c}
// ...
#include <io_control.h>

int main() {
    // First switch to digital input mode before attempting to 
    // read the ditial signal
    pin_set_input_mode(PIN_RA0, PIN_INPUT_MODE_DIGITAL);

    bool pin_state = pin_get_input(PIN_RA0);
    // True will mean that the pin was high, false means the pin was low
}
~~~~~~~~~~~~~~~~~~~~~~~~~

# Writing pin outputs

If a pin has been configured as an output, it is likely that one wnnts to write out data via the pin. This can be achived by using the function [pin_set_output()](@ref pin_set_output). The function takes a single argument, namely the intended output state of the pin.

A simple "led blink" example:


~~~~~~~~~~~~~~~~~~~~~~~~~{.c}
// ...
#include <io_control.h>

int main() {
    // Use RA0 as an output blink an LED on.
    pin_set_pin_mode(PIN_RA0, true);

    while (true) {
        // Toggle output by reading current output and inverting it with '!'
        pin_set_output(PIN_RA0, !pin_get_input(PIN_RA0));
        // Wait 100ms to make the blinking visible
        __delay_ms(100);
    }
}
~~~~~~~~~~~~~~~~~~~~~~~~~


