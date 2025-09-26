#include <printk.h>

void uartinit();
void mminit1();
void kvminit();
void mpinit();
void lapicinit();
void seginit();

int main(void)
{
    uartinit();
    mminit1(); // physical page allocator
    kvminit(); // kernel virtual memory
    mpinit();
    lapicinit();
    seginit();

    printk("Hello ttyOS!\n");
}
