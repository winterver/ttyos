#ifndef __PROC_H__
#define __PROC_H__

#include <types.h>

#define MAXCPU 16

struct cpu {
    u8 apicid;
};

extern struct cpu cpus[MAXCPU];
extern int ncpu;

#endif
