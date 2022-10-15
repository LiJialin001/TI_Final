/*
 * flag.c
 *
 *  Created on: 2022Äê10ÔÂ14ÈÕ
 *      Author: Administrator
 */

#include "flag.h"

void flag_init(void)
{
    GPIO_setAsPeripheralModuleFunctionInputPin(
               GPIO_PORT_P3,
               GPIO_PIN6
               );
    GPIO_setAsPeripheralModuleFunctionInputPin(
                   GPIO_PORT_P3,
                   GPIO_PIN2
                   );
    GPIO_setAsPeripheralModuleFunctionInputPin(
                   GPIO_PORT_P2,
                   GPIO_PIN7
                   );
    GPIO_setAsPeripheralModuleFunctionInputPin(
                   GPIO_PORT_P2,
                   GPIO_PIN0
                   );
    GPIO_setAsPeripheralModuleFunctionInputPin(
                   GPIO_PORT_P2,
                   GPIO_PIN2
                   );
}



