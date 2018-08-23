libpic170x library
==================

[TOC]

The libpic170x modular toolbox that is intended to make using or implemeting certain features of a PIC microcontroller less painful. It either gives a basis to work off, or provides a modular library to build an application from. The library was developed for the [PIC16(L)F1705](https://www.microchip.com/wwwproducts/en/PIC16F1705) and [PIC16(L)F1709](https://www.microchip.com/wwwproducts/en/PIC16F1709) microcontrollers. It is likely that the library will not work with other microcontrollers.

Please refer to the [Usage](#mainpage-usage) section for details on how to use the library.

# License

This library is licensed under the Apache License, Version 2.0. See the the attached [LICENSE.md](@ref LICENSE.md) file for details.

# Features

- [Timer0 library](@ref timer0-guide) for coarse time-keeping
- [Pin IO library](@ref pinio-guide) for reading from and writing to GPIO pins

# Building/Usage  {#mainpage-usage}

The library can be used in two different ways:

- [As inlined sources](@ref mainpage-as-inline)
- [A static library in the `*.lpp` format](@ref mainpage-as-library)

Using the library as inlined sources is most likely the most useful way of using the library since the library uses preprocessor macros to compile certain library settings directly into the library to save program memory.

## General requirements

In order to compile and use any part of the library you must have installed:

- [xc8 compiler](https://www.microchip.com/mplab/compilers) from Microchip

Additional dependecies:

- [GNU Make](https://www.gnu.org/software/make) for building static libraries and this documentation (not required, [see below](@ref mainpage-as-inline))
- [Doxygen](https://www.stack.nl/~dimitri/doxygen/) and [GraphViz](http://graphviz.org) for building this documentation

## Use as inlined sources    {#mainpage-as-inline}

Using the library as inlined sources is most likely the easiest way to use the library. Just copy all the source files for libraries that you would like to use into your your project and compile them yourself. There are not many things to consider. You only need to define `_XTAL_FREQ` to match your desired processor frequency and you are ready to roll!

## Build/use as static library     {#mainpage-as-library}

Using the library as a static library allows staying up to date with the latest releases of the library easily. However, due to the light-weight setup of the library certain aspects like clock speeds and prescaler factors are directly compiled to binary code and cannot be set when using the library. The values in question specifically are:

- Chip type. Supported chips are:
    - chip=16F1705
    - chip=16LF1705
    - chip=16F1709
    - chip=16LF1709
- Base frequency. Supported frequencies are:
    - xtal_freq=32000000
    - xtal_freq=16000000
    - xtal_freq=8000000
    - xtal_freq=4000000
    - xtal_freq=2000000
    - xtal_freq=1000000
    - xtal_freq=500000

To allow users to select the chip type and clock speed when setting up a project the compiled library name includes the parameter selection for `chip` and `frequency` in its name. Example:

~~~~~~~~~~~~~~~~~~~~~~~~
libpic170x_16LF1705_2000000.lpp
~~~~~~~~~~~~~~~~~~~~~~~~

This library is compiled for a PIC16LF1705 micro controller running its main processor clock at 2 MHz.

The sources for `libpic170x` contain a convenience bash-script with the name `build_all.sh` which will build all supported combinations of chip and frequencies in one go. Alternatively, if only one parameter set must be built, it is possible to control which library is being built by defining the make variables `chip` and `xtal_freq` and setting them to the apropriate parameters. Example: `make chip=16LF1705 xtal_freq=8000000` will build `libpic170x_16LF1705_8000000.lpp`.

### Project compile options

After the library is built, it will be placed in the install-directory of the project. Configure your PIC-project as follows:

- Add _XTAL_FREQ compiler definition matching the library build flags.
- Add include directory `install/include` to your project
- Add the correct link library for your settings from the `install/` directory

Then your are set. For a working example, check the blink-demo application from the examples-directory.


# Library dependency tree     {#mainpage-dependency-tree}

The dependency tree indicates what headers nad libraries are required to be included for usinga given feature. `freq.h` forms the basis for all other libraries since it defines important timing constants for all other libraries, so it must always be available. For all other libraries all direct and indirect dependecies must be available for the library to function.

\dot

digraph lib_dep_tree {
  rankdir=BT;

  node [shape=block,fontcolor=blue];

  freq_h[label="freq.h", URL="@ref freq.h"];
  timer0[URL="@ref timer0-guide"];

  timer0 -> freq_h;
}

\enddot
