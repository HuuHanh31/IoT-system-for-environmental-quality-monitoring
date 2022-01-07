#ifndef _ADC_H_
#define _ADC_H_

#include <p18f4620.h>

#define ADC_CHANNEL 	13

void init_adc(void);
int get_adc_value();

extern unsigned int adc_value[ADC_CHANNEL];
extern unsigned int state[3];
#endif