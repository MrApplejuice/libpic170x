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
 * \file freq.h
 * \brief Global timing definitions used for timing critical library components.
 * 
 * 
 * This file defines important timing preprocessor variables that are used 
 * throughout all library components. All libraries are based on the 
 * assumption that the processor clock frequency the PIC microcontroller 
 * operates on does not change over time. It should be set once at the start
 * of the program never be touched again.
 * 
 * The frequencies are set via the OSCCON bits of the PIC microcontroller.
 * freq.h derivs the correct OSCCON-bits from the defined _XTAL_FREQ. _XTAL_FREQ
 * must therefore as a compiler definition from the outside. By default, it is
 * set to 8000000 in `Makefile` but any of the following _XTAL_FREQ frequencies 
 * are supported by libpic170x:
 * 
 * \code{.unparsed}
 *   32000000
 *   16000000
 *   8000000
 *   4000000
 *   2000000
 *   1000000
 *   500000
 * \endcode
 * 
 * Lower frequencies are not supported because the core library timer0 would 
 * become unusable due to huge counter steps.
 * 
 * OSCCAL_BITS
 * 
 * One of the core-featues of `freq.h` is that is exposes the OSCCON bits that 
 * the  main application should set through the preprocessor definition 
 * `OSCCON_BITS`. Before using any other library functions this OSCCON bits 
 * should be set by the application using libpic170x.
 * 
 * Example:
 * 
 * \code{.c}
 *   #include <freq.h>
 *   #include <xc.h>
 * 
 *   int main() {
 *     OSCCON = OSCCON_BITS;
 *     // ...
 *   }
 * \endcode
 */

#ifndef FREQ_H
#define	FREQ_H

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#ifndef _XTAL_FREQ 
#error "_XTAL_FREQ must be set to a valid value"
#endif

#ifdef __LIBPIC170X_DOXYGEN
  //! OSCCON bits that can derived from the set _XTAL_FREQ that the OSCCON register can be initailized with.
  #define OSCCON_BITS 0
  //! Processor name internally used by libpic170x_check_library_build_arguments()
  #define __LIBPIC170X_DEVICE_NAME "PIC16LF1705"
#endif

#if _XTAL_FREQ == 32000000
  #define OSCCON_BITS 0b11110010

  #define TIMER0_MS_INC 8
  #define TIMER0_US_INC 192
  #define TIMER0_PRESCALE_BITS 0x07
#elif _XTAL_FREQ == 16000000
  #define OSCCON_BITS 0b01111010

  #define TIMER0_MS_INC 16
  #define TIMER0_US_INC 384
  #define TIMER0_PRESCALE_BITS 0x07
#elif _XTAL_FREQ == 8000000
  #define OSCCON_BITS 0b01110010

  #define TIMER0_MS_INC 32
  #define TIMER0_US_INC 768
  #define TIMER0_PRESCALE_BITS 0x07
#elif _XTAL_FREQ == 4000000
  #define OSCCON_BITS 0b01101010

  #define TIMER0_MS_INC 65
  #define TIMER0_US_INC 536
  #define TIMER0_PRESCALE_BITS 0x07
#elif _XTAL_FREQ == 2000000
  #define OSCCON_BITS 0b01100010

  #define TIMER0_MS_INC 131
  #define TIMER0_US_INC 072
  #define TIMER0_PRESCALE_BITS 0x07
#elif _XTAL_FREQ == 1000000
  #define OSCCON_BITS 0b01011010

  #define TIMER0_MS_INC 131
  #define TIMER0_US_INC 072
  #define TIMER0_PRESCALE_BITS 0x06
#elif _XTAL_FREQ == 500000
  #define OSCCON_BITS 0b00111010

  #define TIMER0_MS_INC 131
  #define TIMER0_US_INC 072
  #define TIMER0_PRESCALE_BITS 0x05
#else
  #if _XTAL_FREQ < 500000
    #error "Invalid frequency (frequencies < 500kHz not supported)"
  #else
    #error "Invalid frequency"
  #endif
#endif

#ifdef _16LF1705
  #define __LIBPIC170X_DEVICE_NAME "PIC16LF1705"
#elif _16F1705
  #define __LIBPIC170X_DEVICE_NAME "PIC16F1705"
#elif _16LF1709
  #define __LIBPIC170X_DEVICE_NAME "PIC16ÖF1709"
#elif _16F1709
  #define __LIBPIC170X_DEVICE_NAME "PIC16F1709"
#else
  #error "Unsupported chip"
#endif

//! Stores the _XTAL_FREQ that was used while building the static libpic170x library
extern const uint32_t __LIBPIC170X_BUILT_XTAL_FREQ;
//! Stores the device name (as string) used to build the static libpic170x library
extern const char* __LIBPIC170X_BUILT_DEVICE_NAME;

/**
 * \brief Verify matching configuration parameters between main project and static library
 * 
 * This functions allows one to check if the arguments used while building the
 * library are matching the build arguments for the current project. Generally,
 * this should not be required, but it can be handy for sanity checks. It is
 * advisable to include a call to this function at the start of any program and
 * exit to a safe fail-state when the function returns false since the behavior
 * of libpic170x is undefined if the build arguments do not match between
 * a libpic170x static library and main application.
 * 
 * @return 
 *     Returns true in case the build arguments are matching. If the function
 *     returns false, exit to a safe fail-state.
 */
static bool libpic170x_check_library_build_arguments() {
    return 
        (strcmp(__LIBPIC170X_BUILT_DEVICE_NAME, __LIBPIC170X_DEVICE_NAME) == 0)
        && (__LIBPIC170X_BUILT_XTAL_FREQ == _XTAL_FREQ);
}

#endif	/* FREQ_H */

