/*     here is pwm arch .
*
*
*  This file defines pwm micro-definitions for user.
*
* History:
*     03-Mar-2016 Start of Hi351xx Digital Camera,H6
*
*/

#ifndef _PWM_ARCH_H
#define _PWM_ARCH_H

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */


#define PWMI_ADRESS_BASE     0x12130000    

#define HI_IO_PWMI_ADDRESS(base_va, x)  (base_va + ((x)-(PWMI_ADRESS_BASE)))

//PWMI
#define PWM0_CFG_REG0(base_va)        HI_IO_PWMI_ADDRESS(base_va, PWMI_ADRESS_BASE + 0x0000)
#define PWM0_CFG_REG1(base_va)        HI_IO_PWMI_ADDRESS(base_va, PWMI_ADRESS_BASE + 0x0004)
#define PWM0_CFG_REG2(base_va)        HI_IO_PWMI_ADDRESS(base_va, PWMI_ADRESS_BASE + 0x0008)
#define PWM0_CTRL_REG(base_va)        HI_IO_PWMI_ADDRESS(base_va, PWMI_ADRESS_BASE + 0x000C)
#define PWM0_STATE_REG0(base_va)      HI_IO_PWMI_ADDRESS(base_va, PWMI_ADRESS_BASE + 0x0010)
#define PWM0_STATE_REG1(base_va)      HI_IO_PWMI_ADDRESS(base_va, PWMI_ADRESS_BASE + 0x0014)
#define PWM0_STATE_REG2(base_va)      HI_IO_PWMI_ADDRESS(base_va, PWMI_ADRESS_BASE + 0x0018)
             
#define PWM1_CFG_REG0(base_va)        HI_IO_PWMI_ADDRESS(base_va, PWMI_ADRESS_BASE + 0x0020)
#define PWM1_CFG_REG1(base_va)        HI_IO_PWMI_ADDRESS(base_va, PWMI_ADRESS_BASE + 0x0024)
#define PWM1_CFG_REG2(base_va)        HI_IO_PWMI_ADDRESS(base_va, PWMI_ADRESS_BASE + 0x0028)
#define PWM1_CTRL_REG(base_va)        HI_IO_PWMI_ADDRESS(base_va, PWMI_ADRESS_BASE + 0x002C)
#define PWM1_STATE_REG0(base_va)      HI_IO_PWMI_ADDRESS(base_va, PWMI_ADRESS_BASE + 0x0030)
#define PWM1_STATE_REG1(base_va)      HI_IO_PWMI_ADDRESS(base_va, PWMI_ADRESS_BASE + 0x0034)
#define PWM1_STATE_REG2(base_va)      HI_IO_PWMI_ADDRESS(base_va, PWMI_ADRESS_BASE + 0x0038)
             
#define PWM2_CFG_REG0(base_va)        HI_IO_PWMI_ADDRESS(base_va, PWMI_ADRESS_BASE + 0x0040)
#define PWM2_CFG_REG1(base_va)        HI_IO_PWMI_ADDRESS(base_va, PWMI_ADRESS_BASE + 0x0044)
#define PWM2_CFG_REG2(base_va)        HI_IO_PWMI_ADDRESS(base_va, PWMI_ADRESS_BASE + 0x0048)
#define PWM2_CTRL_REG(base_va)        HI_IO_PWMI_ADDRESS(base_va, PWMI_ADRESS_BASE + 0x004C)
#define PWM2_STATE_REG0(base_va)      HI_IO_PWMI_ADDRESS(base_va, PWMI_ADRESS_BASE + 0x0050)
#define PWM2_STATE_REG1(base_va)      HI_IO_PWMI_ADDRESS(base_va, PWMI_ADRESS_BASE + 0x0054)
#define PWM2_STATE_REG2(base_va)      HI_IO_PWMI_ADDRESS(base_va, PWMI_ADRESS_BASE + 0x0058)
                     
#define PWM3_CFG_REG0(base_va)        HI_IO_PWMI_ADDRESS(base_va, PWMI_ADRESS_BASE + 0x0060)
#define PWM3_CFG_REG1(base_va)        HI_IO_PWMI_ADDRESS(base_va, PWMI_ADRESS_BASE + 0x0064)
#define PWM3_CFG_REG2(base_va)        HI_IO_PWMI_ADDRESS(base_va, PWMI_ADRESS_BASE + 0x0068)
#define PWM3_CTRL_REG(base_va)        HI_IO_PWMI_ADDRESS(base_va, PWMI_ADRESS_BASE + 0x006C)
#define PWM3_STATE_REG0(base_va)      HI_IO_PWMI_ADDRESS(base_va, PWMI_ADRESS_BASE + 0x0070)
#define PWM3_STATE_REG1(base_va)      HI_IO_PWMI_ADDRESS(base_va, PWMI_ADRESS_BASE + 0x0074)
#define PWM3_STATE_REG2(base_va)      HI_IO_PWMI_ADDRESS(base_va, PWMI_ADRESS_BASE + 0x0078)
                   
