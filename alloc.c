#include <types.h>

static u32 alloc_start;
static u32 alloc_current;
static u32 alloc_end;

void allocinit(u32 vstart, u32 vend)
{
    alloc_start = vstart;
    alloc_current = vstart;
    alloc_end = vend;
}

static char *sbrk(int len)
{
    return (char*)-1;
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
void kr_free(void *ap)
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
    if (cp == (char *) -1) /* no space at all */
        return nullptr;

    up = (header_t *)cp;
    up->size = nunits;
    kr_free((void *)(up+1));

    return free;
}

/* malloc: general-purpose storage allocator */
void *kr_malloc(unsigned nbytes)
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
