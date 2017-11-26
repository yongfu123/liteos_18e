#include "hi_osal.h"
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/spinlock.h>
#include <linux/slab.h>

int osal_spin_lock_init(osal_spinlock_t* lock)
{
    spinlock_t* p = NULL;
    if (lock == NULL)
    {
        osal_printk("%s - parameter invalid!\n", __FUNCTION__);
        return -1;
    }
    p = (spinlock_t*)kmalloc(sizeof(spinlock_t), GFP_KERNEL);
    if (p == NULL)
    {
        osal_printk("%s - kmalloc error!\n", __FUNCTION__);
        return -1;
    }
    spin_lock_init(p);
    lock->lock = p;
    return 0;
}
void osal_spin_lock(osal_spinlock_t* lock)
{
    spinlock_t* p = NULL;

    p = (spinlock_t*)(lock->lock);
    spin_lock(p);
}
int osal_spin_trylock(osal_spinlock_t* lock)
{
    /* not support yet in liteos */
    return -1;
}
void osal_spin_unlock(osal_spinlock_t* lock)
{
    spinlock_t* p = NULL;

    p = (spinlock_t*)(lock->lock);
    spin_unlock(p);
}
void osal_spin_lock_irqsave(osal_spinlock_t* lock, unsigned long* flags)
{
    spinlock_t* p = NULL;
    unsigned long f;

    p = (spinlock_t*)(lock->lock);
    spin_lock_irqsave(p, f);
    *flags = f;
}
int osal_spin_trylock_irqsave(osal_spinlock_t* lock, unsigned long* flags)
{
    /* not support yet in liteos */
    return -1;
}
void osal_spin_unlock_irqrestore(osal_spinlock_t* lock, unsigned long* flags)
{
    spinlock_t* p = NULL;
    unsigned long f;

    p = (spinlock_t*)(lock->lock);
    f = *flags;
    spin_unlock_irqrestore(p, f);
}
void osal_spin_lock_destory(osal_spinlock_t* lock)
{
    spinlock_t* p = NULL;
    p = (spinlock_t*)(lock->lock);
    kfree(p);
    lock->lock = NULL;
}
