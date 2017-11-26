#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/prctl.h>

#include "test_api.h"
#include "test_comm.h"
#include "osal_mmz.h"

static int stop_app = 0;

int test_init(void)
{
    stop_app = 0;
    return mod_dev_init();
}

int test_destroy(void)
{
    stop_app = 1;
    return mod_dev_destory();
}

void test_dev_open_close(void)
{
    int test0_fd;

    test0_fd = open("/dev/test0", O_RDONLY);
    if(test0_fd<0)
    {
        TEST_NOTPASS("open dev fail.\n");
    }

    ASSERT_RET0(close(test0_fd));

    TEST_PASS();
}

void test_dev_ioctl(void)
{
    int test0_fd;
    int chn = 12345;

    test0_fd = open("/dev/test0", O_RDONLY);
    if(test0_fd<0)
    {
        TEST_NOTPASS("open dev fail.\n");
    }

    ASSERT_RET0(ioctl(test0_fd, CMD_SETDATA, &chn));


    ASSERT_RET0(close(test0_fd));

    TEST_PASS();
}

typedef struct read_fd_st
{
    int fd;
} thread_data_s;

static void* thread_readfd(void* p)
{
    int maxfd = 0;
    fd_set read_fds;
    struct timeval timeout;
    thread_data_s *param;
    int test0_fd = 0;
    int ret = 0;

    prctl(PR_SET_NAME,"readfd_thread", 0, 0, 0);
    
    param = (thread_data_s*)p;
    test0_fd = param->fd;
    
    /* test poll  */
    maxfd = test0_fd+1;

    while(!stop_app)
    {
        FD_ZERO(&read_fds);
        FD_SET(test0_fd, &read_fds);
        timeout.tv_sec = 2;
        timeout.tv_usec = 0;

        printf("before select, wait...\n");
        ret = select(maxfd, &read_fds, NULL, NULL, &timeout);
        if(ret<0)
        {
            PRINT_ERR("select ret error :%d.\n", ret);
            break;
        }
        else if(ret == 0)
        {
            PRINT_WARN("select timeout.\n");
        }
        else
        {
            if(FD_ISSET(test0_fd, &read_fds))
            {
                PRINT_OK("select ok.\n");
            }
        }

    }

    return;
}


void test_dev_poll(void)
{
    int test0_fd;
    int count = 5;
    int chn = 12345;
    thread_data_s st_value;
    pthread_t threadid;
    pthread_attr_t attr;
    
    test0_fd = open("/dev/test0", O_RDONLY);
    if(test0_fd<0)
    {
        TEST_NOTPASS("open dev fail.\n");
    }

    pthread_attr_init(&attr);
    attr.stacksize = 0x1000;
    
    st_value.fd = test0_fd;
    ASSERT_RET0(pthread_create(&threadid, &attr, thread_readfd, (void*)&st_value));

    ASSERT_RET0(ioctl(test0_fd, CMD_SETDATA, &chn));

    ASSERT_RET0(ioctl(test0_fd, CMD_POLL, &count));

    sleep(5);   //for select timeout
    stop_app = 1;
    pthread_join(threadid, 0);
    
    ASSERT_RET0(close(test0_fd));

    TEST_PASS();
}

static void* thread_wakeup(void* p)
{
    thread_data_s *param;
    int test0_fd = 0;
    int ret = 0;

    prctl(PR_SET_NAME,"wakeup_thread", 0, 0, 0);
    
    param = (thread_data_s*)p;
    test0_fd = param->fd;
    
    while(!stop_app)
    {
#if 0
        printf(">>>press any key to wakeup<<<Func:%s, Line:%d\n", __FUNCTION__, __LINE__);
        getchar();
#else
        sleep(2);
#endif
        ret=ioctl(test0_fd, CMD_WAKEUP);
        if(ret)
        {
            printf(">>>IOCTL error, Func:%s, Line:%d\n", __FUNCTION__, __LINE__);
        }

    }

    return;
}


void test_dev_wait_event(void)
{
    int test0_fd;
    thread_data_s st_value;
    pthread_t threadid;

    test0_fd = open("/dev/test0", O_RDONLY);
    if(test0_fd<0)
    {
        TEST_NOTPASS("open dev fail.\n");
    }

    st_value.fd = test0_fd;
    ASSERT_RET0(pthread_create(&threadid, 0, thread_wakeup, (void*)&st_value));

    ASSERT_RET0(ioctl(test0_fd, CMD_WAIT));

    stop_app = 1;
    pthread_join(threadid, 0);

    ASSERT_RET0(close(test0_fd));

    TEST_PASS();
}

void test_dev_semaphore(void)
{
    int test0_fd;

    test0_fd = open("/dev/test0", O_RDONLY);
    if(test0_fd<0)
    {
        TEST_NOTPASS("open dev fail.\n");
    }

    ASSERT_RET0(ioctl(test0_fd, CMD_SEMA));

    ASSERT_RET0(close(test0_fd));

    TEST_PASS();
}

static int MMZ_init(void)
{
    extern int media_mem_init(void * pArgs);
    MMZ_MODULE_PARAMS_S stMMZ_Param;

    snprintf(stMMZ_Param.mmz, MMZ_SETUP_CMDLINE_LEN, "anonymous,0,0x82000000,32M");
    stMMZ_Param.anony = 1;

    return media_mem_init(&stMMZ_Param);
}

static unsigned int MmzMalloc(char *pMmzName, char *pBufName, unsigned int u32Size)
{
    hil_mmb_t *pmmb = NULL;
    pmmb = hil_mmb_alloc(pBufName, u32Size, 0, 0, pMmzName);
    if(NULL == pmmb)
    {
        return NULL;
    }

    return hil_mmb_phys(pmmb);
}

void MmzFree(unsigned int u32PhyAddr, void* pVirtAddr)
{
    if (NULL != pVirtAddr)
    {
        osal_iounmap(pVirtAddr);
    }

    if (0 != u32PhyAddr)
    {
        hil_mmb_freeby_phys(u32PhyAddr);
    }
}

void test_MMZ(void)
{
    unsigned int phyaddr;

    MMZ_init();

    phyaddr = MmzMalloc(NULL, NULL, 0x1000000);
    printf("You have 10 seconds to cat MMZ proc.\n");
    sleep(10);
    MmzFree(phyaddr, NULL);
    printf("You have 10 seconds to cat MMZ proc again.\n");
    sleep(10);

    TEST_PASS();
}



