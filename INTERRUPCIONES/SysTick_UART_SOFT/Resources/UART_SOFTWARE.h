/*

    UART_SOFTWARE.h

*/

#ifndef __UART_SOFTWARE_H__
#define __UART_SOFTWARE_H__

#ifdef __cplusplus
 extern "C" {
#endif

#include "ch32v00x.h"
//#include <stdint.h>

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
#define UART_H (GPIOC -> BSHR = GPIO_BSHR_BS7)
#define UART_L (GPIOC -> BSHR = GPIO_BSHR_BR7)

extern volatile uint8_t DAT_Copia=0, DAT_Pos=0; 
extern uint32_t Baud_Rate=0;

/*  FUNCIONES PROTOTIPO */
void UART_SOFT_Init(uint32_t Bauds);
void SysTick_Handler() __attribute__((interrupt("WCH-Interrupt-fast")));
void SysTick_Enable(void);
void SysTick_Disable(void);
void UART_Char(uint8_t DAT);
void UART_Str(const char* DAT);

#ifdef __cplusplus
 }
#endif //__UART_SOFTWARE_H__
#endif
