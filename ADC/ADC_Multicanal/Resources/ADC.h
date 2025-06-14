/*

    ADC.h

*/

#ifndef __ADC_H__
#define __ADC_H__

#ifdef __cplusplus
 extern "C" {
#endif

#include "ch32v00x.h"

extern uint16_t ADC_VALUE[2];

// FUNCIONES PROTOTIPO
void ADC_Init_Multiple(void);
void ADC_Read_Multiple(void);


#ifdef __cplusplus
 }
#endif //__ADC_H__
#endif
