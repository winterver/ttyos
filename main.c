#include <printk.h>

void uartinit();
void mminit1();
void kvminit();

int main(void)
{
    uartinit();
    mminit1(); // physical page allocator
    kvminit(); // kernel virtual memory

    printk("Hello ttyOS!\n");
}
