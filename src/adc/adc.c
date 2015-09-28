#include <avr/io.h>

#define ADC_ADMUX_MUX_MASK ((1<<MUX3)|(1<<MUX2)|(1<<MUX1)|(1<<MUX0))

int32_t adc_init (void)
{
  /* Configure ADC Clock: MCU clock / 128 = 125 kHz */
  ADCSRA = (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);

  /* Configure ADC reference and input mux */
  ADMUX = (1<<REFS0);

  /* Disable Power Reduction ADC */
  PRR &= ~(1<<PRADC);

  /* Enable ADC */
  ADCSRA |= (1<<ADEN);

  return 0;
}

uint16_t adc_read (uint8_t ch)
{
  /* Select channel */
  ADMUX = ((ADMUX & ~ADC_ADMUX_MUX_MASK) | (ch & ADC_ADMUX_MUX_MASK));

  /* Enable single conversion mode */
  ADCSRA |= (1<<ADSC);

  /* Spin while ADC is busy */
  while (ADCSRA & (1<<ADSC));

  /* Return 16-bit ADC value */
  return ADC;
}
