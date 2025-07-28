/*

    TIM1_PULSE_MODE.h

*/

#ifndef __TIM1_PULSE_MODE_H__
#define __TIM1_PULSE_MODE_H__

#ifdef __cplusplus
 extern "C" {
#endif

#include "ch32v00x.h"
#include <stdbool.h>

#define Log_Pos 1
#define Log_Neg 0

void Pulse_Mod_Init(void);
void Set_Pulse( uint16_t Tdelay, uint16_t Tpulso, bool Polaridad);

#ifdef __cplusplus
 }
#endif //__TIM1_PULSE_MODE_H__
#endif