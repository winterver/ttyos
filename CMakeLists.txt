cmake_minimum_required(VERSION 3.31)
project(ttyos C ASM)

set(TARGET "i686-elf")
set(CMAKE_SYSTEM_NAME "Generic")
set(CMAKE_SYSROOT "/home/winterver/opt/${TARGET}")
set(CMAKE_C_COMPILER "${CMAKE_SYSROOT}/bin/${TARGET}-gcc")
set(CMAKE_ASM_COMPILER "${CMAKE_SYSROOT}/bin/${TARGET}-gcc")

include_directories(.)
link_directories(.)
add_compile_options(-ffreestanding -Wall -Wextra -Werror -g)
add_link_options(-Tlink.ld -nostdlib)

add_executable(
    ttyos

    entry.S
    main.c
    uart.c
    mm.c
    vm.c
    alloc.c
    mp.c
    lapic.c
    proc.c
    ioapic.c
    trap.c
    vectors.S

    printk.c
    vsnprintf.c
    spinlock.c

    alloc.h
    layout.h
    printk.h
    proc.h
    segment.h
    spinlock.h
    string.h
    types.h
    x86asm.h

    link.ld
    build_toolchain.sh
)
