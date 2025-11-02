KERNEL	= build/ttyos
SMP 	= 8

all: $(KERNEL)

$(KERNEL): build
	cmake --build build

build: CMakeLists.txt
	cmake -Bbuild

run: $(KERNEL)
	qemu-system-i386 -m 256 -smp $(SMP) -nographic -kernel $(KERNEL)
	@killall -q qemu-system-i386 || true

gdb: $(KERNEL)
	qemu-system-i386 -m 256 -smp $(SMP) -nographic -kernel $(KERNEL) -s -S &
	cat <(printf "target remote localhost:1234\nfile $(KERNEL)\n") /dev/stdin | i686-elf-gdb
	@killall -q qemu-system-i386 || true

clean:
	make -C build clean
