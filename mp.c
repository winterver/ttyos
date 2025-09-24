#include <layout.h>
#include <types.h>
#include <string.h>
#include <printk.h>
#include <proc.h>

struct rsdp {
    char sig[8]; // "RSD PTR "
    u8 checksum;
    char oemid[6];
    u8 revision;
        #define ACPI_1_0 0
    u32 rsdt;
} PACKED;

struct sdt {
    char sig[4];
    u32 length;
    u8 revision;
    u8 checksum;
    char oemid[6];
    char oemtid[8];
    u32 oemrev;
    u32 creatid;
    u32 creatrev;
};

struct rsdt {
    struct sdt sdt; // sdt.sig = "RSDT"
    void *others[]; // nelem = (sdt.length - sizeof(sdt))/4
};

struct madt {
    struct sdt sdt; // sdt.sig = "APIC", not "ACPI" !
    u32 *lapic; // in last 32 MB of memory, which is identitily mapped.
    u32 flags;
};

struct proc {
    u8 type;
    u8 length;
    u8 acpiid;
    u8 apicid;
    u32 flags;
};

struct ioapic {
    u8 type;
    u8 length;
    u8 apicid;
    u8 unused;
    u32 *apicaddr; // identitily mapped
    u32 gsintbase;
};

#define RSDP_SIG "RSD PTR "
#define RSDT_SIG "RSDT"
#define MADT_SIG "APIC"
#define REC_PROC 0
#define REC_IOAPIC 1

extern volatile u32 *lapic;
u8 ioapicid;

static u8 checksum(void *addr, int len)
{
    int i;
    u8 sum;

    for (i = sum = 0; i < len; i++)
        sum += ((u8*)addr)[i];

    return sum;
}

static struct rsdp *_rsdpsearch(u8 *addr, int len)
{
    u8 *p;

    for (p = addr; p < addr+len; p += 16) {
        if (memcmp(p, RSDP_SIG, 8))
            continue;
        if (checksum(p, sizeof(struct rsdp)))
            continue;
        return (struct rsdp *)p;
    }

    return nullptr;
}

static struct rsdp *rsdpsearch()
{
    u8 *ebda = P2V(*(u16*)P2V(0x040e) << 4);
    u8 *bios = P2V(0xe0000);
    return _rsdpsearch(ebda, 1024)
        ?: _rsdpsearch(bios, 0x20000);
}

void mpinit()
{
    struct rsdp *rsdp;
    struct rsdt *rsdt;
    struct madt *madt;
    struct proc *proc;
    struct ioapic *io;
    int i, n;
    u8 *p, *e;

    if (!(rsdp = rsdpsearch()))
        panic("RSDP not found\n");

    if (rsdp->revision != ACPI_1_0)
        panic("ACPI 2.0 not supported\n");

    rsdt = P2V(rsdp->rsdt);
    if (checksum(rsdt, rsdt->sdt.length))
        panic("Invalid RSDT\n");

    n = (rsdt->sdt.length - sizeof(rsdt->sdt)) / 4;
    for (i = 0; i < n; i++) {
        p = P2V(rsdt->others[i]);
        if (memcmp(p, MADT_SIG, 4))
            continue;
        madt = (struct madt *)p;
        break;
    }

    if (checksum(madt, madt->sdt.length))
        panic("Invalid MADT\n");
    lapic = madt->lapic;

    p = (u8*)(madt+1);
    e = (u8*)madt + madt->sdt.length;

    for (; p < e; p += p[1]) {
        switch (*p) {
        case REC_PROC:
            proc = (struct proc *)p;
            if (ncpu < MAXCPU)
                cpus[ncpu++].apicid = proc->apicid;
            break;
        case REC_IOAPIC:
            io = (struct ioapic *)p;
            ioapicid = io->apicid;
            break;
        }
    }

    // ACPI systems have no IMCR.
    // Do nothing.
}
