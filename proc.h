#ifndef __PROC_H__
#define __PROC_H__

#include <types.h>

#define SI_NONE    0
#define SI_KCODE   1
#define SI_KDATA   2
#define SI_UCODE   3
#define SI_UDATA   4
#define SI_TSS     5

#define SEG(access) (0x00cf01000000ffff | ((u64)(access)<<40))
#define SEG_NONE    0
#define SEG_KCODE   SEG(0x9a)
#define SEG_KDATA   SEG(0x92)
#define SEG_UCODE   SEG(0xfa)
#define SEG_UDATA   SEG(0xf2)

struct cpu {
    u8 apicid;
    u64 gdt[6];
};

#define MAXCPU     16

extern struct cpu cpus[MAXCPU];
extern int ncpu;

struct cpu *thiscpu();
int cpuid();

#endif
