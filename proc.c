#include <proc.h>
#include <printk.h>
#include <x86asm.h>
#include <segment.h>

struct cpu cpus[MAXCPU];
int ncpu;

int lapicid();

// Must be called with interrupts disabled to avoid the caller being
// rescheduled between reading lapicid and running through the loop.
struct cpu *thiscpu()
{
    int apicid;
    int i;

    if (eflags() & 0x200)
        panic("thiscpu() called with interrupt enabled\n");

    apicid = lapicid();
    for (i = 0; i < ncpu; i++) {
        if (cpus[i].apicid == apicid)
            return &cpus[i];
    }

    panic("bad apicid for cpu\n");
}

// Must be called with interrupts disabled
int cpuid()
{
    return thiscpu() - cpus;
}

void seginit()
{
    struct cpu *c;
    c = thiscpu();
    c->gdt[SI_NONE] = 0;
    c->gdt[SI_KCODE] = SEG_KCODE;
    c->gdt[SI_KDATA] = SEG_KDATA;
    c->gdt[SI_UCODE] = SEG_UCODE;
    c->gdt[SI_UDATA] = SEG_UDATA;
    lgdt(c->gdt, sizeof(c->gdt));

    asm volatile (
        "movw %0, %%ax      \n\t"
        "movw %%ax, %%cs    \n\t"
        "movw %1, %%ax      \n\t"
        "movw %%ax, %%ss    \n\t"
        "movw %%ax, %%ds    \n\t"
        "movw %%ax, %%es    \n\t"
        "movw %%ax, %%fs    \n\t"
        "movw %%ax, %%gs    \n\t"
        ::"i"(SI_KCODE<<3),
          "i"(SI_KDATA<<3)
        :"ax");
}
