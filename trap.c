#include <types.h>
#include <x86asm.h>
#include <printk.h>
#include <segment.h>

// Gate descriptors for interrupts and traps
struct gatedesc {
    u32 off_15_0 : 16;  // low 16 bits of offset in segment
    u32 cs : 16;        // code segment selector
    u32 args : 5;       // # args, 0 for interrupt/trap gates
    u32 rsv1 : 3;       // reserved(should be zero I guess)
    u32 type : 4;       // type(STS_{IG32,TG32})
    u32 s : 1;          // must be 0 (system)
    u32 dpl : 2;        // descriptor(meaning new) privilege level
    u32 p : 1;          // Present
    u32 off_31_16 : 16; // high bits of offset in segment
};

// Set up a normal interrupt/trap gate descriptor.
// - sel: Code segment selector for interrupt/trap handler
// - off: Offset in code segment for interrupt/trap handler
// - dpl: Descriptor Privilege Level -
//        the privilege level required for software to invoke
//        this interrupt/trap gate explicitly using an int instruction.
#define SETGATE(gate, gtype, sel, off, d)           \
{                                                   \
    (gate).off_15_0 = (u32)(off) & 0xffff;          \
    (gate).cs = (sel);                              \
    (gate).args = 0;                                \
    (gate).rsv1 = 0;                                \
    (gate).type = (gtype);                          \
    (gate).s = 0;                                   \
    (gate).dpl = (d);                               \
    (gate).p = 1;                                   \
    (gate).off_31_16 = (u32)(off) >> 16;            \
}

// interrupt gate clears FL_IF
#define GT_INTR 0xE
#define GT_TRAP 0xF
#define GT_TSS  0x9

#define DPL_KERN 0
#define DPL_USER 3

#define T_SYSCALL 0x80

struct gatedesc idt[256];
extern u32 vectors[256];

void tvinit()
{
    int i;

    for (i = 0; i < 256; i++)
        SETGATE(idt[i], GT_INTR, SI_KCODE<<3, vectors[i], DPL_KERN);
    SETGATE(idt[T_SYSCALL], GT_TRAP, SI_KCODE<<3, vectors[T_SYSCALL], DPL_USER);
}

void idtinit()
{
    lidt((u64*)idt, sizeof(idt));
}

struct trapframe {
    u32 edi;
    u32 esi;
    u32 ebp;
    u32 _esp;      // useless & ignored
    u32 ebx;
    u32 edx;
    u32 ecx;
    u32 eax;

    // segment registers are pushed as dwords
    u32 gs;
    u32 fs;
    u32 es;
    u32 ds;

    u32 trapno;

    u32 err;
    u32 eip;
    u32 cs;
    u32 eflags;

    // only when crossing rings
    u32 esp;
    u32 ss;
};

void trap(struct trapframe *tf)
{
    printk("eax: 0x%08lx ebx: 0x%08lx ecx: 0x%08lx edx: 0x%08lx\n", tf->eax, tf->ebx, tf->ecx, tf->edx);
    printk("esi: 0x%08lx edi: 0x%08lx ebp: 0x%08lx\n", tf->esi, tf->edi, tf->ebp);
    printk("ds: 0x%04lx es: 0x%04lx fs: 0x%04lx gs: 0x%04lx\n", tf->ds, tf->es, tf->fs, tf->gs);
    printk("eip: 0x%08lx eflags: 0x%08lx trapno: %ld err: %ld\n", tf->eip, tf->eflags, tf->trapno, tf->err);
    if (tf->cs != (SI_KCODE<<3)) {
        printk("cs: 0x%04lx ss: 0x%04lx esp: 0x%08lx\n", tf->cs, tf->ss, tf->esp);
    }
}
