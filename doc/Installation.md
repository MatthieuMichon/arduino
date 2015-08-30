This document describes the installation of the dev tools under Linux Fedora.

# Toolchain

```
$ sudo yum update
$ sudo yum install gcc-avr32-linux-gnu.x86_64 avr-libc avrdude.x86_64
```

# Usage

An example Makefile can found at ``/usr/share/doc/avr-libc/examples/demo/Makefile``.

Using ``avrdude``, the speed of the serial link must be altered to 57600:

```
$ sudo avrdude -v -p atmega328p -c arduino -P /dev/ttyUSB0 -b 57600 -D -U flash:w:l_led.hex:i
```
