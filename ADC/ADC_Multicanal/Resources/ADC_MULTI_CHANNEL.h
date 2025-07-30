/*

    ADC_MULTI_CHANNEL.h

*/

#ifndef __ADC_MULTI_CHANNEL_H__
#define __ADC_MULTI_CHANNEL_H__

#ifdef __cplusplus
 extern "C" {
#endif

#include "ch32v00x.h"

extern uint16_t ADC_VALUE[2];

// FUNCIONES PROTOTIPO
void ADC_Multi_Channel_Init(void);
void ADC_Multi_Channel_Read(void);

#ifdef __cplusplus
 }
#endif //__ADC_MULTI_CHANNEL_H__
#endif