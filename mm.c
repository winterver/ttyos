#include <layout.h>
#include <printk.h>
#include <types.h>

static unsigned long npages;
static u32 *pgstack;

u32 palloc()
{
    if (npages == 0)
        panic("out of physical pages\n");

    return pgstack[--npages];
}

void pfree(u32 p)
{
    pgstack[npages++] = p;
}

void pfreerange(u32 pstart, u32 pend)
{
    u32 p;
    for (p = pstart; p < pend; p += 4096)
        pfree(p);
}

void mminit(u32 pstart, u32 pend)
{
    pgstack = (u32*)kend;
    pfreerange(pstart, pend);
}
