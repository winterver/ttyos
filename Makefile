CC		= i686-elf-gcc
CFLAGS	= -ffreestanding -O2 -Wall -Wextra -I. -g
LDFLAGS	= -ffreestanding -O2 -nostdlib -lgcc
KERNEL	= ttyos
SMP 	= 8

OBJECTS = entry.o main.o uart.o mm.o vm.o alloc.o mp.o lapic.o proc.o ioapic.o	\
		  vectors.o trap.o \
		  spinlock.o printk.o vsnprintf.o

all: $(KERNEL)

$(KERNEL): link.ld $(OBJECTS)
	$(CC) $(LDFLAGS) -o $(KERNEL) -T link.ld $(OBJECTS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.S
	$(CC) $(CFLAGS) -c $< -o $@

run: $(KERNEL)
	qemu-system-i386 -m 256 -smp $(SMP) -nographic -kernel $(KERNEL)
	@killall -q qemu-system-i386 || true

gdb: $(KERNEL)
	qemu-system-i386 -m 256 -smp $(SMP) -nographic -kernel $(KERNEL) -s -S &
	cat <(printf "target remote localhost:1234\nfile $(KERNEL)\n") /dev/stdin | i686-elf-gdb
	@killall -q qemu-system-i386 || true

clean:
	rm -f *.o $(KERNEL)
