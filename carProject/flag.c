/*
 * flag.c
 *
 *  Created on: 2022��10��14��
 *      Author: Administrator
 */

#include "flag.h"

void flag_init(void)
{
    // ��õ�
    GPIO_setAsPeripheralModuleFunctionInputPin(
               GPIO_PORT_P3,
               GPIO_PIN6
               );
    // ����״̬ 1
    GPIO_setAsPeripheralModuleFunctionInputPin(
                   GPIO_PORT_P3,
                   GPIO_PIN2
                   );
    // ����״̬ 2
    GPIO_setAsPeripheralModuleFunctionInputPin(
                   GPIO_PORT_P2,
                   GPIO_PIN7
                   );
    // �󴫸�
    GPIO_setAsPeripheralModuleFunctionInputPin(
                   GPIO_PORT_P2,
                   GPIO_PIN0
                   );
    // �Ҵ���
    GPIO_setAsPeripheralModuleFunctionInputPin(
                   GPIO_PORT_P2,
                   GPIO_PIN2
                   );
    // �󴫸й���
    GPIO_setAsOutputPin(
                   GPIO_PORT_P2,
                   GPIO_PIN6
                   );
    // �Ҵ��й���
    GPIO_setAsOutputPin(
                   GPIO_PORT_P2,
                   GPIO_PIN3
                   );
    // ָʾ��
    GPIO_setAsOutputPin(
                   GPIO_PORT_P1,
                   GPIO_PIN0
                   );

    // A IN1
    GPIO_setAsOutputPin(
            GPIO_PORT_P6,
            GPIO_PIN4
            );
    // A IN2
    GPIO_setAsOutputPin(
            GPIO_PORT_P7,
            GPIO_PIN0
            );
    // B IN1
    GPIO_setAsOutputPin(
            GPIO_PORT_P3,
            GPIO_PIN6
            );
    // B IN2
    GPIO_setAsOutputPin(
            GPIO_PORT_P3,
            GPIO_PIN5
            );
    GPIO_setAsOutputPin(
                   GPIO_PORT_P4,
                   GPIO_PIN7
                   );

}



