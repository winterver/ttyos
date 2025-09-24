#include <printk.h>

void uartinit();
void mminit1();
void kvminit();
void mpinit();
void lapicinit();

int main(void)
{
    uartinit();
    mminit1(); // physical page allocator
    kvminit(); // kernel virtual memory
    mpinit();
    lapicinit();

    printk("Hello ttyOS!\n");
}
