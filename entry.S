#include "layout.h"

    .align 4
    .long 0x1BADB002
    .long 0x00000000
    .long -0x1BADB002

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
    movl $(0 | 0x80 | 0x2 | 0x1), (%eax) // identity mapping for 0-4m

    // map [VBASE, 4G] to [0, 4G - VBASE]
    addl $((VBASE>>22)*4), %eax
    movl $(0 | 0x80 | 0x2 | 0x1), %ebx
    movl $(1024-(VBASE>>22)), %ecx
.L0:
    movl %ebx, (%eax)
    addl $0x400000, %ebx
    addl $4, %eax
    loop .L0

    movl $ASM_V2P(entrypgdir), %eax
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
