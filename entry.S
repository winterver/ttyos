#include "layout.h"

    .align 4
    .long 0x1BADB002, 0, -0x1BADB002

    .align 16           # together with 'ENTRY(_start)' in
    .global _start      # link.ld, this sets the entry
_start = ASM_V2P(entry) # address of the kernel to KPADDR

    .align 16
    .global entry
entry:
    movl %cr4, %eax
    orl $0x10, %eax # enable 4MB large page
    movl %eax, %cr4

    movl $ASM_V2P(entrypgdir), %eax
    movl $(0x000000 | 0x80 | 0x2 | 0x1), 0*4(%eax) // identity mapping for 0-4m
    movl $(0x400000 | 0x80 | 0x2 | 0x1), 1*4(%eax) // identity mapping for 4-8m
    movl $(0x400000 | 0x80 | 0x2 | 0x1), (KVADDR>>22)*4(%eax) // map first 4m of kernel to 4-8m
    movl %eax, %cr3

    movl %cr0, %eax
    orl $0x80010000, %eax # enable paging with write protect
    movl %eax, %cr0

    movl $(stack+KSSIZE), %esp
    movl $main, %eax
    pushl $spin
    jmp *%eax
spin:
    cli
    hlt
    jmp spin

    .section .bss
    .align 4096
    .global entrypgdir
entrypgdir:
    .skip 4096
stack:
    .skip KSSIZE
