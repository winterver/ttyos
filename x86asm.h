#ifndef __X86ASM_H__
#define __X86ASM_H__

#include <types.h>

static inline
u8 inb(u16 port)
{
    u8 ret;
    asm volatile (
        "inb %%dx,%%al"
        :"=a"(ret)
        :"d"(port));
    return ret;
}

static inline
u16 inw(u16 port)
{
    u16 ret;
    asm volatile (
        "inw %%dx,%%ax"
        :"=a"(ret)
        :"d"(port));
    return ret;
}

static inline
u32 inl(u16 port)
{
    u32 ret;
    asm volatile (
        "inl %%dx,%%eax"
        :"=a"(ret)
        :"d"(port));
    return ret;
}

static inline
void outb(u16 port, u8 data)
{
    asm volatile (
        "outb %%al,%%dx"
        ::"d"(port)
         ,"a"(data));
}

static inline
void outw(u16 port, u16 data)
{
    asm volatile (
        "outw %%ax,%%dx"
        ::"d"(port)
         ,"a"(data));
}

static inline
void outl(u16 port, u32 data)
{
    asm volatile (
        "outl %%eax,%%dx"
        ::"d"(port)
         ,"a"(data));
}

static inline
u32 eflags()
{
  u32 ret;
  asm volatile ("pushfl;popl %%eax":"=a"(ret));
  return ret;
}

static inline
void lcr3(u32 pagedir)
{
    asm volatile ("movl %%eax, %%cr3"::"a"(pagedir));
}

static inline
void lgdt(u64 *p, size_t size)
{
    u16 pd[3];
    p[0] = size-1;
    p[1] = (u32)p;
    p[2] = (u32)p >> 16;

    asm volatile ("lgdt %0"::"m"(pd));
}

static inline
void lidt(u64 *p, size_t size)
{
    u16 pd[3];
    p[0] = size-1;
    p[1] = (u32)p;
    p[2] = (u32)p >> 16;

    asm volatile ("lidt %0"::"m"(pd));
}

#endif
