#include <stdbool.h>
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "spi/spi.h"
#include "w5100/w5100.h"

uint8_t size = 64;
uint8_t buffer [64];

void blink (void)
{
  /* set Timer0 Overflow Interrupt Enable bit */
  TIMSK0 = (1 << TOIE0);

  /* reset Timer0 value */
  TCNT0 = 0x00;

  /* set prescaler */
  TCCR0B = (1 << CS02) | (1 << CS00);
  sei();
  PORTB = 0xFF;

}

int main (void)
{
  spi_enable();
  if (w5100_init() == 0) blink();
  //if (w5100_udp_open() == 0) blink();
  spi_disable();
  //blink();
  //for (uint8_t i=0; i < size; i++) buffer[i] = i;
  while (true);
}

ISR(TIMER0_OVF_vect)
{
  PORTB ^= 0xFF;
  return;
  cli();
  uint8_t retval = w5100_udp_tx(buffer, size);
  if (retval == 0) PORTB ^= 0xFF;
  sei();
}
