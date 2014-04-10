// ----------------------------------------------------------------------------
// Target:     Atmel AVR Tiny 13
// Date:       25-NOV-2008
// Author:     Simon Foster
// ----------------------------------------------------------------------------
// Revision:   1     First Release
// ----------------------------------------------------------------------------
// When PB1 (INT0) is pulled low Counter/Timer 0 produces an string of output
// pulses on PB0.  Notice that the pulse are either Low or High Z.
//
//                                Tiny 13
//                               +---v---+
//   (PCINT5/RESET/ADC0/dW)  PB5 |1     8| Vcc
//       (PCINT3/CLKI/ADC3)  PB3 |2     7| PB2  (SCK/ADC1/T0/PCINT2)
//            (PCINT4/ADC2)  PB4 |3     6| PB1  (MISO/AIN1/OC0B/INT0/PCINT1)
//                           GND |4     5| PB0  (MOSI/AIN0/OC0A/PCINT0)
//                               +-------+
//
//    Pin      Function          Comments
// ----------------------------------------------------------------------------
//    1-3      DNC
//
//    4        GND               Ground
//
//    5        PB0               Digital Output Pin (0V/HiZ)
//
//    6        PB1               Digital Input Pin.  Low at this pin activates rapid-fire function.
//                               Internal pullup is activated.
//
//    7        DNC
//
//    8        Vcc               +ve supply voltage
//                               ATtiny13V: 0 - 4 MHz @ 1.8 - 5.5V, 0 - 10 MHz @ 2.7 - 5.5V
//                               ATtiny13: 0 - 10 MHz @ 2.7 - 5.5V, 0 - 20 MHz @ 4.5 - 5.5V
//
// ----------------------------------------------------------------------------

#define F_CPU 1200000UL                      // Tiny 13 as delivered
                                             // 9.6 MHz internal clock with CKDIV8 set
#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <util/delay.h>

ISR( TIM0_COMPA_vect )
{
   DDRB ^= 1;                                // Toggle PB0 between pulled Low and High Z
}

int main( void )
{
   TCCR0A   = _BV( WGM01 );                  // Set timer mode to Clear Timer on Compare (CTC)
   TIMSK0   = _BV( OCIE0A );                 // Enable Timer 0 Compare Match A Interrupt
   OCR0A    = 40;                            // Set Output Compare Register A.  One tick is 853us

   PORTB    |= 2;                            // Enable pull up on PB1

   sei();                                    // enable interrupts

   while(1)
   {
      if ( bit_is_set( PINB, 1 ))            // INT0 is high ie. switch released
      {
         TCCR0B = 0;                         // Stop the timer
         DDRB = 0;                           // Turn the output off
      }
      else
      {
         if ( TCCR0B == 0 )
         {
            TCNT0 = 0;                       // Reset timer
            TCCR0B   = 0x05;                 // Start the timer freq. is 1200000/1024 => 1172/sec
         }
      }
   }
}
