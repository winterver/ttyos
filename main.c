#include <printk.h>
#include <string.h>
#include <x86asm.h>

void uartinit();
void vminit();
void mpinit();
void lapicinit();
void seginit();
void shut8259a();
void ioapicinit();

void *kr_malloc(unsigned);
void kr_free(void*);

void recursive(int n)
{
    if (n > 0) {
        void *p = kr_malloc(1000);
        printk("kr_malloc(1000) = %p\n", p);
        memset(p, 0xff, 1000);
        recursive(n-1);
        kr_free(p);
    }
}

int main(void)
{
    uartinit();     // init serial ports
    vminit();       // memory manager
    mpinit();       // multi-core support
    lapicinit();
    seginit();
    shut8259a();
    ioapicinit();

    recursive(500);
    recursive(500);

    printk("Hello ttyOS!\n");
}

void shut8259a()
{
    const u16 iopic1 = 0x20;
    const u16 iopic2 = 0xa0;
    outb(iopic1+1, 0xff);
    outb(iopic2+1, 0xff);
}
