#ifndef __SPINLOCK_H__
#define __SPINLOCK_H__

struct spinlock
{
    int locked;
};

void lockacquire(struct spinlock *lock);
void lockrelease(struct spinlock *lock);

#endif
