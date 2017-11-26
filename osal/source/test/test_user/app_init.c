/******************************************************************************
  Some simple Hisilicon Hi3516A system functions.

  Copyright (C), 2010-2015, Hisilicon Tech. Co., Ltd.
 ******************************************************************************
    Modification:  2015-6 Created
******************************************************************************/

#include "sys/types.h"

#include "sys/time.h"
#include "unistd.h"
#include "fcntl.h"
#include "sys/statfs.h"
#include "string.h"
#include "los_event.h"
#include "los_printf.h"

#if 1
#include "lwip/tcpip.h"
#include "lwip/netif.h"
#include "eth_drv.h"
#include "arch/perf.h"
#endif
#include "fcntl.h"
//#include "nuttx/fs/fs.h"
#include "shell.h"
#include <stdio.h>
#include "hisoc/uart.h"

extern int mem_dev_register(void);
extern UINT32 osShellInit();
extern int app_main();

#if (HICHIP==0x3518E200)
extern struct los_eth_driver hisi_eth_drv_sc;
#else
extern struct los_eth_driver higmac_drv_sc;
#endif
struct netif *pnetif;

void app_sample(void)
{
    UINT32 camera_taskid;
    TSK_INIT_PARAM_S stappTask;
    int s32Ret = 0;

    memset(&stappTask, 0, sizeof(TSK_INIT_PARAM_S));
    stappTask.pfnTaskEntry = (TSK_ENTRY_FUNC)app_main;
    stappTask.uwStackSize  = 0x10000;
    stappTask.pcName = "sample";
    stappTask.usTaskPrio = 10;
    stappTask.uwResved   = LOS_TASK_STATUS_DETACHED;
    s32Ret = LOS_TaskCreate(&camera_taskid, &stappTask);
    dprintf("camera_Task %d\n", camera_taskid);

    //chdir("/sd0");
    chdir("/nfs");
}

void sample_command(void) 
{
    osCmdReg(CMD_TYPE_STD,"sample", 0, (CMD_CBK_FUNC)app_sample);
}

void net_init()
{
    #if (HICHIP==0x3518E200)
    pnetif = &(hisi_eth_drv_sc.ac_if);
    #else
    pnetif = &(higmac_drv_sc.ac_if);
    #endif
    tcpip_init(NULL, NULL);

    #if (HICHIP==0x3518E200)
    hisi_eth_init();
    #else
    higmac_init();
    #endif
    
    printf("cmd_network : DHCP_BOUND finished\n");
    netif_set_up(pnetif);
}

extern char shell_working_directory[PATH_MAX];

extern unsigned long g_sys_mem_addr_end;

void board_config(void)
{
    g_sys_mem_addr_end = 0x82000000;
}

void app_init(void)
{
    UINT32 camera_taskid;
    TSK_INIT_PARAM_S stappTask;
    int s32Ret = 0;

    proc_fs_init();
    
    dprintf("uart init ...\n");
    hi_uartdev_init();

    dprintf("shell init ...\n");
    system_console_init(TTY_DEVICE);
    osShellInit();
    shell_cmd_register();

    dprintf("random init ...\n");
    ran_dev_register();

    #if 0
    dprintf("sd/mmc host init\n");
    SD_MMC_Host_init();

    dprintf("nand init start...\n");
    if (!nand_init())
    {
        add_mtd_partition("nand", 0x900000, 16 * 0x100000, 0);
        add_mtd_partition("nand", 0x900000 + 16 * 0x100000, 16 * 0x100000, 1);
        mount("/dev/nandblk0", "/yaffs0", "yaffs", 0, NULL);
        //mount("/dev/nandblk1", "/yaffs1", "yaffs", 0, NULL);
    }
    /*
    dprintf("spi nor falsh init ...\n");
    if(!spinor_init()){
        add_mtd_partition("spinor", 0x100000, 2*0x100000, 0);
        add_mtd_partition("spinor", 3*0x100000, 2*0x100000, 1);
        mount("/dev/spinorblk0", "/jffs0", "jffs", 0, NULL);
        //mount("/dev/spinorblk1", "/jffs1", "jffs", 0, NULL);
    }*/

    dprintf("spi bus init start...\n");
    hi_spi_init();

    dprintf("i2c bus init start...\n");
    i2c_dev_init();

    dprintf("gpio init ...\n");
    hi_gpio_init();

    dprintf("dmac init\n");
    hi_dmac_init();

    

    dprintf("net init ...\n");
    net_init();

    dprintf("usb init ...\n");
    usb_init();

    dprintf("Now shell working dir is :%s\n", shell_working_directory);

    shCmd_memcheck(0, NULL);

    s32Ret = mount("/dev/mmcblk0p0", "/sd0", "vfat", 0, 0);
    if (s32Ret)
    {
        printf("mount mmcblk0p0 to sd0 err %d\n", s32Ret);
    }
    #endif
    
    dprintf("done init!\n");
    dprintf("Date:%s.\n", __DATE__);
    dprintf("Time:%s.\n", __TIME__);

    mem_dev_register();

#if 1
    sample_command();
#else
    memset(&stappTask, 0, sizeof(TSK_INIT_PARAM_S));
    stappTask.pfnTaskEntry = (TSK_ENTRY_FUNC)app_main;
    stappTask.uwStackSize  = 0x10000;
    stappTask.pcName = "sample";
    stappTask.usTaskPrio = 10;
    stappTask.uwResved   = LOS_TASK_STATUS_DETACHED;
    s32Ret = LOS_TaskCreate(&camera_taskid, &stappTask);
    dprintf("camera_Task %d\n", camera_taskid);
#endif

    return;

}

/* EOF kthread1.c */
