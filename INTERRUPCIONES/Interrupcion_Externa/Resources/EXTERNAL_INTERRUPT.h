/*

    EXTERNAL_INTERRUPT.h

*/

#ifndef __EXTERNAL_INTERRUPT_H__
#define __EXTERNAL_INTERRUPT_H__

#ifdef __cplusplus
 extern "C" {
#endif

#include "ch32v00x.h"
#include <stdbool.h>

/*  VARIABLES GLOBALES  */
extern volatile uint8_t Interupcion;
extern const u_int8_t Secuencia1[];
extern const u_int8_t Secuencia2[];
extern const u_int8_t Secuencia3[];

/*  PROTOTIPO DE FUNCION  */
void EXTI7_0_IRQHandler(void) __attribute__((interrupt));
void Extern_Interrupt_Init(void);

#ifdef __cplusplus
 }
#endif //__EXTERNAL_INTERRUPT_H__
#endif