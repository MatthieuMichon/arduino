#ifndef _W5100_H
#define _W5100_H

#include <stdint.h>

uint8_t w5100_spi_read(uint16_t addr);
void w5100_spi_write(uint16_t addr, uint8_t data);
int32_t w5100_init();
int32_t w5100_udp_open();
int32_t w5100_udp_tx(uint8_t * buffer, uint16_t size);


#define ESOCK 1
#define ETXMEM 2

#endif /* _W5100_H */
