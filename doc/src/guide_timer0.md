Guide: Timer0 library  {#timer0-guide}
=====================

The timer0 library allows rough time keeping by implementing a counter that counts the elapsed milliseconds (and micro seconds) using the PIC's timer0. The counter is updated using an interrupt handler that must be invoked in the general interrupt handler routine. The counter explicitly is designed for _rough_ time keeping to not waste too many processor cycles on updating the counter. Increments are limited to a maximum of ~131 ms by adjusting the internal timer0 prescaler. This should be generally be precise enough to handle user-oriented delays and timings. This library is explicitly not designed for close time keeping.


Timing details
--------------

The details on how timer0 works can be read in the PIC specifications. However, generally timer0 is configured to use the PIC's base-clock, which is defined using `_XTAL_FREQ` and definitions from freq.h. Selecting a higher base-clock, will generally lead to a higher timer resolution. For low frequencies the timer0 prescaler is adjusted to increase the relative frequency of counter updates so that the maximum interval between counter increments stays at ~131ms. Note that this increased relative update rate will use up more processor cycles. Generally, it is advisable to only use `_XTAL_FREQ >= 2000000` so that the maxium timer0 prescaler of 256 can be used which will lead to less frequent updates, therefore costing the least possible processing overhead with a good update resolution of `<= 65ms`.

Prescaler values and increments can be changed in `freq.h` by adjusting the `TIMER0_`-definitions directly.

Using timer0.h
--------------

timer0.h defines the global struc Timer0 which contains the readable, and writeable counter values. It also defines a global variable [pic170x_timer0](@ref pic170x_timer0) whcih is the default data structure that will be used if no custom data structure is defined. Note that this global data structure must be initializted as well before being used by calling `timer0_init(NULL)`.

The simplest way to use the timer0 library is to rely on the global data structure:

~~~~~~~~~~~~~~~~{.c}

#include <xc.h>
#include <timer0.h>

// ... config words etc ...

void interrupt interrupt_handler() {
  timer0_ih(NULL);
}

int main() {
  OSCCON = OSCCON_BITS;

  timer0_init(NULL);
  GIE = 1; // Interrupts must be enabled manually

  __delay_ms(1000);

  int ms_elapsed = pic170x_timer0.ms;
  // ...
}

~~~~~~~~~~~~~~~~

Things to note here are that the interrupt handler must be defined in the main-application and invoke timer0_ih(). Also, even though timer0_init() enables timer0-interrupts, the global interrupt flag must still be enabled to enable the counter. After doing so the counter will be updated with every timer0-interrupt that is generated.

In the example, `pic170x_timer0.ms` is read to extract the ms that elapsed since the start of the program. Another, feature that might be interesting is that the values of the data structure are also writable, allowing the implementation of simple wait patterns, like these:


~~~~~~~~~~~~~~~~~~{.c}

//...

pic170x_timer0.ms = 0;
while (pic170x_timer0.ms < 10000) {
  // do something
}

// continue with other things

~~~~~~~~~~~~~~~~~~

This can be useful when waiting for user input, or to implement blocking I/O with timeout functionality. One important thing to keep in mind is that if doing read-check-modify operations interrupt-updated values (e.g. `pic170x_timer0`), GIE should be disabled during the interaction, to prevent undefined states from ocurring:

~~~~~~~~~~~~~~~~~~{.c}

// Prevent timer from being updated by
// the interrupt, leading to undefined values
GIE = 0;

if (pic170x_timer0.ms > 1000) {
  pic170x_timer0.ms -= 1000;
  GIE = 1;

  do_sth();
} else {
  GIE = 1;
}


~~~~~~~~~~~~~~~~~~

