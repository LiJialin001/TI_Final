/*
 * adc.c
 *
 *  Created on: 2022Äê10ÔÂ13ÈÕ
 *      Author: Administrator
 */

#include "adc.h"
#include "include.h"



void adcInit(void)
{
    //P6.0 ADC option select
       GPIO_setAsPeripheralModuleFunctionInputPin(
               GPIO_PORT_P6,
               GPIO_PIN0
               );

   //Initialize the ADC12_A Module
      /*
       * Base address of ADC12_A Module
       * Use internal ADC12_A bit as sample/hold signal to start conversion
       * USE MODOSC 5MHZ Digital Oscillator as clock source
       * Use default clock divider of 1
       */
      ADC12_A_init(ADC12_A_BASE,
              ADC12_A_SAMPLEHOLDSOURCE_SC,
              ADC12_A_CLOCKSOURCE_ADC12OSC,
              ADC12_A_CLOCKDIVIDER_1);

      ADC12_A_enable(ADC12_A_BASE);

      /*
       * Base address of ADC12_A Module
       * For memory buffers 0-7 sample/hold for 64 clock cycles
       * For memory buffers 8-15 sample/hold for 4 clock cycles (default)
       * Disable Multiple Sampling
       */
      ADC12_A_setupSamplingTimer(ADC12_A_BASE,
              ADC12_A_CYCLEHOLD_64_CYCLES,
              ADC12_A_CYCLEHOLD_4_CYCLES,
              ADC12_A_MULTIPLESAMPLESDISABLE);

      //Configure Memory Buffer
      /*
       * Base address of the ADC12_A Module
       * Configure memory buffer 0
       * Map input A0 to memory buffer 0
       * Vref+ = AVcc
       * Vr- = AVss
       * Memory buffer 0 is not the end of a sequence
       */
      ADC12_A_configureMemoryParam param = {0};
      param.memoryBufferControlIndex = ADC12_A_MEMORY_0;
      param.inputSourceSelect = ADC12_A_INPUT_A0;
      param.positiveRefVoltageSourceSelect = ADC12_A_VREFPOS_AVCC;
      param.negativeRefVoltageSourceSelect = ADC12_A_VREFNEG_AVSS;
      param.endOfSequence = ADC12_A_NOTENDOFSEQUENCE;
      ADC12_A_configureMemory(ADC12_A_BASE ,&param);

  //Enable memory buffer 0 interrupt
      ADC12_A_clearInterrupt(ADC12_A_BASE,
              ADC12IFG0);
      ADC12_A_enableInterrupt(ADC12_A_BASE,
              ADC12IE0);


}
