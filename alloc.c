#include <types.h>
#include <alloc.h>
#include <string.h>
#include <printk.h>

static u32 alloc_start;
static u32 alloc_current;
static u32 alloc_pfn; // alloc_current aligned down 4096 boundary
static u32 alloc_end;

void allocinit(u32 vstart, u32 vend)
{
    alloc_start = vstart;
    alloc_current = vstart;
    alloc_pfn = vstart;
    alloc_end = vend;
}

static void *kr_malloc(unsigned);
static void kr_free(void*);

void *kzalloc(size_t n)
{
    return memset(kr_malloc(n), 0, n);
}

void kfree(void *p)
{
    kr_free(p);
}

// K&R malloc implementation

void mappage(void *virt, u32 phys, int perm);
u32 palloc();

static char *sbrk(int len)
{
    u32 new_current;
    u32 new_pfn;
    u32 ret;

    if (len == 0)
        return (char*)alloc_current;

    if (len < 0)
        panic("sbrk() shrink not supported\n");

    new_current = alloc_current + len;
    new_pfn = new_current & ~4095;

    if (alloc_start == alloc_current)
        mappage((void*)alloc_start, palloc(), 3);

    for (; (alloc_pfn + 4096) <= new_pfn; alloc_pfn += 4096)
        mappage((void*)(alloc_pfn + 4096), palloc(), 3);

    ret = alloc_current;
    alloc_current = new_current;
    alloc_pfn = new_pfn;

    return (char*)ret;
}

typedef union header {      /* block header: */
    struct {
        union header *ptr;  /* next block if on free list */
        unsigned size;      /* size of this block */
    };
    u64 _align;             /* force alignment of blocks */
} header_t;

static header_t base;       /* empty list to get started */
static header_t *free;      /* start of free list */

#define NALLOC   1024       /* minimum #units to request */

/* free:   put block ap in free list */
static void kr_free(void *ap)
{
    header_t *bp, *p;

    bp = (header_t *)ap - 1; /* point to block header */
    for (p = free; !(bp > p && bp < p->ptr); p = p->ptr)
        if (p >= p->ptr && (bp > p || bp < p->ptr))
            break; /* freed block at start of end of arena */

    if (bp + bp->size == p->ptr) { /* join to upper nbr */
        bp->size += p->ptr->size;
        bp->ptr = p->ptr->ptr;
    } else
        bp->ptr = p->ptr;

    if (p + p->size == bp) { /* join to lower nbr */
        p->size += bp->size;
        p->ptr = bp->ptr;
    } else
        p->ptr = bp;

    free = p;
}

/* moreunits: ask system for more memory */
static header_t *moreunits(unsigned nunits)
{
    char *cp;
    header_t *up;

    if (nunits < NALLOC)
        nunits = NALLOC;

    cp = sbrk(nunits * sizeof(header_t));
    if (cp == (char *)-1) /* no space at all */
        return nullptr;

    up = (header_t *)cp;
    up->size = nunits;
    kr_free((void *)(up+1));

    return free;
}

/* malloc: general-purpose storage allocator */
static void *kr_malloc(unsigned nbytes)
{
    header_t *p, *prev;
    unsigned nunits;

    nunits = (nbytes+sizeof(header_t)-1) / sizeof(header_t) + 1;

    if ((prev = free) == nullptr) { /* no free list yet */
        free = prev = &base;
        base.ptr = &base;
        base.size = 0;
    }

    for (p = prev->ptr; ; prev = p, p = p->ptr) {
        if (p->size >= nunits) { /* big enough */
            if (p->size == nunits) /* exactly */
                prev->ptr = p->ptr;
            else { /* allocate tail end */
                p->size -= nunits;
                p += p->size;
                p->size = nunits;
            }

            free = prev;
            return (void *)(p+1);
        }

        if (p == free) /* wrapped around free list */
            if ((p = moreunits(nunits)) == nullptr)
                return nullptr; /* none left */
    }
}
