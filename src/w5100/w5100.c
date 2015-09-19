#include <avr/interrupt.h>

#include "../spi/spi.h"
#include "w5100.h"
#include "w5100_regs.h"


#define WOP 0xF0
#define ROP 0x0F

uint8_t gw[4] = {192, 168, 1, 254};
uint8_t mask[4] = {255, 255, 255, 0};
uint8_t mac[6] = {0xCA, 0xFE, 0xDE, 0xCA, 0xFA, 0xCE};
uint8_t ip[4] = {192, 168, 1, 22};
uint8_t src_port[2] = {0x10, 0x00};
uint8_t bc[4] = {192, 168, 1, 255};
uint8_t src_dport[2] = {0x20, 0x00};

uint8_t w5100_spi_read(uint16_t addr)
{
  uint8_t data;
  cli();
  spi_select();
  spi_write(ROP);
  spi_write(addr >> 8);
  spi_write(addr & 0x00FF);
  data = spi_write_read(0x00 /* dummy */);
  spi_deselect();

  sei();
  return data;
}

void w5100_spi_write(uint16_t addr, uint8_t data)
{
  cli();
  spi_select();
  spi_write(WOP);
  spi_write(addr >> 8);
  spi_write(addr & 0x00FF);
  spi_write(data);
  spi_deselect();
  sei();
}

int32_t w5100_init()
{
  /* Perform software reset */
  w5100_spi_write(MR, (1 << RST));

  /* Set gateway IP */
  w5100_spi_write(GAR0, 0xDD);
  if (w5100_spi_read(GAR0) != 0xDD) return -1;
  w5100_spi_write(GAR1, gw[1]);
  w5100_spi_write(GAR2, gw[2]);
  w5100_spi_write(GAR3, gw[3]);

  /* Set netmask */
  w5100_spi_write(SUBR0, mask[0]);
  w5100_spi_write(SUBR1, mask[1]);
  w5100_spi_write(SUBR2, mask[2]);
  w5100_spi_write(SUBR3, mask[3]);

  /* Set MAC address */
  w5100_spi_write(SHAR0, mac[0]);
  w5100_spi_write(SHAR1, mac[1]);
  w5100_spi_write(SHAR2, mac[2]);
  w5100_spi_write(SHAR3, mac[3]);
  w5100_spi_write(SHAR4, mac[4]);
  w5100_spi_write(SHAR5, mac[5]);

  /* Set IP address */
  w5100_spi_write(SIPR0, ip[0]);
  w5100_spi_write(SIPR1, ip[1]);
  w5100_spi_write(SIPR2, ip[2]);
  w5100_spi_write(SIPR3, ip[3]);

  return 0;
}

#define W5100_TIMEOUT 100

int32_t w5100_udp_open()
{
  /* Switch socket #0 to UDP mode */
  w5100_spi_write(S0_MR, UDP);

  /* Set socket #0 source port number */
  w5100_spi_write(S0_PORT0, src_port[0]);
  w5100_spi_write(S0_PORT1, src_port[1]);

  /* Send socket #0 Open command */
  w5100_spi_write(S0_CR, OPEN);

  /* Check if socket #0 is up */
  uint8_t retries;
  do {
    if (w5100_spi_read(S0_SR) == SOCK_UDP) break;
    retries++;
  } while (retries < W5100_TIMEOUT);
  if (retries == W5100_TIMEOUT) return -ESOCK;

  /* Set remote IP */
  w5100_spi_write(S0_DIPR0, bc[0]);
  w5100_spi_write(S0_DIPR1, bc[1]);
  w5100_spi_write(S0_DIPR2, bc[2]);
  w5100_spi_write(S0_DIPR3, bc[3]);

  /* Set remote port */

  /* Set socket #0 source port number */
  w5100_spi_write(S0_DPORT0, src_dport[0]);
  w5100_spi_write(S0_DPORT1, src_dport[1]);

  return 0;
}

int32_t w5100_udp_tx(uint8_t * buffer, uint16_t size)
{
  uint16_t tx_offset;
  uint16_t tx_ptr;
  uint16_t i;

  /* Check that Tx memory free size is enough */
  if (w5100_spi_read(S0_TX_FSR1) < (1024 >> 8)) return -ETXMEM;

  /* Get Tx offset */
  tx_offset = ((w5100_spi_read(S0_TX_WR0) & 0x00FF) << 8) + w5100_spi_read(S0_TX_WR1);

  /* Write buffer contents */
  for (i = 0; i < size; i++) {
    tx_ptr = S0_TX_BASE + ((tx_offset + i) & S0_TX_MASK);
    w5100_spi_write(tx_ptr, buffer[i]);
  }

  /* Update Tx offset */
  tx_offset = ((tx_offset + size) & S0_TX_MASK);

  /* Issue send command */
  w5100_spi_write(S0_CR, SEND);

  /* Wait for command to complete */
  while (w5100_spi_read(S0_CR));

  return 0;
}
