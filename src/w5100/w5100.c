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
  if (0x00 != spi_write_read(ROP)) return 0;
  if (0x01 != spi_write_read(addr >> 8)) return 0;
  if (0x02 != spi_write_read(addr & 0x00FF)) return 0;
  data = spi_write_read(0x00 /* dummy */);
  spi_deselect();

  sei();
  return data;
}

uint16_t w5100_spi_read_short(uint16_t addr)
{
  uint16_t retval = 0;
  retval = (w5100_spi_read(addr) << 8);
  retval += w5100_spi_read(addr+1);

  return retval;
}

uint32_t w5100_spi_read_long(uint16_t addr)
{
  uint32_t retval = 0;
  retval = ((uint32_t) w5100_spi_read(addr) << 24);
  retval += ((uint32_t) w5100_spi_read(addr+1) << 16);
  retval += ((uint32_t) w5100_spi_read(addr+2) << 8);
  retval += (uint32_t) w5100_spi_read(addr+3);

  return retval;
}

int32_t w5100_spi_write(uint16_t addr, uint8_t data)
{
  /* Select the W5100 through SPI */
  spi_select();

  /* Send Write Operation word */
  if (0x00 != spi_write_read(WOP)) return -EW5100_IO;
  if (0x01 != spi_write_read(addr >> 8)) return -EW5100_IO;
  if (0x02 != spi_write_read(addr & 0x00FF)) return -EW5100_IO;
  if (0x03 != spi_write_read(data)) return -EW5100_IO;

  /* Release the W5100 */
  spi_deselect();

  return 0;
}

int32_t w5100_spi_write_short(uint16_t addr, uint16_t data)
{
  int32_t status;
  status = w5100_spi_write(addr, (data >> 8) & 0x00FF);
  if (status) return status;
  status = w5100_spi_write(addr+1, data & 0x00FF);
  if (status) return status;

  return 0;
}

int32_t w5100_spi_write_long(uint16_t addr, uint32_t data)
{
  int32_t status;
  status = w5100_spi_write(addr, (data >> 24) & 0xFF);
  if (status) return status;
  status = w5100_spi_write(addr+1, (data >> 16) & 0xFF);
  if (status) return status;
  status = w5100_spi_write(addr+2, (data >> 8) & 0xFF);
  if (status) return status;
  status = w5100_spi_write(addr+3, data & 0xFF);
  if (status) return status;

  return 0;
}

/* High Level */

int32_t w5100_init()
{
  int32_t status;

  /* Perform software reset */
  status = w5100_spi_write(MR, (1 << RST));
  if (status) return status;

  /* Set gateway IP */
  status = w5100_spi_write(GAR0, 0xDD);
  if (status) return status;
  status = w5100_spi_write(GAR1, gw[1]);
  if (status) return status;
  status = w5100_spi_write(GAR2, gw[2]);
  if (status) return status;
  status = w5100_spi_write(GAR3, gw[3]);
  if (status) return status;

  /* Set netmask */
  status = w5100_spi_write(SUBR0, mask[0]);
  if (status) return status;
  status = w5100_spi_write(SUBR1, mask[1]);
  if (status) return status;
  status = w5100_spi_write(SUBR2, mask[2]);
  if (status) return status;
  status = w5100_spi_write(SUBR3, mask[3]);
  if (status) return status;

  /* Set MAC address */
  status = w5100_spi_write(SHAR0, mac[0]);
  if (status) return status;
  status = w5100_spi_write(SHAR1, mac[1]);
  if (status) return status;
  status = w5100_spi_write(SHAR2, mac[2]);
  if (status) return status;
  status = w5100_spi_write(SHAR3, mac[3]);
  if (status) return status;
  status = w5100_spi_write(SHAR4, mac[4]);
  if (status) return status;
  status = w5100_spi_write(SHAR5, mac[5]);
  if (status) return status;

  /* Set IP address */
  status = w5100_spi_write(SIPR0, ip[0]);
  if (status) return status;
  status = w5100_spi_write(SIPR1, ip[1]);
  if (status) return status;
  status = w5100_spi_write(SIPR2, ip[2]);
  if (status) return status;
  status = w5100_spi_write(SIPR3, ip[3]);
  if (status) return status;

  /* Set Rx per socket memory */
  status = w5100_spi_write(RMSR, 0x55);
  if (status) return status;

  /* Set Tx per socket memory */
  status = w5100_spi_write(TMSR, 0x55);
  if (status) return status;

  return 0;
}

