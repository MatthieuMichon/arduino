#ifndef _W5100_REGS_H_
#define _W5100_REGS_H_


#define MR 0x0000
#define RST 7

#define GAR0 0x0001
#define GAR1 GAR0+1
#define GAR2 GAR0+2
#define GAR3 GAR0+3

#define SUBR0 0x0005
#define SUBR1 SUBR0+1
#define SUBR2 SUBR0+2
#define SUBR3 SUBR0+3

#define SHAR0 0x0009
#define SHAR1 SHAR0+1
#define SHAR2 SHAR0+2
#define SHAR3 SHAR0+3
#define SHAR4 SHAR0+4
#define SHAR5 SHAR0+5

#define SIPR0 0x000F
#define SIPR1 SIPR0+1
#define SIPR2 SIPR0+2
#define SIPR3 SIPR0+3

#define IR 0x0015

#define RMSR 0x001A

#define TMSR 0x001B

#define S0_MR 0x0400
#define MULTI 7
#define MC 5
#define P3 3
#define P2 2
#define P1 1
#define P0 0
#define UDP (1 << P1)

#define S0_CR 0x0401
#define SN_CR_CLEAR 0x00
#define SN_CR_OPEN 0x01
#define SN_CR_CLOSE 0x10
#define SN_CR_SEND 0x21

#define S0_SR 0x0403
#define SOCK_UDP 0x22

#define S0_PORT0 0x0404
#define S0_PORT1 S0_PORT0+1

#define S0_DIPR0 0x040C
#define S0_DIPR1 S0_DIPR0+1
#define S0_DIPR2 S0_DIPR0+2
#define S0_DIPR3 S0_DIPR0+3

#define S0_DPORT0 0x0410
#define S0_DPORT1 S0_DPORT0+1

#define S0_TX_FSR0 0x0420
#define S0_TX_FSR1 S0_TX_FSR0+1

#define S0_TX_WR0 0x0424
#define S0_TX_WR1 S0_TX_WR0+1

#define S0_TX_BASE 0x4000

#define S0_TX_MASK 0x07FF

#endif //_W5100_REGS_H_
