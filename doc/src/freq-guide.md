Library configuration header             {#freq-guide}
============================

[TOC]

The header file libpic170x/freq.h contains library frequency and timing related configurations and functions to verify that a linked library matches the configuration of a built target project. Verifying timing settings is important because the main processor frequency is used for many components of libpic170x and therefore must match the frequency that the PIC micro controller is run on.

# Matching library and project configurations

The library can be built for different PIC micro controllers and/or different *static* processor speeds (see [build instructions](@ref mainpage-as-library) for details). When using libpic170x by including it as a static library, one must select the apropriate static library for the selected chip type and chip processing speed. The chip type and clock frequencies are burned into the different variants of libpic170x during building and all libraries depend on this burned-in chip type and clock frequency.

## Configuring the correct processor type and frequency

The processor type for a project using libpic170x is implicitly declared through preprocessor macros exposed by xc8's `xc.h` header. The processor frequency, is declared by delcaring the preprocessor macro `_XTAL_FREQ` which is also optionally used by `xc.h`, but it is required to be declared before using any component of libpic170x. To set _XTAL_FREQ project-wide, it is advisable to define it in the project settings such that it is used for all source files of a given project, for example:

~~~~~~~~~~~~~
xc8 -D_XTAL_FREQ=1000000
~~~~~~~~~~~~~

However, it is also possible for a small project to locally define the macro or define it through a config-header file which is include before `freq.h` is included.

To help configuring the actual processor speed to match _XTAL_FREQ, freq.h defines the macro OSCCON_BITS which can be directly assigned to the OSCCON register to set the processor speed. It is advisable to do this at the very start of your program:

~~~~~~~~~~~~~~{.c}
#define _XTAL_FREQ 1000000

#include <lipbic170x/timer0.h> // This will include freq.h

int main() {
    // Set OSCCON bits from macro that is defined in freq.h
    OSCCON = OSCCON_BITS;

    // ... now start the program ...
}
~~~~~~~~~~~~~~

## Verify correct configuration

To check if the configured project _XTAL_FREQ and processor type matches the static link library, freq.h defines a handy but optional utility function libpic170x_check_library_build_arguments(). The function can be called to verify that the correct project configuration has been used to build a project that links to a libpic170x static library. It returns true, if the configuration parameter match the library's configuration. I case that the function returns false, the application should exit into a safe fail state and not attempt to use libpci170x since its behavior will be undefined. As such, if you call the check-function, you should do so ath the very start of your program.

See the [blink.X/main.c](@ref blink.X/main.c) for an example.


# Configuration macros

