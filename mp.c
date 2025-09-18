#include <types.h>

struct mpconf;

struct mpfp {
  u8 sig[4];                // "_MP_"
  struct mpconf *physaddr;
  u8 length;                // 1
  u8 specrev;               // 14
  u8 checksum;
  u8 type;
  u8 imcrp;
  u8 reserved[3];
};

struct mpconf {
  u8 sig[4];                // "PCMP"
  u16 length;               // total length of the table
  u8 version;               // 14
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


static struct mpfp *_mpsearch(u32 a, int len)
{
}

static struct mpfp *mpsearch()
{
    struct mpfp *fp;
    u32 p1;
    u32 p2;

    p1 = (u32)(*(u16*)0x040e) << 4;
    p2 = (u32)(*(u16*)0x0413) * 1024;

    if ((fp = _mpsearch(p1 ?: p2-1024, 1024)))
        return fp;

    return _mpsearch(0xf0000, 0x10000);
}

void mpinit()
{
    struct mpfp *fp;
    fp = mpsearch();
}
