/*

    TIM1_PWM.h

*/

#ifndef __TIM1_PWM_H__
#define __TIM1_PWM_H__

#ifdef __cplusplus
 extern "C" {
#endif

#include "ch32v00x.h"

#define PWM1_MODE 0
#define PWM2_MODE 1

void PWM_Init(uint8_t MODE);
void PWM_Set(uint16_t Freq, uint8_t Duty_Cycle, uint32_t Base_Time);


#ifdef __cplusplus
 }
#endif //__TIM1_PWM_H__
#endif
