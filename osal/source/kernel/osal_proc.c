#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/seq_file.h>
#include <linux/proc_fs.h>
#include <linux/slab.h>
#include <asm/uaccess.h>
#include "hi_osal.h"
#include "proc_fs.h"

#define OSAL_PROC_DEBUG 0

static struct osal_list_head list;
static osal_proc_entry_t* proc_entry = NULL;

static int osal_seq_show(struct seq_file* s, void* p)
{
    osal_proc_entry_t* sentry = s->private;
    if (sentry == NULL)
    {
        osal_printk("%s - parameter invalid!\n", __FUNCTION__);
        return -1;
    }
    sentry->seqfile = s;
    sentry->read(sentry);
    return 0;
}

static ssize_t osal_procwrite(struct proc_file* file, const char * buf, size_t count, loff_t* ppos)
{
    //char *kbuf;
    osal_proc_entry_t* sentry = ((struct seq_file*)(file->private_data))->private;
#if 0
    kbuf = kmalloc(count, GFP_KERNEL);
    if (kbuf == NULL)
    {
        osal_printk("%s - kmalloc failed!\n", __FUNCTION__);
        return -1;
    }
    copy_from_user(kbuf, buf, count);
    if (OSAL_PROC_DEBUG)
    {
        osal_printk("%s - %s!\n", __FUNCTION__, kbuf);
    }
#endif
    return sentry->write(sentry, (char*)buf, count, (long long *)ppos);
}

static int osal_procopen(struct inode* inode, struct proc_file* file)
{
    osal_proc_entry_t* sentry = file->pPDE->data;
    if(sentry->open!=NULL)
    {
    	sentry->open(sentry);
    }
    return single_open(file, osal_seq_show, sentry);
}

static struct proc_file_operations osal_proc_ops =
{
    .open    = osal_procopen,
    .read    = seq_read,
    .write   = osal_procwrite,
    .llseek  = seq_lseek,
    .release = single_release
};

osal_proc_entry_t* osal_create_proc_entry(const char* name, osal_proc_entry_t* parent)
{
    struct proc_dir_entry* entry = NULL;
    osal_proc_entry_t* sentry = NULL;
    parent = proc_entry;
    if (parent == NULL)
    {
        //osal_printk("%s - parent is NULL!\n", __FUNCTION__);
        entry = create_proc_entry(name, 0, NULL);
    }
    else
    {
        //osal_printk("%s - parent is not NULL! parent=%p\n", __FUNCTION__, parent->proc_dir_entry);
        entry = create_proc_entry(name, 0, parent->proc_dir_entry);
    }
    if (entry == NULL)
    {
        osal_printk("%s - create_proc_entry failed!\n", __FUNCTION__);
        return NULL;
    }
    sentry = kmalloc(sizeof(struct osal_proc_dir_entry), GFP_KERNEL);
    if (!sentry)
    {
        if (parent != NULL)
        {
            remove_proc_entry(name, parent->proc_dir_entry);
        }
        else
        {
            remove_proc_entry(name, NULL);
        }
        osal_printk("%s - kmalloc failed!\n", __FUNCTION__);
        return NULL;
    }
    memset(sentry, 0, sizeof(struct osal_proc_dir_entry));
    osal_strcpy(sentry->name, name);
    sentry->proc_dir_entry = entry;
    entry->proc_fops = &osal_proc_ops;
    entry->data = sentry;
    osal_list_add_tail(&(sentry->node), &list);
    return sentry;
}
osal_proc_entry_t* osal_proc_mkdir(const char* name, osal_proc_entry_t* parent)
{
    struct proc_dir_entry* proc = NULL;
    struct osal_proc_dir_entry* sproc = NULL;
    if (parent != NULL)
    {
        proc = proc_mkdir(name, parent->proc_dir_entry);
        osal_printk("%s - parent is not NULL!\n", __FUNCTION__);
        //proc = create_proc_entry(name, S_IFDIR | S_IRUGO | S_IXUGO, parent->proc_dir_entry);
    }
    else
    {
        proc = proc_mkdir(name, NULL);
        //osal_printk("%s - parent is NULL! proc=%p \n", __FUNCTION__, proc);
        //proc = create_proc_entry(name, S_IFDIR | S_IRUGO | S_IXUGO, NULL);
    }
    if (proc == NULL)
    {
        osal_printk("%s - proc_mkdir failed!\n", __FUNCTION__);
        return NULL;
    }
    //osal_memset(proc, 0x00, sizeof(proc));
    sproc = kmalloc(sizeof(struct osal_proc_dir_entry), GFP_KERNEL);
    if (!sproc)
    {
        if (parent != NULL)
        {
            remove_proc_entry(name, parent->proc_dir_entry);
        }
        else
        {
            remove_proc_entry(name, NULL);
        }
        osal_printk("%s - kmalloc failed!\n", __FUNCTION__);
        return NULL;
    }
    osal_strcpy(sproc->name, name);
    sproc->proc_dir_entry = proc;
    //sproc->read = NULL;
    //sproc->write = NULL;
    //proc->proc_fops = NULL;
    proc->data = sproc;
    osal_list_add_tail(&(sproc->node), &list);
    return sproc;
}

void osal_remove_proc_entry(const char* name, osal_proc_entry_t* parent)
{
    struct osal_proc_dir_entry* sproc = NULL;
    parent = proc_entry;
    if (name == NULL)
    {
        osal_printk("%s - parameter invalid!\n", __FUNCTION__);
        return;
    }
    if (parent != NULL)
    {
        remove_proc_entry(name, parent->proc_dir_entry);
    }
    else
    {
        remove_proc_entry(name, NULL);
    }
    osal_list_for_each_entry(sproc, &list, node)
    {
        if (osal_strcmp(sproc->name, name) == 0)
        {
            osal_list_del(&(sproc->node));
            break;
        }
    }
    if (sproc != NULL)
    {
        kfree(sproc);
        sproc = NULL;
    }
}
void osal_remove_proc_root(const char *name, osal_proc_entry_t *parent){
	struct osal_proc_dir_entry * sproc = NULL;
	if(name == NULL){
		osal_printk("%s - parameter invalid!\n", __FUNCTION__);
		return;
	}
	if(parent != NULL)
		remove_proc_entry(name, parent->proc_dir_entry);
	else
		remove_proc_entry(name, NULL); 
	osal_list_for_each_entry(sproc, &list, node){
		if(osal_strcmp(sproc->name, name) == 0){
			osal_list_del(&(sproc->node));
			break;
		}
	}
	if(sproc != NULL)
	{
            kfree(sproc);
            sproc = NULL;
	}
}

int osal_seq_printf(osal_proc_entry_t* entry, const char* fmt, ...)
{
    struct seq_file* s = (struct seq_file*)(entry->seqfile);
    va_list args;
    char b[512];
    int r;
    va_start(args, fmt);
    r = vsnprintf(b, 512, fmt, args);
    va_end(args);
    r = seq_printf(s, b);
    return r;
}

void osal_proc_init(void)
{
    OSAL_INIT_LIST_HEAD(&list);
    proc_entry = osal_proc_mkdir("umap", OSAL_NULL);
    if (proc_entry == OSAL_NULL)
    {
        osal_printk("test init, proc mkdir error!\n");
    }
}
void osal_proc_exit(void)
{
    osal_remove_proc_root("umap", OSAL_NULL);
}

