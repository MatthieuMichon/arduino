#ifndef _SPI_H
#define _SPI_H

#include <stdint.h>

int8_t spi_enable(void);
int8_t spi_disable(void);
int8_t spi_write_read(uint8_t byte);
void spi_write(uint8_t byte);
void spi_select(void);
void spi_deselect(void);

#endif /* _SPI_H */
