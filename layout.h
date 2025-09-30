#ifndef __LAYOUT_H__
#define __LAYOUT_H__

#define VBASE       0xC0000000  /* base of virtual address */
#define KPADDR      0x100000    /* physical address into which the kernel will be loaded by grub */
#define KVADDR      0xC0100000  /* virtual address of the kernel */
#define KSSIZE      0x4000      /* kernel stack size */

#define IOSPACE     0           /* I/O space is in low mem */
#define IOLENGTH    0x100000
#define DEVSPACE    0xFE000000
#define DEVLENGTH   0x2000000
#define PHYSTOP     0x10000000  /* 256 MB */

#define V2P(x) ((u32)(x) - VBASE)
#define P2V(x) ((void*)((char*)(x) + VBASE))

#define ASM_V2P(x) ((x) - VBASE)
#define ASM_P2V(x) ((x) + VBASE)

#endif
