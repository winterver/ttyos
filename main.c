#include <printk.h>
#include <x86asm.h>

void uartinit();
void vminit();
void mminit();
void mpinit();
void lapicinit();
void seginit();
void shut8259a();
void ioapicinit();

int main(void)
{
    uartinit();     // init serial ports
    vminit();       // virtual memory manager
    mminit();       // physical memory manager
    panic("mminit() successful\n");
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
