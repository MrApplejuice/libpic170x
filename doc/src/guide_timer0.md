Guide: Timer0 library  {#timer0-guide}
=====================

The timer0 library allows rough time keeping by implementing a counter that counts the elapsed milliseconds
utilizing the PIC's internal processor clock and timer0. 

- Setup function selects prescaler
    - Big prescale value to prevent hogging of processing resources
- Relies on constant processor frequency        
- Interrupt Handler to increment counter
- ms and us tracking