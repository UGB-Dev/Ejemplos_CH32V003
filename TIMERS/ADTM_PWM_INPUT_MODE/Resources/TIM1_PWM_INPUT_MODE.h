/*

    TIM1_PWM_INPUT_MODE.h

*/

#ifndef __TIM1_PWM_INPUT_MODE_H__
#define __TIM1_PWM_INPUT_MODE_H__

#ifdef __cplusplus
 extern "C" {
#endif

#include "ch32v00x.h"
#include <stdbool.h>

#define Log_Pos 1
#define Log_Neg 0

void PWM_Input_Mode_Init(void);
void Set_Pulse( uint16_t Tdelay, uint16_t Tpulso, bool Polaridad);

#ifdef __cplusplus
 }
#endif //__TIM1_PWM_INPUT_MODE_H__
#endif