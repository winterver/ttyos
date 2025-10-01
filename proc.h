#ifndef __PROC_H__
#define __PROC_H__

#include <types.h>

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
