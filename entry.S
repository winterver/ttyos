#include "layout.h"

.macro spin
    cli
1:  hlt
    jmp 1b
.endm

    .align 4
    .long 0x1BADB002, 0, -0x1BADB002

    .align 16       # together with 'ENTRY(_start)' in
    .global _start  # linker.ld, this sets the entry
_start = V2P(entry) # address of the kernel to KPADDR

    .align 16
    .global entry
entry:
    movl %cr4, %eax
    orl $0x10, %eax # enable 4MB large page
    movl %eax, %cr4

    movl $V2P(pagedir), %eax # pagedir is statically initialized in main.c
    movl %eax, %cr3

    movl %cr0, %eax
    orl $0x80010000, %eax # enable paging with write protect
    movl %eax, %cr0

    movl $(stack+KSSIZE), %esp
    movl $main, %eax
    call *%eax
    spin

    .align 16
    .lcomm stack, KSSIZE
