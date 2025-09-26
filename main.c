#include <printk.h>
#include <x86asm.h>

void uartinit();
void mminit1();
void vminit();
void mpinit();
void lapicinit();
void seginit();
void shut8259a();
void ioapicinit();

int main(void)
{
    uartinit();     // init serial ports
    mminit1();      // physical page allocator
    vminit();       // virtual memory manager
    mpinit();       // multi-core support
    lapicinit();
    seginit();
    shut8259a();
    ioapicinit();

    printk("Hello ttyOS!\n");
}

void shut8259a()
{
    const u16 iopic1 = 0x20;
    const u16 iopic2 = 0xa0;
    outb(iopic1+1, 0xff);
    outb(iopic2+1, 0xff);
}
