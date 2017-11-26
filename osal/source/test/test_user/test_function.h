#ifndef __TEST_FUNCTION_H__
#define __TEST_FUNCTION_H__


int test_init(void);
int test_destroy(void);

void test_dev_open_close(void);
void test_dev_ioctl(void);
void test_dev_poll(void);

void test_dev_wait_event(void);

void test_dev_semaphore(void);

void test_MMZ(void);


#endif

