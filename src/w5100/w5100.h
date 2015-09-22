#ifndef _W5100_H
#define _W5100_H

#include <stdint.h>

uint8_t w5100_spi_read(uint16_t addr);
int32_t w5100_spi_write(uint16_t addr, uint8_t data);
int32_t w5100_init();
int32_t w5100_udp_open();
int32_t w5100_udp_tx(uint8_t * buffer, uint16_t size);

#define W5100_TIMEOUT 10000

/* Return error codes */

#define EW5100_SOCK 1
#define EW5100_TXMEM 2
#define EW5100_IO 3
#define EW5100_ARG 4

#endif /* _W5100_H */
