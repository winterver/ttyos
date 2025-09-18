#ifndef __STRING_H__
#define __STRING_H__

#include <stddef.h>

static inline
void *memset(void *dest, int c, size_t n)
{
    while (n--)
       ((char*)dest)[n] = c;
    return dest;
}

static inline
void *memcpy(void *dest, const void *src, size_t n)
{
     while (n--)
       ((char*)dest)[n] = ((const char*)src)[n];
     return dest;
}

#endif
