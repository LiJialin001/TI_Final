#include "driverlib.h"
#include "motor.h"
#include "timer.h"
#include "delay.h"
#include "control.h"
#include "flag.h"



volatile uint16_t results[2];


void main (void)
{
    WDT_A_hold(WDT_A_BASE);

    uint16_t left, right;

    // 初始化

    _DINT();

    System_Clock_Init();
    Motor_Init();
    flag_init();

    //P6.0 P6.1 ADC option select
    GPIO_setAsPeripheralModuleFunctionInputPin(
            GPIO_PORT_P6,
            GPIO_PIN0 + GPIO_PIN1
            );


    ADC12_A_init(ADC12_A_BASE,
            ADC12_A_SAMPLEHOLDSOURCE_SC,
            ADC12_A_CLOCKSOURCE_ADC12OSC,
            ADC12_A_CLOCKDIVIDER_1);

    ADC12_A_enable(ADC12_A_BASE);

    ADC12_A_setupSamplingTimer(ADC12_A_BASE,
            ADC12_A_CYCLEHOLD_64_CYCLES,
            ADC12_A_CYCLEHOLD_4_CYCLES,
            ADC12_A_MULTIPLESAMPLESDISABLE);

    ADC12_A_configureMemoryParam param = {0};
    param.memoryBufferControlIndex = ADC12_A_MEMORY_0;
    param.inputSourceSelect = ADC12_A_INPUT_A0;
    param.positiveRefVoltageSourceSelect = ADC12_A_VREFPOS_AVCC;
    param.negativeRefVoltageSourceSelect = ADC12_A_VREFNEG_AVSS;
    param.endOfSequence = ADC12_A_NOTENDOFSEQUENCE;
    ADC12_A_configureMemory(ADC12_A_BASE ,&param);

    ADC12_A_configureMemoryParam param_1 = {0};
    param_1.memoryBufferControlIndex = ADC12_A_MEMORY_1;
    param_1.inputSourceSelect = ADC12_A_INPUT_A1;
    param_1.positiveRefVoltageSourceSelect = ADC12_A_VREFPOS_AVCC;
    param_1.negativeRefVoltageSourceSelect = ADC12_A_VREFNEG_AVSS;
    param_1.endOfSequence = ADC12_A_NOTENDOFSEQUENCE;
    ADC12_A_configureMemory(ADC12_A_BASE ,&param_1);

    ADC12_A_clearInterrupt(ADC12_A_BASE,
            ADC12IE1);
    ADC12_A_enableInterrupt(ADC12_A_BASE,
            ADC12IE1);

    _EINT();

    GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);

    GPIO_setOutputHighOnPin(GPIO_PORT_P4, GPIO_PIN7);


    // 检测充电完毕（充电模式）
    while(1)
    {
        // 采样充电电位
        ADC12_A_startConversion(ADC12_A_BASE,
                ADC12_A_MEMORY_0,
                ADC12_A_SEQOFCHANNELS);

        //LPM0, ADC12_A_ISR will force exit
//        __bis_SR_register(LPM4_bits + GIE);
        //for Debugger
        __no_operation();

        if (ADC12_A_getResults(ADC12_A_BASE,
                               ADC12_A_MEMORY_0) <= 0x005)               // ToDo : 调节充电结束标志阈值
        {
            // 防止误判
            delay_ms(100);
            ADC12_A_startConversion(ADC12_A_BASE,
                    ADC12_A_MEMORY_0,
                    ADC12_A_SEQOFCHANNELS);

            //LPM0, ADC12_A_ISR will force exit
//            __bis_SR_register(LPM4_bits + GIE);        // ToDo: 测试低功耗模式
            //for Debugger
            __no_operation();
            if (ADC12_A_getResults(ADC12_A_BASE,
                                    ADC12_A_MEMORY_0) <= 0x00f)
            {
                GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0);   // ToDo: 比赛时换成P4.7
                break;
            }
        }
    }

    // 传感器上电
    GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN3);
    GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN6);

    // ToDo: 传感器pwm测试
//    UpdateMotor(0, 0, 500, 500);

    //LPM0, ADC12_A_ISR will force exit
