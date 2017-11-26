#ifndef __TEST_COMM_H__
#define __TEST_COMM_H__

#include "CUnit.h"

#define PRINT_OK(...) \
do{ \
    printf("\033[0;32mFunc:%s, Line:%d, \033[0m", __FUNCTION__, __LINE__); \
    printf(__VA_ARGS__); \
}while(0)


#define PRINT_ERR(...) \
do{ \
    printf("\033[0;31mFunc:%s, Line:%d, \033[0m", __FUNCTION__, __LINE__); \
    printf(__VA_ARGS__); \
}while(0)

#define PRINT_WARN(...) \
do{ \
    printf("\033[0;33mFunc:%s, Line:%d, \033[0m", __FUNCTION__, __LINE__); \
    printf(__VA_ARGS__); \
}while(0)


#ifndef TEST_PASS
#define TEST_PASS(msg)\
do{\
    printf("\033[0;32m Test case Func:%s PASS. Y(^_^)Y \033[0m\n", __FUNCTION__); \
    CU_PASS(msg);\
}while(0)
#endif

#ifndef TEST_NOTPASS
#define TEST_NOTPASS(msg)\
do{\
    printf("\033[0;31mFunc:%s, Line:%d, %s ::>_<::\033[0m\n", __FUNCTION__, __LINE__, ("(" #msg ")")); \
    CU_FAIL_FATAL(msg);\
}while(0)
#endif

#ifndef ASSERT_RET0
#define ASSERT_RET0(value)\
do{\
    if(0 != value) \
        TEST_NOTPASS(value); \
}while(0)
#endif

#endif  /* end of __TEST_COMM_H__ */


