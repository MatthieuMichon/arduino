#include <avr/io.h>
#include <avr/interrupt.h>

#define SPI_SS DDB2
#define SPI_MOSI DDB3
#define SPI_MISO DDB4
#define SPI_SCK DDB5


int8_t spi_enable()
{
  /* Set idle value for SPI pin */
  PORTB |= (1 << SPI_SS);

  /* Set direction for SPI pins */
  DDRB |= (1 << SPI_SS);
  DDRB |= (1 << SPI_MOSI);
  DDRB |= (1 << SPI_SCK);
  DDRB &= ~(1 << SPI_MISO);

  /* Enable SPI Master mode @ 1 MHz */
  SPCR |= (1 << MSTR);
  SPCR |= (1 << SPR0);
  SPCR |= (1 << SPE);

  /* Clear SPI interrupt flag */
  SPSR;
  SPDR;

  return 0;
}

int8_t spi_disable()
{
  SPCR &= ~(1 << SPE);

  return 0;
}

int8_t spi_write_read(uint8_t byte)
{
  SPDR = byte;
  while (!(SPSR & (1 << SPIF)));

  return SPDR;
}

void spi_write(uint8_t byte)
{
  spi_write_read(byte);
}

void spi_select(void)
{
  /* Set SS# output low */
  PORTB &= ~(1 << SPI_SS);
}

void spi_deselect(void)
{
  /* Set SS# output high */
  PORTB |= (1 << SPI_SS);
}
