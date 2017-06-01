#ifndef __ADC_H
#define	__ADC_H


#include "stm32f0xx.h"
void filter();
void Adc_Init();

#define    macNOFCHANEL											6


#define    macADCx                          ADC1
#define    macADC_DMA_CHANNEL               DMA1_Channel1


/**************************º¯ÊýÉùÃ÷********************************/
void               ADCx_Init                               (void);


#endif /* __ADC_H */

