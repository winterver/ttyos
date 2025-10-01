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

#define NTBL (1024-(VBASE>>22))

ALIGNED(0x1000) u32 pagedir[1024];
ALIGNED(0x1000) u32 pagetbl[NTBL][1024];

/*
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
*/

void mappage(void *virt, u32 phys, int perm)
{
    u32 *pde;
    u32 *pt;

    pde = &pagedir[PDI(virt)];

    if (*pde & PTE_P)
        pt = P2V(PTE_ADDR(*pde));
    else
        panic("attempt to map userspace\n");

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
    size_t i;
    for (i = 0; i < NTBL; i++)
        pagedir[i+1024-NTBL] = V2P(pagetbl[i]) | PTE_W | PTE_P;
}

void mminit(u32 pstart, u32 pend);
void allocinit(u32 vstart, u32 vend);
void pfreerange(u32 pstart, u32 pend);

void vminit()
{
    int size;

    tblinit();

    maprange((void*)VBASE,      IOSPACE,    IOLENGTH,   PTE_W | PTE_P);
    maprange((void*)DEVSPACE,   DEVSPACE,   DEVLENGTH,  PTE_W | PTE_P);

    // map text + rodata as readonly
    maprange((void*)KVADDR,     KPADDR,     (u32)kdata - KVADDR,    PTE_P);

    // map data + bss + Physical Page Stack(used in mm.c)
    size = (PHYSTOP - V2P(kend)) / 4096 * 4;
    size = (size + 4095) & ~4095; // align up 4096 boundary
    maprange((void*)kdata,      V2P(kdata), V2P(kend) - V2P(kdata) + size, PTE_W | PTE_P);

    // ACPI structures reside in the last MB of physical memory
    maprange(P2V(PHYSTOP-0x100000), PHYSTOP-0x100000, 0x100000, PTE_W | PTE_P);

    lcr3(V2P(pagedir));

    mminit(V2P(kend)+size, PHYSTOP-0x100000);
    allocinit((u32)kend+size, DEVSPACE);
}

void unmapacpi()
{
    maprange(P2V(PHYSTOP-0x100000), 0, 0x100000, 0);
    pfreerange(PHYSTOP-0x100000, PHYSTOP);
}