int32_t w5100_socket_close(uint8_t socket)
{
  int32_t status;

  /* Limit support to socket #0 */
  if (socket) return -EW5100_ARG;

  /* Close socket #0 */
  status = w5100_spi_write(S0_CR, SN_CR_CLOSE);
  if (status) return status;

  /* Wait for command to complete */
  uint32_t retries = 0;
  do {
    if (w5100_spi_read(S0_CR) == SN_CR_CLEAR) break;
    retries++;
  } while (retries < W5100_TIMEOUT);
  if (retries == W5100_TIMEOUT) return -EW5100_SOCK;

  /* Clear interrupt lines */
  status = w5100_spi_write(IR, 0xFF);
  if (status) return status;

  return 0;
}

int32_t w5100_udp_open()
{
  int32_t status;

  /* Close socket #0*/
  status = w5100_socket_close(0);
  if (status) return status;

  /* Switch socket #0 to UDP mode */
  status = w5100_spi_write(S0_MR, UDP);
  if (status) return status;

  /* Set socket #0 source port number */
  status = w5100_spi_write(S0_PORT0, src_port[0]);
  if (status) return status;
  status = w5100_spi_write(S0_PORT1, src_port[1]);
  if (status) return status;

  /* Send socket #0 Open command */
  status = w5100_spi_write(S0_CR, SN_CR_OPEN);
  if (status) return status;

  /* Wait for command to complete */
  uint32_t retries = 0;
  do {
    if (w5100_spi_read(S0_CR) == SN_CR_CLEAR) break;
    retries++;
  } while (retries < W5100_TIMEOUT);
  if (retries == W5100_TIMEOUT) return -EW5100_SOCK;

  /* Check if socket #0 is up */
  retries = 0;
  do {
    if (w5100_spi_read(S0_SR) == SOCK_UDP) break;
    retries++;
  } while (retries < W5100_TIMEOUT);
  if (retries == W5100_TIMEOUT) return -EW5100_SOCK;

  /* Set remote IP */
  status = w5100_spi_write(S0_DIPR0, bc[0]);
  if (status) return status;
  status = w5100_spi_write(S0_DIPR1, bc[1]);
  if (status) return status;
  status = w5100_spi_write(S0_DIPR2, bc[2]);
  if (status) return status;
  status = w5100_spi_write(S0_DIPR3, bc[3]);
  if (status) return status;

  /* Set remote port */
  status = w5100_spi_write(S0_DPORT0, src_dport[0]);
  if (status) return status;
  status = w5100_spi_write(S0_DPORT1, src_dport[1]);
  if (status) return status;

  return 0;
}

int32_t w5100_udp_tx(uint8_t * buffer, uint16_t size)
{
  /* Check that Tx memory free size is enough */
  if (w5100_spi_read_short(S0_TX_FSR0) < 1024) return -EW5100_TXMEM;

  /* Get W5100 internal write pointer */
  uint16_t w5100_s0_tx_wr_ptr;
  w5100_s0_tx_wr_ptr = w5100_spi_read_short(S0_TX_WR0);

  /* Convert to physical offset (from S0_TX_BASE) */
  uint16_t tx_wr_offset;
  tx_wr_offset = w5100_s0_tx_wr_ptr & S0_TX_MASK;

  /* Write buffer contents */
  uint16_t tx_wr_ptr;
  for (uint16_t i = 0; i < size; i++) {
    tx_wr_ptr = S0_TX_BASE + ((tx_wr_offset+i) & S0_TX_MASK);
    w5100_spi_write(tx_wr_ptr, buffer[i]);
  }

  /* Update Tx offset */
  w5100_s0_tx_wr_ptr += size;
  w5100_spi_write_short(S0_TX_WR0, w5100_s0_tx_wr_ptr);

  /* Issue send command */
  w5100_spi_write(S0_CR, SN_CR_SEND);

  /* Wait for command to complete */
  while (w5100_spi_read(S0_CR));

  return 0;
}
