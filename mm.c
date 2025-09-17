struct freepage
{
    struct freepage *next;
};

struct freepage *freelist;

void *kalloc()
{
    struct freepage *p;
    if ((p = freelist))
        freelist = p->next;
    return p;
}

void kfree(void *v)
{
    struct freepage *p;
    p = (struct freepage *)v;
    p->next = freelist;
    freelist = p;
}

void freerange(void *vstart, void *vend)
{
    char (*p)[4096];
    for (p = vstart; (void*)p < vend; p++)
        kfree(p);
}

void mminit1(void *vstart, void *vend)
{
    freerange(vstart, vend);
}
