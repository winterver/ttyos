#include <x86asm.h>

#define PORT 0x3f8

void uartputc(char a)
{
    if (a == '\n')
        uartputc('\r');
    while ((inb(PORT + 5) & 0x20) == 0);
    outb(PORT, a);
}

void uartinit()
{
    outb(PORT + 1, 0x00);
    outb(PORT + 3, 0x80);
    outb(PORT + 0, 0x03);
    outb(PORT + 1, 0x00);
    outb(PORT + 3, 0x03);
    outb(PORT + 2, 0xC7);
    outb(PORT + 4, 0x0B);
    outb(PORT + 4, 0x0F);

    uartputc('\n');
}
