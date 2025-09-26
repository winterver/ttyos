#include <types.h>
#include <printk.h>

#define IOAPIC      0xfec00000
#define ID          0x00
#define VER         0x01
#define TBL         0x10

#define DISABLED    0x00010000
#define LEVEL       0x00008000
#define ACTIVELOW   0x00002000
#define LOGICAL     0x00000800

#define IRQ0        32

struct ioapic {
    u32 reg;
    u32 pad[3];
    u32 data;
};

volatile
struct ioapic *ioapic;
u8 ioapicid;

static u32 ioapicread(int reg)
{
    ioapic->reg = reg;
    return ioapic->data;
}

static void ioapicwrite(int reg, u32 data)
{
    ioapic->reg = reg;
    ioapic->data = data;
}

void ioapicinit()
{
    int i, id, max;

    ioapic = (struct ioapic*)IOAPIC;
    id = ioapicread(ID) >> 24;
    max = (ioapicread(VER) >> 16) & 0xff;

    if (id != ioapicid)
        panic("ioapicid unequal\n");

    for (i = 0; i <= max; i++) {
        ioapicwrite(TBL+2*i, DISABLED | (IRQ0 + i));
        ioapicwrite(TBL+2*i+1, 0);
    }
}