#define PWM4_CFG_REG0(base_va)        HI_IO_PWMI_ADDRESS(base_va, PWMI_ADRESS_BASE + 0x0080)
#define PWM4_CFG_REG1(base_va)        HI_IO_PWMI_ADDRESS(base_va, PWMI_ADRESS_BASE + 0x0084)
#define PWM4_CFG_REG2(base_va)        HI_IO_PWMI_ADDRESS(base_va, PWMI_ADRESS_BASE + 0x0088)
#define PWM4_CTRL_REG(base_va)        HI_IO_PWMI_ADDRESS(base_va, PWMI_ADRESS_BASE + 0x008C)
#define PWM4_STATE_REG0(base_va)      HI_IO_PWMI_ADDRESS(base_va, PWMI_ADRESS_BASE + 0x0090)
#define PWM4_STATE_REG1(base_va)      HI_IO_PWMI_ADDRESS(base_va, PWMI_ADRESS_BASE + 0x0094)
#define PWM4_STATE_REG2(base_va)      HI_IO_PWMI_ADDRESS(base_va, PWMI_ADRESS_BASE + 0x0098)
                  
#define PWM5_CFG_REG0(base_va)        HI_IO_PWMI_ADDRESS(base_va, PWMI_ADRESS_BASE + 0x00A0)
#define PWM5_CFG_REG1(base_va)        HI_IO_PWMI_ADDRESS(base_va, PWMI_ADRESS_BASE + 0x00A4)
#define PWM5_CFG_REG2(base_va)        HI_IO_PWMI_ADDRESS(base_va, PWMI_ADRESS_BASE + 0x00A8)
#define PWM5_CTRL_REG(base_va)        HI_IO_PWMI_ADDRESS(base_va, PWMI_ADRESS_BASE + 0x00AC)
#define PWM5_STATE_REG0(base_va)      HI_IO_PWMI_ADDRESS(base_va, PWMI_ADRESS_BASE + 0x00B0)
#define PWM5_STATE_REG1(base_va)      HI_IO_PWMI_ADDRESS(base_va, PWMI_ADRESS_BASE + 0x00B4)
#define PWM5_STATE_REG2(base_va)      HI_IO_PWMI_ADDRESS(base_va, PWMI_ADRESS_BASE + 0x00B8)
                  
#define PWM6_CFG_REG0(base_va)        HI_IO_PWMI_ADDRESS(base_va, PWMI_ADRESS_BASE + 0x00C0)
#define PWM6_CFG_REG1(base_va)        HI_IO_PWMI_ADDRESS(base_va, PWMI_ADRESS_BASE + 0x00C4)
#define PWM6_CFG_REG2(base_va)        HI_IO_PWMI_ADDRESS(base_va, PWMI_ADRESS_BASE + 0x00C8)
#define PWM6_CTRL_REG(base_va)        HI_IO_PWMI_ADDRESS(base_va, PWMI_ADRESS_BASE + 0x00CC)
#define PWM6_STATE_REG0(base_va)      HI_IO_PWMI_ADDRESS(base_va, PWMI_ADRESS_BASE + 0x00D0)
#define PWM6_STATE_REG1(base_va)      HI_IO_PWMI_ADDRESS(base_va, PWMI_ADRESS_BASE + 0x00D4)
#define PWM6_STATE_REG2(base_va)      HI_IO_PWMI_ADDRESS(base_va, PWMI_ADRESS_BASE + 0x00D8)
                     
#define PWM7_CFG_REG0(base_va)        HI_IO_PWMI_ADDRESS(base_va, PWMI_ADRESS_BASE + 0x00E0)
#define PWM7_CFG_REG1(base_va)        HI_IO_PWMI_ADDRESS(base_va, PWMI_ADRESS_BASE + 0x00E4)
#define PWM7_CFG_REG2(base_va)        HI_IO_PWMI_ADDRESS(base_va, PWMI_ADRESS_BASE + 0x00E8)
#define PWM7_CTRL_REG(base_va)        HI_IO_PWMI_ADDRESS(base_va, PWMI_ADRESS_BASE + 0x00EC)
#define PWM7_STATE_REG0(base_va)      HI_IO_PWMI_ADDRESS(base_va, PWMI_ADRESS_BASE + 0x00F0)
#define PWM7_STATE_REG1(base_va)      HI_IO_PWMI_ADDRESS(base_va, PWMI_ADRESS_BASE + 0x00F4)
#define PWM7_STATE_REG2(base_va)      HI_IO_PWMI_ADDRESS(base_va, PWMI_ADRESS_BASE + 0x00F8)

//PWM
#define PWM_NUM_MAX    0x08

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /*_PWM_ARCH_H*/



