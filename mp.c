#include <layout.h>
#include <types.h>
#include <string.h>
#include <printk.h>

struct rsdp {
    char sig[8]; // "RSD PTR "
    u8 checksum;
    char oemid[6];
    u8 revision;
        #define ACPI_1_0 0
    u32 rsdt;
} PACKED;

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
        if (memcmp(p, "RSD PTR ", 8))
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

    if (!(rsdp = rsdpsearch()))
        panic("RSDP not found\n");

    if (rsdp->revision != ACPI_1_0)
        panic("ACPI 2.0 not supported\n");

    panic("rsdp = %p, rsdt = %p\n", rsdp, P2V(rsdp->rsdt));
}
