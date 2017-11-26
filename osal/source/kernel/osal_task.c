#include "hi_osal.h"
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/slab.h>

osal_task_t* osal_kthread_create(threadfn_t thread, void* data, char* name)
{
    /* not support yet in liteos */
    return NULL;
}
void osal_kthread_destory(osal_task_t* task)
{
    /* not support yet in liteos */
    return;
}
void osal_kthread_bind(osal_task_t* task, unsigned int cpu)
{
    /* not support yet in liteos */
    return;
}
