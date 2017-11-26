#include "hi_osal.h"
#include <linux/module.h>
#include <linux/kernel.h>
#include <asm/cacheflush.h>
#include <linux/dma-direction.h>
#include "osal_mmz.h"

void osal_flush_cache_all(void)
{
    /* not support yet in liteos */
    return;
}

void osal_cpuc_flush_dcache_area(void* addr, int size)
{
    //dma_cache_clean(addr, addr + size);
    //dma_cache_inv((int)addr, addr + size);
}

void osal_dma_map_area(void* addr, int size, int dir)
{
    //dma_cache_clean(addr, addr + size);
    //dma_cache_inv(addr, addr + size);
}

void osal_flush_dcache_area(void* kvirt, unsigned long phys_addr, unsigned long length)
{
    hil_mmb_flush_dcache_byaddr(kvirt,phys_addr,length);
}

void osal_flush_l2cache_area(void* kvirt, unsigned long phys_addr, unsigned long length)
{
    /* not support yet in liteos */
    return;
}

int osal_flush_dcache_all(void)
{
    /* not support yet in liteos */

    return 0;
}

