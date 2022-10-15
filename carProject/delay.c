#include "delay.h"


void delay_ms(uint32_t ms)
{
    int i = 0;
    for(i = 0; i<ms; i++)
        __delay_cycles(24000);
}
