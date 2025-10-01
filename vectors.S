#include "segment.h"

    .section .text
    .global trapenter
    .global trapret
trapenter:
    pushl %ds
    pushl %es
    pushl %fs
    pushl %gs
    pushal

    movw $(SI_KDATA<<3), %ax
    movw %ax, %ds
    movw %ax, %es

    pushl %esp
    call trap
    addl $4, %esp
trapret:
    popal
    popl %gs
    popl %fs
    popl %es
    popl %ds
    addl $8, %esp
    iret

    .section .data
    .global vectors
vectors:
    .rept 256
    .long vector\+
    .endr

    .section .text
    # 0-17 with errcode
    .irp i, 8, 10, 11, 12, 13, 14, 17
vector\i:
    pushl $\i
    jmp trapenter
    .endr

    # 0-17 without errcode
    .irp i, 0, 1, 2, 3, 4, 5, 6, 7, 9, 15, 16
vector\i:
    pushl $0
    pushl $\i
    jmp trapenter
    .endr

    .altmacro
    .macro _defvec i
vector\i:
    pushl $0
    pushl $\i
    jmp trapenter
    .endm

    .altmacro
    .macro defvec i
    _defvec %(i+18)
    .endm

    # 18-255 without errcode
    .rept 238
    defvec \+
    .endr
