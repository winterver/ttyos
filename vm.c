#include <layout.h>
#include <x86asm.h>
#include <string.h>
#include <types.h>
#include <printk.h>

#define PTE_P 0x001
#define PTE_W 0x002
#define PTE_U 0x004

#define PDI(a) (((u32)(a) >> 22) & 0x3FF)
#define PTI(a) (((u32)(a) >> 12) & 0x3FF)
#define PTE_ADDR(pte) ((pte) & ~0xFFF)

ALIGNED(0x1000) u32 pagedir[1024];
ALIGNED(0x1000) u32 pagetbl[256][1024];

u32 virttophys(void *virt)
{
    u32 *pde;
    u32 *pt;

    pde = &pagedir[PDI(virt)];

    if (*pde & PTE_P)
        pt = P2V(PTE_ADDR(*pde));
    else
        panic("page directory entry not present\n");

    if (!(pt[PTI(virt)] & PTE_P))
        panic("page table entry not present\n");

    return PTE_ADDR(pt[PTI(virt)]);
}

void mappage(void *virt, u32 phys, int perm)
{
    u32 *pde;
    u32 *pt;

    pde = &pagedir[PDI(virt)];

    if (*pde & PTE_P) {
        pt = P2V(PTE_ADDR(*pde));
    }
    else {
        //pt = kalloc();
        //memset(pt, 0, 4096);
        //*pde = V2P(pt) | PTE_U | PTE_W | PTE_P;
        panic("attempt to map userspace\n");
    }

    pt[PTI(virt)] = phys | perm;
}

static void maprange(void *virt, u32 phys, size_t size, int perm)
{
    size_t i;

    for (i = 0; i < size; i += 4096)
        mappage((char*)virt + i, phys + i, perm);
}

static void tblinit()
{
    int i;
    for (i = 0; i < 256; i++)
        pagedir[i+768] = V2P(pagetbl[i]) | PTE_W | PTE_P;
}

void vminit()
{
    tblinit();

    maprange((void*)VBASE,      IOSPACE,    IOLENGTH,               PTE_W | PTE_P);
    maprange((void*)KVADDR,     KPADDR,     (u32)kdata - KVADDR,    PTE_P);
    maprange((void*)kdata,      V2P(kdata), V2P(kend) - V2P(kdata), PTE_W | PTE_P);
    maprange((void*)DEVSPACE,   DEVSPACE,   DEVLENGTH,              PTE_W | PTE_P);

    // ACPI structures resides in the last MB of physical memory
    maprange((void*)(VBASE+PHYSTOP-0x100000), PHYSTOP-0x100000, 0x100000, PTE_W | PTE_P);

    lcr3(V2P(pagedir));
}

void unmapacpi()
{
    maprange((void*)(VBASE+PHYSTOP-0x100000), 0, 0x100000, 0);
}

static void *valloc(int pages)
{
    u32 vstart; // head of found virtual space
    int cont; // current npages of vstart
    int i, j;

    cont = 0;

    for (i = 0; i < 256; i++) {
        for (j = 0; j < 1024; j++) {
            if (!(pagetbl[i][j] & PTE_P)) {
                if (cont == 0)
                    vstart = VBASE + i * 0x400000 + j * 0x1000;
                cont++;
            }
            else {
                cont = 0;
            }

            if (cont == pages)
                return (void*)vstart;
        }
    }

    panic("out of virtual space\n");
}

u32 palloc();

void *kzalloc()
{
    void *v;
    u32 p;

    v = valloc(1);
    p = palloc();
    mappage(v, p, PTE_W | PTE_P);
    memset(v, 0, 4096);

    return v;
}
