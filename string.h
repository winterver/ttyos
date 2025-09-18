#ifndef __STRING_H__
#define __STRING_H__

#include <stddef.h>

static inline
void *memset(void *dest, int c, size_t n)
{
    while (n--)
       ((unsigned char*)dest)[n] = (unsigned char)c;
    return dest;
}

static inline
void *memcpy(void *dest, const void *src, size_t n)
{
     while (n--)
       ((char*)dest)[n] = ((const char*)src)[n];
     return dest;
}

static inline
int memcmp(const void *s1, const void *s2, size_t n)
{
    const unsigned char *p1 = s1;
    const unsigned char *p2 = s2;

    while (n--) {
        if (*p1 != *p2)
            return (int)*p1++ - (int)*p2++;
    }

    return 0;
}

#endif
