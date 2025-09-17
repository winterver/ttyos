#include <spinlock.h>

void lockacquire(struct spinlock *lock)
{
    asm volatile (
    "1:  lock btsl $0, %0   \n\t"
        "jnc 3f             \n\t"
    "2:  pause              \n\t"
        "testl $1, %0       \n\t"
        "jnz 2b             \n\t"
        "jmp 1b             \n\t"
    "3:                     \n\t"
        :"+m"(lock->locked));
}

void lockrelease(struct spinlock *lock)
{
    asm volatile ("movl $0, %0" : "=m"(lock->locked));
}
