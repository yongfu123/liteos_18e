#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/timer.h>
#include <linux/delay.h>
#include <linux/slab.h>
#include <linux/sched.h>
#include <linux/rtc.h>

#include "hi_osal.h"


int osal_timer_init(osal_timer_t* timer)
{
    struct timer_list* t = NULL;

    if (timer == NULL)
    {
        osal_printk("%s - parameter invalid!\n", __FUNCTION__);
        return -1;
    }

    t = (struct timer_list*)kmalloc(sizeof(struct timer_list), GFP_KERNEL);
    if (t == NULL)
    {
        osal_printk("%s - kmalloc error!\n", __FUNCTION__);
        return -1;
    }

    init_timer(t);
    timer->timer = t;
    return 0;
}

int osal_set_timer(osal_timer_t* timer, unsigned long interval)
{
    struct timer_list* t = NULL;
    if (timer == NULL || timer->timer == NULL || timer->function == NULL || interval == 0)
    {
        osal_printk("%s - parameter invalid!\n", __FUNCTION__);
        return -1;
    }
    t = timer->timer;
    t->function = timer->function;
    t->data = timer->data;
    t->expires = jiffies + msecs_to_jiffies(interval);
    add_timer(t);

    return 0;
    // todo: why mod_timer do not work in liteos
    //return mod_timer(t, jiffies + msecs_to_jiffies(interval));
}

int osal_del_timer(osal_timer_t* timer)
{
    struct timer_list* t = NULL;
    if (timer == NULL || timer->timer == NULL || timer->function == NULL)
    {
        osal_printk("%s - parameter invalid!\n", __FUNCTION__);
        return -1;
    }
    t = timer->timer;
    return del_timer(t);
}

int osal_timer_destory(osal_timer_t* timer)
{
    struct timer_list* t = timer->timer;
    del_timer(t);
    kfree(t);
    timer->timer = NULL;
    return 0;
}

unsigned long osal_msleep(unsigned int msecs)
{
    msleep(msecs);
    return 0;
}

void osal_udelay(unsigned int usecs)
{
    udelay(usecs);
}

void osal_mdelay(unsigned int msecs)
{
    mdelay(msecs);
}

unsigned int osal_get_tickcount()
{
    return jiffies_to_msecs(jiffies);
}

unsigned long long osal_sched_clock()
{
    return hi_sched_clock();
}

void osal_gettimeofday(osal_timeval_t* tv)
{
    struct timeval t;
    if (tv == NULL)
    {
        osal_printk("%s - parameter invalid!\n", __FUNCTION__);
        return;
    }
    do_gettimeofday(&t);

    tv->tv_sec = t.tv_sec;
    tv->tv_usec = t.tv_usec;
}

void osal_rtc_time_to_tm(unsigned long time, osal_rtc_time_t *tm)
{
    struct rtc_time _tm;

    rtc_time_to_tm(time, &_tm);

    tm->tm_sec   = _tm.tm_sec;
    tm->tm_min   = _tm.tm_min;
    tm->tm_hour  = _tm.tm_hour;
    tm->tm_mday  = _tm.tm_mday;
    tm->tm_mon   = _tm.tm_mon;
    tm->tm_year  = _tm.tm_year;
    tm->tm_wday  = _tm.tm_wday;
    tm->tm_yday  = _tm.tm_yday;
    tm->tm_isdst = _tm.tm_isdst;
}

void osal_rtc_tm_to_time(osal_rtc_time_t *tm ,unsigned long *time)
{
    struct rtc_time _tm;
    _tm.tm_sec  = tm->tm_sec;
    _tm.tm_min  = tm->tm_min;
    _tm.tm_hour = tm->tm_hour;
    _tm.tm_mday = tm->tm_mday;
    _tm.tm_mon  = tm->tm_mon;
    _tm.tm_year = tm->tm_year;
    _tm.tm_wday = tm->tm_wday;
    _tm.tm_yday = tm->tm_yday;
    _tm.tm_isdst = tm->tm_isdst;

    rtc_tm_to_time(&_tm, time);
}

void osal_getjiffies(unsigned long long *pjiffies)
{
    *pjiffies = jiffies;
}

int osal_rtc_valid_tm(struct osal_rtc_time *tm)
{
    struct rtc_time _tm;
    _tm.tm_sec  = tm->tm_sec;
    _tm.tm_min  = tm->tm_min;
    _tm.tm_hour = tm->tm_hour;
    _tm.tm_mday = tm->tm_mday;
    _tm.tm_mon  = tm->tm_mon;
    _tm.tm_year = tm->tm_year;
    _tm.tm_wday = tm->tm_wday;
    _tm.tm_yday = tm->tm_yday;
    _tm.tm_isdst = tm->tm_isdst;
    
    return rtc_valid_tm(&_tm);
}

