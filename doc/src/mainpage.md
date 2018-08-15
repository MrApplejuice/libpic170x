libpic170x library
==================

[TOC]

The libpic170x modular toolbox library that is intended to make using certain functions of a PIC microcontroller less painful to implement by either giving a basis to work off, or asa a toolbox to take components from. The library was developed for the [PIC16(L)F1705](https://www.microchip.com/wwwproducts/en/PIC16F1705) and [PIC16(L)F1709](https://www.microchip.com/wwwproducts/en/PIC16F1709) microcontrollers. It is likely that the library will not work with other microcontrollers.

Please refer to the [Usage](#mainpage-usage) section for details on how to use the library before attempting to use it!

# Features

- [Timer0 library](@ref timer0-guide) for rough time-keeping

# Usage  {#mainpage-usage}

The library can be used in two different ways:

- A static library in the `*.lpp` format
- As inlined sources

Using the library as inlined sources is most likely the most useful way of using the library since the library uses preprocessor macros to compile certain library settings directly into the library to save program memory.

## Requirements

In order to compile and use any piece of the project you must have installed:

- [xc8 compiler](https://www.microchip.com/mplab/compilers) from Microchip

Additional dependecies:

- [GNU Make](https://www.gnu.org/software/make) for building static libraries and this documentation (not required, [see below](@ref mainpage-as-inline))
- [Doxygen](https://www.stack.nl/~dimitri/doxygen/) and [GraphViz](http://graphviz.org) for building this documentation

## Use as inlined sources    {#mainpage-as-inline}

Using the library as inlined sources is most likely the easiest way to use the library. Just copy all the source files for libraries that you would like to use into your your project and compile them yourself. There are not many things to consider. You only need to define `_XTAL_FREQ` to match your desired processor frequency and you are ready to roll!

## Use as static library

There is not much to gain by using this form of inclusion, I feel, but it is possible. 

**TODO**

- Library must be compiled with specific settings
    - Dependent on _XTAL_FREQ
    - Extra settings in a custom `defaults.h` file

# Library dependency tree

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
