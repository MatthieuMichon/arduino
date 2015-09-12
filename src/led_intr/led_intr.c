#include <stdint.h>
#include <stdbool.h>
#include <avr/io.h>
#include <avr/interrupt.h>

int32_t init_timers (void)
{
  /* set Timer0 Overflow Interrupt Enable bit */
  TIMSK0 = (1 << TOIE0);

  /* reset Timer0 value */
  TCNT0 = 0x00;

  /* set prescaler */
  TCCR0B = (1 << CS02) | (1 << CS00);
}


int main (void)
{
  uint16_t i;

  DDRB = 0xFF;

  init_timers();
  sei();
  PORTB = 0xFF;
  while (true);
}


ISR(TIMER0_OVF_vect)
{
  PORTB ^= 0xFF;
}
