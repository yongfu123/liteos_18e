#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/ioctl.h>
#include <linux/poll.h>
#include <linux/slab.h>
#include <linux/vmalloc.h>
#include <linux/mm.h>
#include "hi_osal.h"

#define DRVAL_DEBUG 0

typedef struct osal_coat_dev
{
    struct osal_dev osal_dev;
    //struct himedia_device himedia_dev;
} osal_coat_dev_t;


struct osal_private_data
{
    struct osal_dev* dev;
    void* data;
    struct osal_poll table;
};

static int osal_open(struct file* file)
{
    osal_coat_dev_t* coat_dev;
    struct osal_private_data* pdata = NULL;

    coat_dev = (struct osal_coat_dev*)file->f_inode->i_private;
    if (coat_dev == NULL)
    {
        osal_printk("%s - get coat_dev device error!\n", __FUNCTION__);
        return -1;
    }

    pdata = (struct osal_private_data*)kmalloc(sizeof(struct osal_private_data), GFP_KERNEL);
    if (pdata == NULL)
    {
        osal_printk("%s - kmalloc error!\n", __FUNCTION__);
        return -1;
    }
    if (DRVAL_DEBUG)
    {
        osal_printk("%s - file->private_data=%p!\n", __FUNCTION__, pdata);
    }
    file->f_priv = pdata;
    pdata->dev = &(coat_dev->osal_dev);
    if (coat_dev->osal_dev.fops->open != NULL)
    {
        return coat_dev->osal_dev.fops->open((void*) & (pdata->data));
    }
    return 0;
}

static int osal_read(struct file* file, char* buf, size_t size)
{
    struct osal_private_data* pdata = file->f_priv;
    int ret = 0;

    if (pdata->dev->fops->read != NULL)
    {
        ret = pdata->dev->fops->read(buf, (int)size, 0, (void*) & (pdata->data));
    }
    return ret;
}

static int osal_write(struct file* file, const char* buf, size_t size)
{
    struct osal_private_data* pdata = file->f_priv;
    int ret = 0;

    if (pdata->dev->fops->write != NULL)
    {
        ret = pdata->dev->fops->write(buf, (int)size, 0, (void*) & (pdata->data));
    }

    return ret;
}

static loff_t osal_llseek(struct file* file, loff_t offset, int whence)
{
    struct osal_private_data* pdata = file->f_priv;
    int ret = 0;
    if (DRVAL_DEBUG)
    {
        osal_printk("%s - file->private_data=%p!\n", __FUNCTION__, pdata);
    }

    if (whence == SEEK_SET)
    {
        if (pdata->dev->fops->llseek != NULL)
        {
            ret = pdata->dev->fops->llseek((long)offset, OSAL_SEEK_SET, (void*) & (pdata->data));
        }
    }
    else if (whence == SEEK_CUR)
    {
        if (pdata->dev->fops->llseek != NULL)
        {
            ret = pdata->dev->fops->llseek((long)offset, OSAL_SEEK_CUR, (void*) & (pdata->data));
        }
    }
    else if (whence == SEEK_END)
    {
        if (pdata->dev->fops->llseek != NULL)
        {
            ret = pdata->dev->fops->llseek((long)offset, OSAL_SEEK_END, (void*) & (pdata->data));
        }
    }
    return (loff_t)ret;
}

static int osal_release(struct file* file)
{
    int ret = 0;
    struct osal_private_data* pdata = file->f_priv;

    if (DRVAL_DEBUG)
    {
        osal_printk("%s - file->private_data=%p!\n", __FUNCTION__, pdata);
    }

    if (pdata->dev->fops->release != NULL)
    {
        ret = pdata->dev->fops->release((void*) & (pdata->data));
    }
    if (ret != 0)
    {
        osal_printk("%s - release failed!\n", __FUNCTION__);
        return ret;
    }
    kfree(file->f_priv);
    file->f_priv = NULL;
    return 0;
}

static long osal_unlocked_ioctl(struct file* file, int cmd, unsigned long arg)
{
    int ret = -1;
    struct osal_private_data* pdata = file->f_priv;
    //  char *kbuf = NULL;

    if (DRVAL_DEBUG)
    {
        osal_printk("%s - file->private_data=%p!\n", __FUNCTION__, pdata);
    }

    if (pdata->dev->fops->unlocked_ioctl == NULL)
    {
        return -1;
    }
    else
    {
        ret = pdata->dev->fops->unlocked_ioctl(cmd, arg, (void*) & (pdata->data));
    }

#if 0
    if (_IOC_DIR(cmd) == _IOC_NONE)
    {
        if (pdata->dev->fops->unlocked_ioctl == NULL)
        {
            return -1;
        }
        else
        {
            ret = pdata->dev->fops->unlocked_ioctl(cmd, arg, (void*) & (pdata->data));
        }
    }
    else if (_IOC_DIR(cmd) == _IOC_WRITE)
    {
        kbuf = (char*)vmalloc(_IOC_SIZE(cmd));
        if (kbuf == NULL)
        {
            osal_printk("%s - vmalloc failed!\n", __FUNCTION__);
            return -1;
        }
        copy_from_user(kbuf, (char*)arg, _IOC_SIZE(cmd));
        if (pdata->dev->fops->unlocked_ioctl == NULL)
        {
            vfree(kbuf);
            return -1;
        }
        else
        {
            ret = pdata->dev->fops->unlocked_ioctl(cmd, (unsigned long)kbuf, (void*) & (pdata->data));
        }
    }
    else if (_IOC_DIR(cmd) == _IOC_READ)
    {
        kbuf = vmalloc(_IOC_SIZE(cmd));
        if (kbuf == NULL)
        {
            osal_printk("%s - vmalloc failed!\n", __FUNCTION__);
            return -1;
        }
        if (pdata->dev->fops->unlocked_ioctl == NULL)
        {
            vfree(kbuf);
            return -1;
        }
        else
        {
            ret = pdata->dev->fops->unlocked_ioctl(cmd, (unsigned long)kbuf, (void*) & (pdata->data));
            if (ret == 0)
            {
                copy_to_user((char*)arg, kbuf, _IOC_SIZE(cmd));
            }
        }
    }
    else if (_IOC_DIR(cmd) == (_IOC_READ + _IOC_WRITE))
    {
        kbuf = vmalloc(_IOC_SIZE(cmd));
        if (kbuf == NULL)
        {
            osal_printk("%s - vmalloc failed!\n", __FUNCTION__);
            return -1;
        }
        copy_from_user(kbuf, (char*)arg, _IOC_SIZE(cmd));
        if (pdata->dev->fops->unlocked_ioctl == NULL)
        {
            vfree(kbuf);
            return -1;
        }
        else
        {
            ret = pdata->dev->fops->unlocked_ioctl(cmd, (unsigned long)kbuf, (void*) & (pdata->data));
            if (ret == 0)
            {
                copy_to_user((char*)arg, kbuf, _IOC_SIZE(cmd));
            }
        }
    }

    if (kbuf != NULL)
    {
        vfree(kbuf);
    }
#endif
    return ret;
}

