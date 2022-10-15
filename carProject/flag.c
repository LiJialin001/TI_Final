/*
 * flag.c
 *
 *  Created on: 2022年10月14日
 *      Author: Administrator
 */

#include "flag.h"

void flag_init(void)
{
    // 充好电
    GPIO_setAsPeripheralModuleFunctionInputPin(
               GPIO_PORT_P3,
               GPIO_PIN6
               );
    // 电量状态 1
    GPIO_setAsPeripheralModuleFunctionInputPin(
                   GPIO_PORT_P3,
                   GPIO_PIN2
                   );
    // 电量状态 2
    GPIO_setAsPeripheralModuleFunctionInputPin(
                   GPIO_PORT_P2,
                   GPIO_PIN7
                   );
    // 左传感
    GPIO_setAsPeripheralModuleFunctionInputPin(
                   GPIO_PORT_P2,
                   GPIO_PIN0
                   );
    // 右传感
    GPIO_setAsPeripheralModuleFunctionInputPin(
                   GPIO_PORT_P2,
                   GPIO_PIN2
                   );
    // 左传感供电
    GPIO_setAsOutputPin(
                   GPIO_PORT_P2,
                   GPIO_PIN6
                   );
    // 右传感供电
    GPIO_setAsOutputPin(
                   GPIO_PORT_P2,
                   GPIO_PIN3
                   );
    // 指示灯
    GPIO_setAsOutputPin(
                   GPIO_PORT_P1,
                   GPIO_PIN0
                   );
    GPIO_setAsOutputPin(
                   GPIO_PORT_P4,
                   GPIO_PIN7
                   );

}



