/*----------------------------------------------------------------------------
 * Copyright (c) <2016-2018>, <Huawei Technologies Co., Ltd>
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice, this list of
 * conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list
 * of conditions and the following disclaimer in the documentation and/or other materials
 * provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used
 * to endorse or promote products derived from this software without specific prior written
 * permission.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *---------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
 * Notice of Export Control Law
 * ===============================================
 * Huawei LiteOS may be subject to applicable export control laws and regulations, which might
 * include those applicable to Huawei LiteOS of U.S. and the country in which you are located.
 * Import, export and usage of Huawei LiteOS in any manner by you shall be in compliance with such
 * applicable export control laws and regulations.
 *---------------------------------------------------------------------------*/

#include "sys_init.h"

#include <shell.h>
#include <osport.h>

#include <mem.h>

#if defined (__CC_ARM)
extern char Image$$RW_IRAM1$$ZI$$Limit [];
extern char Image$$ARM_LIB_STACKHEAP$$Base [];
#elif defined (__GNUC__)
extern char __los_heap_addr_start__ [];
extern char __los_heap_addr_end__ [];
#else
#error "fix me"
#endif

const struct phys_mem system_phys_mem [] =
    {
#if defined (__CC_ARM)
        { Image$$RW_IRAM1$$ZI$$Limit, Image$$ARM_LIB_STACKHEAP$$Base },
#elif defined (__GNUC__)
        { (unsigned long)__los_heap_addr_start__,(unsigned long) __los_heap_addr_end__ },
#else
#error "unsupported tool!"
#endif
        { 0, 0 }
    };

VOID HardWare_Init(VOID)
{
    HAL_Init();
    /* Configure the system clock */
    SystemClock_Config();
    MX_GPIO_Init();
    MX_I2C1_Init();
    MX_USART1_UART_Init();
    MX_SPI2_Init();
    dwt_delay_init(SystemCoreClock);
    LCD_Init();
    LCD_Clear(WHITE);
    POINT_COLOR = RED;
    LCD_ShowString(10, 50, 240, 24, 24, "Welcome to IoTCluB!");
    LCD_ShowString(20, 90, 240, 16, 16, "BearPi IoT Develop Board");
    LCD_ShowString(20, 130, 240, 16, 16, "Powerd by Huawei LiteOS!");
    LCD_ShowString(10, 170, 240, 16, 16, "Please wait for system init");
}
int main(void)
{
    UINT32 uwRet = LOS_OK;

    //do the hardware initialize
    HardWare_Init();

    //do the liteos kernel initialize
    uwRet = LOS_KernelInit();
    if (uwRet != LOS_OK)
    {
        return LOS_NOK;
    }
    los_shell_init();     //which means you could use shell

    ////////////////////////APPLICATION INITIALIZE END///////////////////
    extern int app_init();
    app_init();
    //start the system
    LOS_Start();
}
