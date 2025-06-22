/*

    Timer1_PWM.h

*/

#ifndef __TIMER1_PWM_H__
#define __TIMER1_PWM_H__

#ifdef __cplusplus
 extern "C" {
#endif

#include "ch32v00x.h"

void PWM_Init(void);
void PWM_Set(uint16_t Freq, uint8_t Duty_Cycle, uint32_t Base_Time);


#ifdef __cplusplus
 }
#endif //__TIMER1_PWM_H__
#endif