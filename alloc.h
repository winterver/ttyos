#ifndef __ALLOC_H__
#define __ALLOC_H__

#include <types.h>

void *kzalloc(size_t n);
void kfree(void *p);

#endif
