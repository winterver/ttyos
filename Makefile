CC		= i686-elf-gcc
CFLAGS	= -ffreestanding -O2 -Wall -Wextra -I.
LDFLAGS	= -ffreestanding -O2 -nostdlib -lgcc
KERNEL	= ttyos
SMP 	= 8

OBJECTS = entry.o main.o uart.o mm.o vm.o mp.o lapic.o proc.o spinlock.o printk.o vsnprintf.o

all: $(KERNEL)

$(KERNEL): link.ld $(OBJECTS)
	$(CC) $(LDFLAGS) -o $(KERNEL) -T link.ld $(OBJECTS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.S
	$(CC) -c $< -o $@

run: $(KERNEL)
	qemu-system-x86_64 -m 256 -smp $(SMP) -nographic -kernel $(KERNEL)
	@killall -q qemu-system-x86_64 || true

clean:
	rm -f *.o $(KERNEL)
