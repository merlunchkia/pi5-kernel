CROSS   = aarch64-none-elf
CC      = $(CROSS)-gcc
LD      = $(CROSS)-ld
OBJCOPY = $(CROSS)-objcopy
RM      = del /f /q

CFLAGS  = -Wall -Wextra -O2 -ffreestanding -nostdlib -nostartfiles -Iinclude
LDFLAGS = -T linker.ld

OBJS = boot.o kernel.o uart.o

all: kernel8.img

boot.o: boot.S
	$(CC) $(CFLAGS) -c boot.S -o boot.o

kernel.o: kernel.c
	$(CC) $(CFLAGS) -c kernel.c -o kernel.o

uart.o: uart.c
	$(CC) $(CFLAGS) -c uart.c -o uart.o

kernel.elf: $(OBJS) linker.ld
	$(LD) $(LDFLAGS) $(OBJS) -o kernel.elf

kernel8.img: kernel.elf
	$(OBJCOPY) kernel.elf -O binary kernel8.img

clean:
	$(RM) *.o *.elf *.img
