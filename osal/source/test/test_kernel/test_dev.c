#include "hi_osal.h"
#include "test_api.h"

static osal_dev_t* test_dev;

#define UMAP_GET_CHN(f)      (*((int*)(f)))
#define UMAP_SET_CHN(f, chn) (*((int*)(f)) = (chn))

osal_wait_t test_wait;
static int poll_ready = 0;
int condition = 0;
osal_wait_t st_osal_wait;

static int test_open(void* data)
{
    osal_printk("\nFunc:%s, Line:%d\n", __FUNCTION__, __LINE__);
    return 0;
}

static int test_release(void* data)
{
    osal_printk("\nFunc:%s, Line:%d\n", __FUNCTION__, __LINE__);
    return 0;
}

static long test_ioctl(unsigned int cmd, unsigned long arg, void* data)
{
    long ret = 0;
    int chn = UMAP_GET_CHN(data);
    switch (cmd)
    {
        case CMD_SETDATA:
            UMAP_SET_CHN(data, *((int *)arg));
            osal_printk("Func:%s, Line:%d\n", __FUNCTION__, __LINE__);
            chn = UMAP_GET_CHN(data);
            osal_printk("chn:%d\n", chn);
            break;
        case CMD_POLL:
            {
                int count = *(int*)arg;
                while(count--)
                {
                    ret = osal_msleep(1000);
                    osal_printk("--sleep 1s and wakeup--, count=%d\n", count);
                    poll_ready = 1;
                    osal_wakeup(&test_wait);
                }
            }
            break;
        case CMD_WAIT:
            {
                int ret3;

                osal_printk("--start wait timeout....--Func:%s, Line:%d\n", __FUNCTION__, __LINE__);
                ret3 = osal_wait_event_timeout(&st_osal_wait, condition, 10000);
                osal_printk("--wait timeout ret:%d--Func:%s, Line:%d\n", ret3, __FUNCTION__, __LINE__);
                if(ret3==0)
                {
                    osal_printk("--TIMEOUT--Func:%s, Line:%d\n", __FUNCTION__, __LINE__);
                }
                else if(ret3 > 0)
                {
                    osal_printk("--condition OK--Func:%s, Line:%d\n", __FUNCTION__, __LINE__);
                }
                else
                {
                    osal_printk("--ERROR--Func:%s, Line:%d\n", __FUNCTION__, __LINE__);
                    return ret3;
                }
                osal_printk("--wait timeout ok--Func:%s, Line:%d\n", __FUNCTION__, __LINE__);
                condition = 0;

                ret3 = osal_wait_event(&st_osal_wait, condition);
                if(ret3)
                {
                    osal_printk("--ERROR--Func:%s, Line:%d\n", __FUNCTION__, __LINE__);
                    return ret3;
                }
                osal_printk("--wait ok--Func:%s, Line:%d\n", __FUNCTION__, __LINE__);
                condition = 0;
            }
            break;

        case CMD_WAKEUP:
            condition = 1;
            osal_printk("--condition=1 before wakeup<<<Func:%s, Line:%d\n", __FUNCTION__, __LINE__);
            osal_wakeup(&st_osal_wait);
            osal_printk("--condition=1 after wakeup<<<Func:%s, Line:%d\n", __FUNCTION__, __LINE__);
            break;

        case CMD_SEMA:
            {
                int i;
                osal_semaphore_t st_sem;
                osal_printk("--sema test start...--Func:%s, Line:%d\n", __FUNCTION__, __LINE__);
                for(i=0; i<10000; i++)
                {
                    ret = osal_sema_init(&st_sem, 1);
                    if(ret)
                    {
                        osal_printk("--sema init ERROR--Func:%s, Line:%d\n", __FUNCTION__, __LINE__);
                        break;
                    }

                    ret = osal_down(&st_sem);
                    if(ret)
                    {
                        osal_printk("--sema down ERROR--Func:%s, Line:%d\n", __FUNCTION__, __LINE__);
                        break;
                    }

                    osal_up(&st_sem);

                    osal_sema_destory(&st_sem);
                }
                osal_printk("--sema test over--Func:%s, Line:%d\n", __FUNCTION__, __LINE__);
            }
            break;
            
        default:
            ret = -1;
            break;
    }

    return ret;
}

static unsigned int test_poll(osal_poll_t *poll, void *data){
    int chn = UMAP_GET_CHN(data);
    osal_printk("Func:%s, Line:%d, chn:%d\n", __FUNCTION__, __LINE__, chn);
    
    osal_poll_wait(poll, &test_wait);	
    if(poll_ready == 0){
    	osal_printk("--test_poll return 0!\n");
    	return 0;
    }else{
    	osal_printk("--test poll return %x !\n", OSAL_POLLIN|OSAL_POLLRDNORM);
    	poll_ready = 0;
    	return OSAL_POLLIN|OSAL_POLLRDNORM;
    }
}

static int test_mmap(osal_vm_t *vm, unsigned long start, unsigned long end, unsigned long vm_pgoff, void *private_data){
	osal_printk("Func:%s, Line:%d\n", __FUNCTION__, __LINE__);
	return 0;
}

static struct osal_fileops test_fops =
{
    .open = test_open,
    .release = test_release,
    .unlocked_ioctl = test_ioctl,
    .poll = test_poll,
    .mmap = test_mmap,
};

int mod_dev_init(void)
{
    int ret = 0;

    test_dev = osal_createdev("test0");
    test_dev->fops = &test_fops;
    test_dev->osal_pmops = OSAL_NULL;

    ret = osal_registerdevice(test_dev);
    osal_printk("osal_registerdevice : ret = %d\n", ret);

    osal_wait_init(&test_wait);

    ret = osal_wait_init(&st_osal_wait);
    if(ret)
    {
        osal_printk("--ERROR--Func:%s, Line:%d\n", __FUNCTION__, __LINE__);
        return ret;
    }
    
    osal_printk("--dev init ok--Func:%s, Line:%d\n", __FUNCTION__, __LINE__);
                
    return ret;
}

int mod_dev_destory(void)
{
    osal_wait_destory(&test_wait);

    osal_wait_destory(&st_osal_wait);

    osal_printk("--dev destory ok--Func:%s, Line:%d\n", __FUNCTION__, __LINE__);

    return 0;
}

