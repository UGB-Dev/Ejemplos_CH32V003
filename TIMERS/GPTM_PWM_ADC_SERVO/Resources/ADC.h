/*

    ADC.h

*/

#ifndef __ADC_H__
#define __ADC_H__

#ifdef __cplusplus
 extern "C" {
#endif

#include "ch32v00x.h"

void ADC_Init_Simple(void);
uint16_t ADC_Read_Simple(void);

#ifdef __cplusplus
 }
#endif //__ADC_H__
#endif