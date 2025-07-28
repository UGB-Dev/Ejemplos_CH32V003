/*

    TIM1_INPUT_CAPTURE.h

*/

#ifndef __TIM1_INPUT_CAPTURE_H__
#define __TIM1_INPUT_CAPTURE_H__

#ifdef __cplusplus
 extern "C" {
#endif

#include "ch32v00x.h"
#include <stdbool.h>

#define Flanco_Bajada 1
#define Flanco_Subida 0

void Input_Capture_Init(void);
void Set_Cuenta(uint16_t Ncuenta, bool Edge);


#ifdef __cplusplus
 }
#endif //__TIM1_INPUT_CAPTURE_H__
#endif