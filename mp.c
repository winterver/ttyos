#include <layout.h>
#include <types.h>
#include <string.h>
#include <printk.h>

// struct mpstruct is under 1MB, while others are not

struct mpstruct {
  u8 sig[4];                // "_MP_"
  u32 physaddr;             // struct mpconf*, may be higher than 1MB!
  u8 length;                // 1
  u8 specrev;               // 1 or 4
  u8 checksum;              // used to make sure all bytes of this struct add up to zero
  u8 type;
  u8 imcrp;
  u8 reserved[3];
};

struct mpconf {
  u8 sig[4];                // "PCMP"
  u16 length;               // total length of the table
  u8 version;               // 1 or 4
  u8 checksum;
  u8 product[20];
  u32 *oemtable;
  u16 oemlength;
  u16 entry;
  u32 *lapicaddr;
  u16 xlength;
  u8 xchecksum;
  u8 reserved;
};

struct mpproc {
  u8 type;
  u8 apicid;
  u8 version;
  u8 flags;
    #define MPBOOT 0x02
  u8 sig[4];
  u32 feature;
  u8 reserved[8];
};

struct mpioapic {
  u8 type;
  u8 apicno;
  u8 version;
  u8 flags;
  u32 *addr;
};

#define MPPROC    0x00
#define MPBUS     0x01
#define MPIOAPIC  0x02
#define MPIOINTR  0x03
#define MPLINTR   0x04

static u8 mpchecksum(void *addr, int len)
{
    int i;
    u8 sum;

    for (i = sum = 0; i < len; i++)
        sum += ((u8*)addr)[i];

    return sum;
}

static struct mpstruct *_mpsearch(u32 addr, int len)
{
    struct mpstruct *p, *e;

    p = (struct mpstruct *)P2V(addr);
    e = (struct mpstruct *)P2V(addr+len);

    for (; p < e; p++) {
        if (memcmp(p->sig, "_MP_", 4) == 0 && mpchecksum(p, sizeof(struct mpstruct)) == 0)
            return p;
    }

    return nullptr;
}

static struct mpstruct *mpsearch()
{
    struct mpstruct *fp;
    u32 p1;
    u32 p2;

    p1 = (u32)(*(u16*)P2V(0x040e)) << 4;
    p2 = (u32)(*(u16*)P2V(0x0413)) << 10;

    if ((fp = _mpsearch(p1 ?: p2-1024, 1024)))
        return fp;

    return _mpsearch(0xf0000, 0x10000);
}

static struct mpconf *mpconfig(struct mpstruct **pfp)
{
    struct mpstruct *fp;
    struct mpconf *conf;

    if ((fp = mpsearch()) && fp->physaddr)
        conf = P2V(fp->physaddr);
    else
        return nullptr;

    panic("conf = 0x%lx, P2V(conf) = %p\n", fp->physaddr, conf);
    if (memcmp(conf->sig, "PCMP", 4) != 0)
        return nullptr;

    if (conf->version != 1 && conf->version != 4)
        return nullptr;

    if (mpchecksum(conf, conf->length) != 0)
        return nullptr;

    *pfp = fp;
    return conf;
}

void mpinit()
{
    struct mpstruct *fp;
    struct mpconf *conf;

    if ((conf = mpconfig(&fp)) == nullptr)
        panic("Expect to run on an SMP\n");

    printk("fp = %p, conf = %p\n", fp, conf);
}
