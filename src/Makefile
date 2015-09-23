obj = spi.o w5100.o main.o
prj = udp_beacon

mcu = atmega328p

override CFLAGS += -Wall -std=c99 -mmcu=$(mcu)
VPATH = spi:w5100
CC = avr-gcc
OBJCOPY = avr-objcopy
OBJDUMP = avr-objdump

all: $(prj).elf $(prj).lst $(prj).hex

$(prj).elf: $(obj)
	$(CC) $(CFLAGS) $^ -o $@

%.lst: %.elf
	$(OBJDUMP) -h -S $< > $@

%.hex: %.elf
	$(OBJCOPY) -j .text -j .data -O ihex $< $@

clean:
	rm -f $(obj)
	rm -f $(prj).elf $(prj).lst $(prj).hex

.PHONY: clean
