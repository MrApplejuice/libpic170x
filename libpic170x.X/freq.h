/**
 * File:   freq.h
 * Author: paul
 *
 * Created on 20. Juli 2018, 20:12
 * 
 * 
 */

#ifndef FREQ_H
#define	FREQ_H

#ifndef _XTAL_FREQ 
#error "_XTAL_FREQ must be set to a valid value"
#endif

#if _XTAL_FREQ == 8000000
  #define OSCCON_BITS 0b01110010
  #define TIMER0_MS_INC 32
  #define TIMER0_US_INC 768
#elif _XTAL_FREQ == 2000000
  #define OSCCON_BITS 0b01100010
  #define TIMER0_MS_INC 131
  #define TIMER0_US_INC 072
#else
  #error "Invalid frequency"
#endif

#endif	/* FREQ_H */

