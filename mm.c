#include <layout.h>
#include <printk.h>
#include <types.h>

struct ps {
    struct ps *next;
    int count;
    u32 free[1022];   
};

static struct ps initps;
static struct ps *current = &initps;

u32 virttophys(void *virt);
void mappage(void *virt, u32 phys, int perm);
void *kzalloc();

u32 palloc()
{
    struct ps *tmp;
    u32 p;

    if (current->count == 0) {
        tmp = current;
        current = current->next;
        p = virttophys(tmp);
        mappage(tmp, 0, 0);
    }
    else {
        p = current->free[--current->count];
    }

    return p;
}

void pfree(u32 p)
{
    struct ps *neo;

    if (current->count == 1022) {
        neo = kzalloc(); // can only alloc a whole page for now
        current->free[current->count++] = p; // count decreases by 1 after kzalloc()
        neo->next = current;
        current = neo;
    }
    else {
        current->free[current->count++] = p;
    }
}

void pfreerange(u32 pstart, u32 pend)
{
    u32 p;
    for (p = pstart; p < pend; p += 4096) {
        pfree(p);
    }
}

void mminit()
{
    pfreerange(V2P(kend), PHYSTOP);
}
