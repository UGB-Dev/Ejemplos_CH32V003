/*

    UART_SOFTWARE.h

*/

#ifndef __UART_SOFTWARE_H__
#define __UART_SOFTWARE_H__

#ifdef __cplusplus
 extern "C" {
#endif

#include "ch32v00x.h"

/* 
                           SysTick -> CMP
        Tiempo_INT = -----------------------------  
                       24 MHz / STK_CTLR_STLCK

        donde:
                Tiempo_INT = Tiempo de interrupcion 
                SysTick -> CMP = registro para el valor de comparacion de interrupcion
                STK_CTLR_STLCK = divisor de frecuencia un 1 equivale a dividir por 1 y
                                 un 0 equivale a dividir por 8
*/

#define VALUE_CMP_ms(x) ((SystemCoreClock/8000 )*x ) 
#define VALUE_CMP_us(x) ((SystemCoreClock/8000000 )*x ) 
#define UART_H (GPIOC -> BSHR = GPIO_BSHR_BS5)
#define UART_L (GPIOC -> BSHR = GPIO_BSHR_BR5)

/* PROTOTIPO DE VARIABLES GLOBALES */
extern volatile uint8_t DAT_Copia, DAT_Pos; // variables de control para envio de datos
extern uint32_t Baud_Rate;
extern void (*Trama_Protocolo)(void);

/*  FUNCIONES PROTOTIPO */
void UART_SOFT_Init(uint32_t Bauds);
void SysTick_Enable(uint32_t CMP_);
void SysTick_Disable(void);
void UART_SOFT_Char(uint8_t DAT);
void UART_SOFT_Str(const char* DAT);
void Trama_UART(void);

#ifdef __cplusplus
 }
#endif //__UART_SOFTWARE_H__
#endif
