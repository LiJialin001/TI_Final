/* --COPYRIGHT--,BSD
 * Copyright (c) 2017, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --/COPYRIGHT--*/

#include "driverlib.h"
#include "motor.h"
#include "timer.h"
#include "delay.h"
#include "control.h"
#include "flag.h"


//******************************************************************************
//!
//!   Empty Project that includes driverlib
//!
//******************************************************************************


// Motor [left, right]

void main (void)
{
//    uint16_t u16CVol;
    WDT_A_hold(WDT_A_BASE);

    uint16_t left, right;

    // 初始化

    _DINT();

    System_Clock_Init();
    Motor_Init();
    flag_init();

    _EINT();

    // 检测充电完毕
    while(1)
    {
        if (!GPIO_getInputPinValue(GPIO_PORT_P3, GPIO_PIN6))
        {
            // 防止误判
            delay_ms(100);
            {
                if (!GPIO_getInputPinValue(GPIO_PORT_P3, GPIO_PIN6))
                {
                    break;
                }
            }
        }
    }

    left = 300;
    right = 300;

    // 从转弯处开始
    UpdateMotor(left, right, 0, 0);

    delay_ms(300);

    if (GPIO_getInputPinValue(GPIO_PORT_P2, GPIO_PIN2) && !GPIO_getInputPinValue(GPIO_PORT_P3, GPIO_PIN0))
    {
        left += 300;
        right -= 200;
        UpdateMotor(left, right, 0, 0);
    } else if (!GPIO_getInputPinValue(GPIO_PORT_P2, GPIO_PIN2) && GPIO_getInputPinValue(GPIO_PORT_P3, GPIO_PIN0))
    {
        left -= 100;
        right += 200;
        UpdateMotor(left, right, 0, 0);
    }


    while(1)
    {
        // 依据电容电压判断速度
        if (!GPIO_getInputPinValue(GPIO_PORT_P3, GPIO_PIN2))
        {
            left = 300;
            right = 300;
            UpdateMotor(left, right, 0, 0);
            if (!GPIO_getInputPinValue(GPIO_PORT_P2, GPIO_PIN2) && !GPIO_getInputPinValue(GPIO_PORT_P2, GPIO_PIN0))
            {
                left += 300;
                right -= 200;
                UpdateMotor(left, right, 0, 0);
            } else if (!GPIO_getInputPinValue(GPIO_PORT_P2, GPIO_PIN2) && GPIO_getInputPinValue(GPIO_PORT_P2, GPIO_PIN0))
            {
                left -= 100;
                right += 200;
                UpdateMotor(left, right, 0, 0);
            } else {
                delay_ms(20);
            }
        } else {
            left = 998;
            right = 998;
            UpdateMotor(left, right, 0, 0);
            if (GPIO_getInputPinValue(GPIO_PORT_P2, GPIO_PIN2) && !GPIO_getInputPinValue(GPIO_PORT_P2, GPIO_PIN0))
            {
                right = 0;
                UpdateMotor(left, right, 0, 0);
            } else if (!GPIO_getInputPinValue(GPIO_PORT_P2, GPIO_PIN2) && GPIO_getInputPinValue(GPIO_PORT_P2, GPIO_PIN0))
            {
                left = 0;
                UpdateMotor(left, right, 0, 0);
            } else {
                delay_ms(20);
            }
        }

    }



}

