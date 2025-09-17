#ifndef __TYPES_H__
#define __TYPES_H__

#if defined(__linux__) || !defined(__i386__)
# error "i686-elf toolchain required"
#endif

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#define PACKED __attribute__((packed))
#define ALIGNED(x) __attribute__((aligned(x)))

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

#endif
