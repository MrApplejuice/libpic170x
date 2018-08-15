libpic170x library
==================

[TOC]

The libpic170x modular toolbox library that is intended to make using certain functions of a PIC microcontroller less painful to implement by either giving a basis to work off, or asa a toolbox to take components from. The library was developed for the [PIC16(L)F1705](https://www.microchip.com/wwwproducts/en/PIC16F1705) and [PIC16(L)F1709](https://www.microchip.com/wwwproducts/en/PIC16F1709) microcontrollers. It is likely that the library will not work with other microcontrollers.

Please refer to the [Usage](#mainpage-usage) section for details on how to use the library before attempting to use it!

# Features

- [Timer0 library](@ref timer0-guide) for rough time-keeping


# Usage  {#mainpage-usage}

- Library must be compiled with specific settings
    - Dependent on _XTAL_FREQ
    - Extra settings in a custom `defaults.h` file