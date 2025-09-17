#include <layout.h>
#include <types.h>
#include <printk.h>

ALIGNED(0x1000) u32 pagedir[1024] = {
    [0] = 0x000000 | 0x80 | 0x2 | 0x1, // identity mapping for 0-4m
    [1] = 0x400000 | 0x80 | 0x2 | 0x1, // identity mapping for 4-8m
    [KVADDR>>22] = 0x400000 | 0x80 | 0x2 | 0x1, // map first 4m of kernel to 4-8m
};

extern int kend[];

void uartinit();
void mminit1(void *vstart, void *vend);

int main(void)
{
    uartinit();
    printk("Hello ttyOS!\n");
    mminit1(kend, (void*)(KVADDR + 0x400000));
}
