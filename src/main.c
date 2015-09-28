#include <stdbool.h>
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "spi/spi.h"
#include "w5100/w5100.h"
#include "adc/adc.h"


#define ADC_CHANNELS 8
#define ADC_RESOLUTION 16

uint8_t size = ADC_RESOLUTION*ADC_CHANNELS/8;
uint8_t buffer [ADC_RESOLUTION*ADC_CHANNELS/8];

void blink (void)
{
  //spi_disable();

  /* set Timer0 Overflow Interrupt Enable bit */
  TIMSK0 = (1 << TOIE0);

  /* reset Timer0 value */
  TCNT0 = 0x00;

  /* set prescaler */
  TCCR0B = (1 << CS02);
  TCCR0B |= (1 << CS00);

  PORTB = 0xFF;

  sei();
}

int main (void)
{
  int32_t status;

  for (uint8_t i=0; i < size; i++) buffer[i] = i;

  spi_enable();
  status = w5100_init();
  if (status) while (true);

  status = w5100_udp_open();
  if (status) while (true);

  adc_init();

  if (!w5100_udp_tx(buffer, size)) blink();

  while (true);
}

/**
 * Interrupt Service Routine - Timer #0 overflow
 *
 * Should be fired not more than several dozen times per second
 */

ISR (TIMER0_OVF_vect)
{
  for (uint8_t i = 0; i < ADC_CHANNELS; i++) {
    buffer[i*(ADC_RESOLUTION/8)] = adc_read(i);
  }
  w5100_udp_tx(buffer, size);

  //PORTB ^= 0xFF;
}
