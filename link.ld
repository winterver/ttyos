ENTRY(_start)

SECTIONS
{
    . = 0x80100000;             /* '. =' sets virtual address */
    .text BLOCK(4K) : AT(1M)    /* AT() sets physical address */
    {
        *(.text .text.*)
    }

    .rodata BLOCK(4K) : ALIGN(4K)
    {
        *(.rodata .rodata.*)
    }

    PROVIDE(data = ALIGN(4K));

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
