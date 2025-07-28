/*

    TIM1_OUTPUT_COMPARE.h

*/

#ifndef __TIM1_OUTPUT_COMPARE_H__
#define __TIM1_OUTPUT_COMPARE_H__

#ifdef __cplusplus
 extern "C" {
#endif

#include "ch32v00x.h"

#define FLIP_MODE 0
#define PWM1_MODE 1
#define PWM2_MODE 2

void Output_Compare_Init(uint8_t MODE);
void PWM_Set(uint16_t Freq, uint8_t Duty_Cycle, uint32_t Base_Time);


#ifdef __cplusplus
 }
#endif //__TIM1_OUTPUT_COMPARE_H__
#endif