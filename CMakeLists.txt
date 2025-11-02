cmake_minimum_required(VERSION 3.31)
project(ttyos C ASM)

set(TARGET "i686-elf")
set(CMAKE_SYSTEM_NAME "Generic")
set(CMAKE_SYSROOT "/home/winterver/opt/${TARGET}")
set(CMAKE_C_COMPILER "${CMAKE_SYSROOT}/bin/${TARGET}-gcc")
set(CMAKE_ASM_COMPILER "${CMAKE_SYSROOT}/bin/${TARGET}-gcc")

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

target_include_directories(ttyos PRIVATE .)
target_link_directories(ttyos PRIVATE .)
target_compile_options(ttyos PRIVATE -ffreestanding -Wall -Wextra -Werror -g)
target_link_options(ttyos PRIVATE -Tlink.ld -nostdlib)
