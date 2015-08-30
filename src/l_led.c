#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>

int main (void)
{
  uint16_t i;

  DDRB = 0xFF;

  while (1) {
    PORTB = 0x00;
    for (i = 0; i < 500; i ++) _delay_ms(1); // 16 ms @ 16 MHz
    PORTB = 0xFF;
    for (i = 0; i < 500; i ++) _delay_ms(1); // 16 ms @ 16 MHz
  }
}
