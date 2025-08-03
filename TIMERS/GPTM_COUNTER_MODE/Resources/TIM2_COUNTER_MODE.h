/*

    TIM2_COUNTER_MODE.h

*/

#ifndef __TIM2_COUNTER_MODE_H__
#define __TIM2_COUNTER_MODE_H__

#ifdef __cplusplus
 extern "C" {
#endif

#include "ch32v00x.h"
#include <stdbool.h>

#define Flanco_Bajada 1
#define Flanco_Subida 0

void Counter_Mode_Init(void);
void Set_Cuenta(uint16_t Ncuenta, bool Edge);


#ifdef __cplusplus
 }
#endif //__TIM2_COUNTER_MODE_H__
#endif