//        __bis_SR_register(LPM4_bits + GIE);
    //for Debugger
    __no_operation();


    // 电量较高模式
    while(1)
    {
        // ToDo: 是否每n个周期才一次ADC以节省能耗
        // 采样电容电位
        ADC12_A_startConversion(ADC12_A_BASE,
                ADC12_A_MEMORY_1,
                ADC12_A_SEQOFCHANNELS);

        if (ADC12_A_getResults(ADC12_A_BASE,
                                ADC12_A_MEMORY_1) >= 0x00f)
        {
            if (GPIO_getInputPinValue(GPIO_PORT_P2, GPIO_PIN2) && !GPIO_getInputPinValue(GPIO_PORT_P3, GPIO_PIN0))
            {
                left = 0.8*ADC12_A_getResults(ADC12_A_BASE,
                                              ADC12_A_MEMORY_1);
                right = 0.1*ADC12_A_getResults(ADC12_A_BASE,
                                               ADC12_A_MEMORY_1);
            } else if (!GPIO_getInputPinValue(GPIO_PORT_P2, GPIO_PIN2) && GPIO_getInputPinValue(GPIO_PORT_P3, GPIO_PIN0))
            {
                left = 0.1*ADC12_A_getResults(ADC12_A_BASE,
                                              ADC12_A_MEMORY_1);
                right = 0.8*ADC12_A_getResults(ADC12_A_BASE,
                                               ADC12_A_MEMORY_1);
            } else {
                left = 0.5*ADC12_A_getResults(ADC12_A_BASE,
                                              ADC12_A_MEMORY_1);
                right = 0.5*ADC12_A_getResults(ADC12_A_BASE,
                                               ADC12_A_MEMORY_1);
            }
             UpdateMotor(left, right, 0, 0);
        } else {
            break;
        }
    }


    // 电量较低模式
    while(1)
    {
        // 传感器上电
        GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN3);
        GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN6);

        GPIO_setAsOutputPin(
                       GPIO_PORT_P1,
                       GPIO_PIN2 + GPIO_PIN3
                       );
        GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN2);
        GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN3);

//            left = 998;
//            right = 998;
//            UpdateMotor(left, right, 0, 0);
        if (GPIO_getInputPinValue(GPIO_PORT_P2, GPIO_PIN2) && !GPIO_getInputPinValue(GPIO_PORT_P2, GPIO_PIN0))
        {
//                right = 0;
//                UpdateMotor(left, right, 0, 0);
            GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN2);
            GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN3);
        } else if (!GPIO_getInputPinValue(GPIO_PORT_P2, GPIO_PIN2) && GPIO_getInputPinValue(GPIO_PORT_P2, GPIO_PIN0))
        {
//                left = 0;
//                UpdateMotor(left, right, 0, 0);
            GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN2);
            GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN3);
        } else {
            GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN2);
            GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN3);
            // 传感器断电
//                GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN3);
//                GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN6);
//                delay_ms(20);
        }






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
                // 传感器断电
//                GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN3);
//                GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN6);
//                delay_ms(20);
            }
        } else {

        }

    }


}



#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=ADC12_VECTOR
__interrupt
#elif defined(__GNUC__)
__attribute__((interrupt(ADC12_VECTOR)))
#endif
void ADC12ISR (void)
{
    switch (__even_in_range(ADC12IV,34)){
        case  0: break;   //Vector  0:  No interrupt
        case  2: break;   //Vector  2:  ADC overflow
        case  4: break;   //Vector  4:  ADC timing overflow
        case  6: break;         //Vector  6:  ADC12IFG0
        case  8:   //Vector  8:  ADC12IFG1
            //Move results, IFG is cleared
            results[0] =
                ADC12_A_getResults(ADC12_A_BASE,
                    ADC12_A_MEMORY_0);
            //Move results, IFG is cleared
            results[1] =
                ADC12_A_getResults(ADC12_A_BASE,
                    ADC12_A_MEMORY_1);
            //Exit active CPU, SET BREAKPOINT HERE
            __bic_SR_register_on_exit(LPM4_bits);
        case 10: break;   //Vector 10:  ADC12IFG2
        case 12: break;   //Vector 12:  ADC12IFG3
        case 14: break;   //Vector 14:  ADC12IFG4
        case 16: break;   //Vector 16:  ADC12IFG5
        case 18: break;   //Vector 18:  ADC12IFG6
        case 20: break;   //Vector 20:  ADC12IFG7
        case 22: break;   //Vector 22:  ADC12IFG8
        case 24: break;   //Vector 24:  ADC12IFG9
        case 26: break;   //Vector 26:  ADC12IFG10
        case 28: break;   //Vector 28:  ADC12IFG11
        case 30: break;   //Vector 30:  ADC12IFG12
        case 32: break;   //Vector 32:  ADC12IFG13
        case 34: break;   //Vector 34:  ADC12IFG14
        default: break;
    }
}

