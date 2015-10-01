# arduino

Projects relying on the Arduino single-board MCU

This goal is to broadcast information gathered from a smart electricity meter, more precisly a **Actaris AC14C5** issued by the french company ERDF operating the national low-voltage grid.

## Milestones

- [X] Initialize the MCU
- [X] Initialize the W5100 SPI <-> LAN interface IC
- [X] Send a single dummy broadcast UDP datagram
- [X] Continuously broadcast dummy UDP datagrams
- [X] Continuously broadcast UDP datagrams with a variable data (using ADCs as input data)
- [ ] Continuously broadcast UDP datagrams with a data structure TBD (CSV; JSON; YAML or whatever)
- [ ] Continuously broadcast UDP datagrams with a data structure TBD with data from UART connected to the electricity meter
