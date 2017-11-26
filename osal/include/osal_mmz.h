#ifndef _OSAL_MMZ_H
#define _OSAL_MMZ_H

#include "hi_osal.h"

#define HIL_MMZ_NAME_LEN 32
#define	CACHE_LINE_SIZE (0x20)
#define HIL_MMZ_NAME_LEN 32
#define HIL_MMB_NAME_LEN 16

struct hil_media_memory_zone {
	char name[HIL_MMZ_NAME_LEN];

	unsigned long gfp;

	unsigned long phys_start;
	unsigned long nbytes;

	struct osal_list_head list;

	unsigned char *bitmap;
	struct osal_list_head mmb_list;
	
	unsigned int alloc_type;
	unsigned long block_align;

	void (*destructor)(const void *);
};
typedef struct hil_media_memory_zone hil_mmz_t;

#define HIL_MMZ_FMT_S "PHYS(0x%08lX, 0x%08lX), GFP=%lu, nBYTES=%luKB,	NAME=\"%s\""
#define hil_mmz_fmt_arg(p) (p)->phys_start,(p)->phys_start+(p)->nbytes-1,(p)->gfp,(p)->nbytes/SZ_1K,(p)->name

#define HIL_MMB_NAME_LEN 16
struct hil_media_memory_block {
	#ifndef MMZ_V2_SUPPORT
	unsigned int id;
	#endif
	char name[HIL_MMB_NAME_LEN];
	struct hil_media_memory_zone *zone;
	struct osal_list_head list;

	unsigned long phys_addr;
	void *kvirt;
	unsigned long length;

	unsigned long flags;
	
	unsigned int order;
	
	int phy_ref;
	int map_ref;
};
typedef struct hil_media_memory_block hil_mmb_t;

#define hil_mmb_kvirt(p)	({hil_mmb_t *__mmb=(p); OSAL_BUG_ON(__mmb==NULL); __mmb->kvirt;})
#define hil_mmb_phys(p)		({hil_mmb_t *__mmb=(p); OSAL_BUG_ON(__mmb==NULL); __mmb->phys_addr;})
#define hil_mmb_length(p)	({hil_mmb_t *__mmb=(p); OSAL_BUG_ON(__mmb==NULL); __mmb->length;})
#define hil_mmb_name(p)		({hil_mmb_t *__mmb=(p); OSAL_BUG_ON(__mmb==NULL); __mmb->name;})
#define hil_mmb_zone(p)		({hil_mmb_t *__mmb=(p); OSAL_BUG_ON(__mmb==NULL); __mmb->zone;})

#define HIL_MMB_MAP2KERN	(1<<0)
#define HIL_MMB_MAP2KERN_CACHED	(1<<1)
#define HIL_MMB_RELEASED	(1<<2)


#define HIL_MMB_FMT_S "phys(0x%08lX, 0x%08lX), kvirt=0x%p, flags=0x%08lX, length=%luKB,	name=\"%s\""
#define hil_mmb_fmt_arg(p) (p)->phys_addr,mmz_grain_align((p)->phys_addr+(p)->length)-1,(p)->kvirt,(p)->flags,(p)->length/SZ_1K,(p)->name


#define DEFAULT_ALLOC 0
#define SLAB_ALLOC 1
#define EQ_BLOCK_ALLOC 2

#define LOW_TO_HIGH 0
#define HIGH_TO_LOW 1

struct mmb_info {
	unsigned long phys_addr;	/* phys-memory address */
	unsigned long align;		/* if you need your phys-memory have special align size */
	unsigned long size;		/* length of memory you need, in bytes */
	unsigned int order;

	void *mapped;			/* userspace mapped ptr */

	union {
		struct {
			unsigned long prot  :8;	/* PROT_READ or PROT_WRITE */
			unsigned long flags :12;/* MAP_SHARED or MAP_PRIVATE */

#ifdef __KERNEL__
			unsigned long reserved :8; /* reserved, do not use */
			unsigned long delayed_free :1; 
			unsigned long map_cached :1; 
#endif
		};
		unsigned long w32_stuf;
	};

	char mmb_name[HIL_MMB_NAME_LEN];
	char mmz_name[HIL_MMZ_NAME_LEN];
	unsigned long gfp;		/* reserved, do set to 0 */

#ifdef __KERNEL__
	int map_ref;
	int mmb_ref;

	struct osal_list_head list;
	hil_mmb_t *mmb;
#endif
};

struct dirty_area {
	unsigned long dirty_phys_start;	/* dirty physical address */
	unsigned long dirty_virt_start; /* dirty virtual  address,
					   must be coherent with dirty_phys_addr */
	unsigned long dirty_size;
};
#define IOC_MMB_ALLOC		_IOWR('m', 10,  struct mmb_info)
#define IOC_MMB_ATTR		_IOR('m',  11,  struct mmb_info)
#define IOC_MMB_FREE		_IOW('m',  12,  struct mmb_info)
#define IOC_MMB_ALLOC_V2	_IOWR('m', 13,  struct mmb_info)

#define IOC_MMB_USER_REMAP	_IOWR('m', 20,  struct mmb_info)
#define IOC_MMB_USER_REMAP_CACHED _IOWR('m', 21,  struct mmb_info)
#define IOC_MMB_USER_UNMAP	_IOWR('m', 22,  struct mmb_info)

#define IOC_MMB_VIRT_GET_PHYS	_IOWR('m',  23,  struct mmb_info)

#define IOC_MMB_ADD_REF		_IO('r', 30)	/* ioctl(file, cmd, arg), arg is mmb_addr */
#define IOC_MMB_DEC_REF		_IO('r', 31)	/* ioctl(file, cmd, arg), arg is mmb_addr */

#define IOC_MMB_FLUSH_DCACHE	_IO('c', 40)

#define IOC_MMB_FLUSH_DCACHE_DIRTY		_IOW('d', 50, struct dirty_area)
#define IOC_MMB_TEST_CACHE	_IOW('t',  11,  struct mmb_info)


#define MMZ_SETUP_CMDLINE_LEN 256
typedef struct hiMMZ_MODULE_PARAMS_S
{
    char mmz[MMZ_SETUP_CMDLINE_LEN];
    int anony;
}MMZ_MODULE_PARAMS_S;

/*
 * APIs
 */
extern hil_mmb_t *hil_mmb_alloc(const char *name, unsigned long size, unsigned long align, 
		unsigned long gfp, const char *mmz_name);
extern int hil_mmb_free(hil_mmb_t *mmb);
extern hil_mmb_t *hil_mmb_getby_phys(unsigned long addr);
extern hil_mmb_t *hil_mmb_getby_kvirt(void *virt);
extern unsigned long usr_virt_to_phys(unsigned long virt);
#define hil_mmb_freeby_phys(phys_addr) hil_mmb_free(hil_mmb_getby_phys(phys_addr))
#define hil_mmb_freeby_kvirt(kvirt) hil_mmb_free(hil_mmb_getby_kvirt(kvirt))
extern void* hil_mmb_map2kern(hil_mmb_t *mmb);
extern void* hil_mmb_map2kern_cached(hil_mmb_t *mmb);
extern int hil_mmb_invalid_cache_byaddr(void *kvirt, unsigned long phys_addr, unsigned long length);
extern int hil_mmb_flush_dcache_byaddr(void *kvirt, unsigned long phys_addr, unsigned long length);
extern int hil_mmb_unmap(hil_mmb_t *mmb);

/* helper micros */
#endif
