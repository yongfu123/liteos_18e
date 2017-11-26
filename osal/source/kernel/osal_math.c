#include <linux/module.h>
#include <linux/kernel.h>
#include "hi_osal.h"

unsigned int osal_do_div(unsigned int n, unsigned int base)
{
    do_div(n , base);
    return n;

}

unsigned long long osal_do_div64(unsigned long long n, unsigned int base)
{
    do_div(n , base);
    return n;
}

unsigned int osal_random(void)
{
    return random();
}
