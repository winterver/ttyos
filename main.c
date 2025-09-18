#include <layout.h>
#include <types.h>
#include <printk.h>

extern int kend[];

void uartinit();
void mminit1(void *vstart, void *vend);

int main(void)
{
    uartinit();
    printk("Hello ttyOS!\n");
    mminit1(kend, (void*)(KVADDR + 0x400000));
}
