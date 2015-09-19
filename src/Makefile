obj = spi.o w5100.o main.o
prj = udp_beacon

mcu = atmega328p

override CFLAGS += -Wall -std=c99 -mmcu=$(mcu)
VPATH = spi:w5100
CC = avr-gcc
OBJCOPY = avr-objcopy

all: $(prj).elf $(prj).hex

$(prj).elf: $(obj)
	$(CC) $(CFLAGS) $^ -o $@

%.hex: %.elf
	$(OBJCOPY) -j .text -j .data -O ihex $< $@

clean:
	rm -f *.o

.PHONY: clean
