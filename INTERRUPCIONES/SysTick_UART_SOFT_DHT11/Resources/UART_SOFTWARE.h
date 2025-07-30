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
                           SysTick -> CMP             Despeje                                 Fclk (SystemCoreClock)
        Tiempo_INT = -----------------------------     ====>       SysTick -> CMP = ------------------------------------------  
                       24 MHz / STK_CTLR_STLCK                                          STK_CTLR_STLCK * BAUDS(Freq interrupt)                                                

        donde:
                Tiempo_INT = Tiempo de interrupcion 
                SysTick -> CMP = Registro para el valor de comparacion de interrupcion
                STK_CTLR_STLCK = Prescalador, un 1 equivale a dividir por 1 y un 0 equivale a dividir por 8,
                                 este ultimo es el divisor por defecto
*/

#define VALUE_CMP(x) (SystemCoreClock/(x*8)) 
//#define VALUE_CMP(x) (SystemCoreClock/x) 
#define UART_H (GPIOC -> BSHR = GPIO_BSHR_BS1)
#define UART_L (GPIOC -> BSHR = GPIO_BSHR_BR1)

#define Systick_STRE ((uint32_t)0x00000008) /* Auto-Reload count enable bit */
#define SysTick_STLK ((uint32_t)0x00000004) /* Counter clock source selection bit */
#define Systick_STIE ((uint32_t)0x00000002) /* Counter interrupt enable control bit */
#define SysTick_STE ((uint32_t)0x00000001) /* System counter enable control bit */


extern volatile uint8_t DAT_Copia, DAT_Pos; 
extern uint32_t Baud_Rate;
extern void (*Trama_Protocolo)(void);

/*  FUNCIONES PROTOTIPO */
void UART_SOFT_Init(uint32_t Bauds);
void SysTick_Enable(void);
void SysTick_Disable(void);
void UART_SOFT_Char(uint8_t DAT);
void UART_SOFT_Str(const char* DAT);
void Trama_UART(void);
void SysTick_Handler() __attribute__((interrupt("WCH-Interrupt-fast")));

#ifdef __cplusplus
 }
#endif //__UART_SOFTWARE_H__
#endif
