ENTRY(_start)

SECTIONS
{
    . = 0x80000000;             /* '. =' sets virtual address */
    .text BLOCK(4K) : AT(4M)    /* AT() sets physical address */
    {
        *(.text .text.*)
    }

    .rodata BLOCK(4K) : ALIGN(4K)
    {
        *(.rodata .rodata.*)
    }

    .data BLOCK(4K) : ALIGN(4K)
    {
        *(.data .data.*)
    }

    .bss BLOCK(4K) : ALIGN(4K)
    {
        *(.bss)
        *(COMMON)
    }

    PROVIDE(kend = ALIGN(4K));
}
