#include <layout.h>
#include <x86asm.h>
#include <string.h>
#include <types.h>
#include <mm.h>

#define PTE_P 0x001
#define PTE_W 0x002
#define PTE_U 0x004

#define PDI(a) (((u32)(a) >> 22) & 0x3FF)
#define PTI(a) (((u32)(a) >> 12) & 0x3FF)
#define PTE_ADDR(pte) ((pte) & ~0xFFF)

u32 *pagedir;

extern char data[];

static void mappage(void *virt, u32 phys, int perm)
{
    u32 *pde;
    u32 *pt;

    pde = &pagedir[PDI(virt)];

    if (*pde & PTE_P) {
        pt = P2V(PTE_ADDR(*pde));
    }
    else {
        pt = kalloc();
        memset(pt, 0, 4096);
        *pde = V2P(pt) | PTE_U | PTE_W | PTE_P;
    }

    pt[PTI(virt)] = phys | perm;
}

static void maprange(void *virt, u32 phys, size_t size, int perm)
{
    size_t i;

    for (i = 0; i < size; i += 4096)
        mappage((char*)virt + i, phys + i, perm);
}

void vminit()
{
    pagedir = kalloc();
    memset(pagedir, 0, 4096);

    maprange((void*)VBASE,      IOSPACE,    IOLENGTH,               PTE_W | PTE_P);
    maprange((void*)KVADDR,     KPADDR,     (u32)data - KVADDR,     PTE_P);
    maprange((void*)data,       V2P(data),  PHYSTOP - V2P(data),    PTE_W | PTE_P);
    maprange((void*)DEVSPACE,   DEVSPACE,   DEVLENGTH,              PTE_W | PTE_P);

    lcr3(V2P(pagedir));
}
