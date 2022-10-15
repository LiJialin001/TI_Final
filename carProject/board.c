/*
 * board.c
 *
 *  Created on: 2022��10��13��
 *      Author: Administrator
 */

#include "board.h"
#include "include.h"
#include "ucs.h"

int64_t Clk_ACLK = 0;
int64_t Clk_SMCLK = 0;
int64_t Clk_MCLK = 0;


void System_Clock_Init(void)
{
    // Stop Watch Dog Timer
    WDT_A_hold(WDT_A_BASE);

    // XT1 Pin used as peripheral function for 32768 Hz
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P5, GPIO_PIN4 + GPIO_PIN5);

    // XT2 Pin used as peripheral function for 4 MHz
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P5, GPIO_PIN2 + GPIO_PIN3);

    //__bis_SR_register(GIE);

    // System Power Improve step by step
    PMM_setVCore(PMM_CORE_LEVEL_1);
    PMM_setVCore(PMM_CORE_LEVEL_2);
    PMM_setVCore(PMM_CORE_LEVEL_3);

    UCS_setExternalClockSource(32768, 4000000);

    // Turn on external crystal
    UCS_turnOnLFXT1(UCS_XT1_DRIVE_3, UCS_XCAP_3);
    UCS_turnOnXT2(UCS_XT2_DRIVE_4MHZ_8MHZ);

    // UCS_XT2CLK_SELECT: 4 MHz    UCS_VLOCLK_SELECT: 10 kHz    UCS_REFOCLK_SELECT: 32768 Hz
    // 4 000 000 / 8 = 500 000 = 500 kHz
    // Set PLL Clock source and prescale
    // ��ʵ�� ������໷ PLL ������Ƶ�ʴ��ڵ��� 1MHz Ҳ�ܹ�����ֻ��Ŀ��Ƶ�ʲ���ȷ������25M �õ� 24M��
    UCS_initClockSignal(UCS_FLLREF, UCS_XT2CLK_SELECT, UCS_CLOCK_DIVIDER_8);

    // Target PLL Frequency(kHz) and ratio
    // 25 000 000 / 500 000 = 50
    UCS_initFLLSettle(25000, 50);

    UCS_initClockSignal(UCS_ACLK, UCS_DCOCLK_SELECT, UCS_CLOCK_DIVIDER_1);
    /*
        If the frequency is greater than 16 MHz,
        the function sets the MCLK and SMCLK source to the undivided DCO frequency.
        Otherwise, the function sets the MCLK and SMCLK source to the DCOCLKDIV frequency.
    */
    UCS_initClockSignal(UCS_MCLK, UCS_DCOCLK_SELECT, UCS_CLOCK_DIVIDER_1);
    UCS_initClockSignal(UCS_SMCLK, UCS_DCOCLK_SELECT, UCS_CLOCK_DIVIDER_1);

    UCS_turnOnSMCLK();

    Clk_ACLK = UCS_getACLK();
    Clk_SMCLK = UCS_getSMCLK();
    Clk_MCLK = UCS_getMCLK();
}

