#ifndef __HI_ISP_MOD_PARAM__
#define __HI_ISP_MOD_PARAM__

#include "hi_type.h"

typedef struct hiISP_MODULE_PARAMS_S
{
    HI_U32 u32PwmNum;
    HI_U32 u32ProcParam;
    HI_U32 u32UpdatePos;
    HI_BOOL bIntBottomHalf;
}ISP_MODULE_PARAMS_S;

#endif

