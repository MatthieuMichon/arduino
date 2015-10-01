#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "spi/spi.h"
#include "w5100/w5100.h"
#include "adc/adc.h"


#define ADC_CHANNELS 8
#define ADC_RESOLUTION 16

#define BUFFER_SIZE 96
uint8_t size = BUFFER_SIZE;
uint8_t buffer [BUFFER_SIZE];

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
  memset((char*) buffer, 0, BUFFER_SIZE);
  strcpy((char*) buffer, "ADC: ");
  for (uint8_t i = 0; i <= ADC_CHANNELS; i++) {
    buffer[5+i*5] = '0' + i;
    buffer[5+i*5+1] = ':';
    buffer[5+i*5+2] = ' ';
    buffer[5+i*5+3] = adc_read(i);
  }
  w5100_udp_tx(buffer, size);

  //PORTB ^= 0xFF;
}
