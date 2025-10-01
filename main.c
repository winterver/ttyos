#include <printk.h>
#include <x86asm.h>

void uartinit();
void vminit();
void mpinit();
void lapicinit();
void seginit();
void shut8259a();
void ioapicinit();

void *kr_malloc(unsigned);

int main(void)
{
    uartinit();     // init serial ports
    vminit();       // memory manager
    mpinit();       // multi-core support
    lapicinit();
    seginit();
    shut8259a();
    ioapicinit();

    printk("kr_malloc(8) = %p\n", kr_malloc(8));
    printk("Hello ttyOS!\n");
}

void shut8259a()
{
    const u16 iopic1 = 0x20;
    const u16 iopic2 = 0xa0;
    outb(iopic1+1, 0xff);
    outb(iopic2+1, 0xff);
}
