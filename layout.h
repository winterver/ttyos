#ifndef __LAYOUT_H__
#define __LAYOUT_H__

#define PHYSTOP     0xE000000   /* top of physical memory, 224MB. though
                                 * "-m 256" is specified to qemu, the last
                                 * 32MB are reserved for DEVSPACE */
#define KPADDR      0x400000    /* physical address into which the kernel will be loaded by grub */
#define KVADDR      0x80000000  /* virtual address of the kernel */
#define KSSIZE      0x4000      /* kernel stack size */
#define IOSPACE     0           /* I/O space is in low mem */
#define IOLENGTH    0x100000
#define DEVSPACE    0xFE000000  /* memory map is not continuous */
#define DEVLENGTH   0x2000000

#define V2P(x) ((u32)(x) - KVADDR + KPADDR)
#define P2V(x) ((void*)((char*)(x) + KVADDR - KPADDR))

#define ASM_V2P(x) ((x) - KVADDR + KPADDR)
#define ASM_P2V(x) ((x) + KVADDR - KPADDR)

#endif