static unsigned int osal_poll(struct file* file, struct poll_table* table)
{
    struct osal_private_data* pdata = file->f_priv;
    osal_coat_dev_t* coat_dev = container_of(pdata->dev, struct osal_coat_dev, osal_dev);
    struct osal_poll* t = &(pdata->table);

    if (DRVAL_DEBUG)
    {
        osal_printk("%s - table=%p, file=%p!\n", __FUNCTION__, table, file);
    }
    t->poll_table = table;
    t->data = file;
    if (pdata->dev->fops->poll != NULL)
    {
        return pdata->dev->fops->poll(t, (void*) & (pdata->data));
    }
    return 0;
}

static struct file_operations_vfs s_osal_fops =
{
    .open = osal_open,
    .close = osal_release,
    .read = osal_read,
    .write = osal_write,
    .seek = osal_llseek,
    .ioctl = osal_unlocked_ioctl,
    .poll = osal_poll,

};

osal_dev_t* osal_createdev(char* name)
{
    osal_coat_dev_t* pdev;
    if (name == NULL)
    {
        osal_printk("%s - parameter invalid!\n", __FUNCTION__);
        return NULL;
    }
    pdev = (osal_coat_dev_t*)kmalloc(sizeof(osal_coat_dev_t), GFP_KERNEL);
    if (pdev == NULL)
    {
        osal_printk("%s - kmalloc error!\n", __FUNCTION__);
        return NULL;
    }
    memset(pdev, 0, sizeof(osal_coat_dev_t));
    osal_snprintf(pdev->osal_dev.name, 48, "/dev/%s", name);
    pdev->osal_dev.dev = pdev;
    return &(pdev->osal_dev);
}

int osal_destroydev(osal_dev_t* osal_dev)
{
    osal_coat_dev_t* pdev;
    if (osal_dev == NULL)
    {
        osal_printk("%s - parameter invalid!\n", __FUNCTION__);
        return -1;
    }
    pdev = osal_dev->dev;
    if (pdev == NULL)
    {
        osal_printk("%s - parameter invalid!\n", __FUNCTION__);
        return -1;
    }
    kfree(pdev);
    pdev = NULL;
    
    return 0;
}

int osal_registerdevice(osal_dev_t* osal_dev)
{
    if (osal_dev == NULL || osal_dev->fops == NULL)
    {
        osal_printk("%s - parameter invalid!\n", __FUNCTION__);
        return -1;
    }

    return register_driver(osal_dev->name, &s_osal_fops, 0x0666, osal_dev->dev);
}

void osal_deregisterdevice(osal_dev_t* pdev)
{
    if (pdev == NULL)
    {
        osal_printk("%s - parameter invalid!\n", __FUNCTION__);
        return;
    }

    return unregister_driver(pdev->name);
}

void osal_poll_wait(osal_poll_t* table, osal_wait_t* wait)
{
    if (DRVAL_DEBUG)
    {
        osal_printk("%s - call poll_wait +!, table=%p, file=%p\n", __FUNCTION__, table->poll_table, table->data);
    }

    poll_wait((struct file*)table->data, (wait_queue_head_t*)(wait->wait), table->poll_table);

    if (DRVAL_DEBUG)
    {
        osal_printk("%s - call poll_wait -!\n", __FUNCTION__);
    }
}

void osal_pgprot_noncached(osal_vm_t* vm)
{
    /* not support yet in liteos */
    return;
}

void osal_pgprot_cached(osal_vm_t *vm)
{
    /* not support yet in liteos */
    return;
}

void osal_pgprot_writecombine(osal_vm_t* vm)
{
    /* not support yet in liteos */
    return;
}

void osal_pgprot_stronglyordered(osal_vm_t* vm)
{
    /* not support yet in liteos */
    return;
}

int osal_remap_pfn_range(osal_vm_t* vm, unsigned long addr, unsigned long pfn, unsigned long size)
{
    /* not support yet in liteos */
    return 0;
}

int osal_io_remap_pfn_range(osal_vm_t* vm, unsigned long addr, unsigned long pfn, unsigned long size)
{
    /* not support yet in liteos */
    return 0;
}

#ifdef CONFIG_HISI_SNAPSHOT_BOOT
int osal_call_usermodehelper_force(char *path, char **argv, char **envp, int wait){
    /* not support yet in liteos */
    return 0;
}
#endif

