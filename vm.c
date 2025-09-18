#include <layout.h>
#include <types.h>
#include <mm.h>

#define PTE_P 0x001
#define PTE_W 0x002
#define PTE_U 0x004

#define PDI(a) (((u32)(a) >> 22) & 0x3FF)
#define PTI(a) (((u32)(a) >> 12) & 0x3FF)
#define PTE_ADDR(pte) ((u32)(pte) & ~0xFFF)
#define PTE_FLAGS(pte) ((u32)(pte) &  0xFFF)

u32 *kernpgdir;

static void mappage(void *virt, void *phys, int perm)
{
    u32 *pde;
    u32 *pt;

    pde = &kernpgdir[PDI(virt)];

    if (*pde & PTE_P) {
        pt = (u32*)P2V(PTE_ADDR(*pde));
    }
    else {
        pt = kalloc();
        memset(pt, 0, 4096);
        *pde = V2P(pt) | PTE_U | PTE_W | PTE_P;
    }

    pt[PTI(virt)] = (u32)virt | perm | PTE_P;
}

static void maprange(void *virt, void *phys, size_t size, int perm)
{
    int i;

    for (i = 0; i < size; i += 4096)
        mappage((char*)virt + i, (char*)phys + i, perm);
}

void kvminit()
{
    kernpgdir = kalloc();
    memset(kernpgdir, 0, 4096);
}


