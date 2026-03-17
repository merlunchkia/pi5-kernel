CROSS   = aarch64-none-elf
CC      = $(CROSS)-gcc
LD      = $(CROSS)-ld
OBJCOPY = $(CROSS)-objcopy

CFLAGS  = -Wall -Wextra -O2 -ffreestanding -nostdlib -nostartfiles
LDFLAGS = -T linker.ld

all: kernel8.img

boot.o: boot.S
	$(CC) $(CFLAGS) -c boot.S -o boot.o

kernel.o: kernel.c
	$(CC) $(CFLAGS) -c kernel.c -o kernel.o

kernel.elf: boot.o kernel.o linker.ld
	$(LD) $(LDFLAGS) boot.o kernel.o -o kernel.elf

kernel8.img: kernel.elf
	$(OBJCOPY) kernel.elf -O binary kernel8.img

clean:
	rm -f *.o *.elf *.img