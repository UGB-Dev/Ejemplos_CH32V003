/*

    I2C_SOFTWARE.h

*/

#ifndef __I2C_SOFTWARE_H__
#define __I2C_SOFTWARE_H__

#ifdef __cplusplus
 extern "C" {
#endif

#include "ch32v00x.h"
#include <stdbool.h>

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

#define SCL_ON()  GPIOC -> CFGLR &= ~(0xF<<28); \
                  GPIOC -> CFGLR |= GPIO_CFGLR_CNF7_0;

#define SCL_OFF() GPIOC -> CFGLR &= ~(0xF<<28); \
                  GPIOC -> CFGLR |= GPIO_CFGLR_MODE7; 

#define SDA_ON()  GPIOC -> CFGLR &= ~(0xF<<24); \
                  GPIOC -> CFGLR |= GPIO_CFGLR_CNF6_0;

#define SDA_OFF() GPIOC -> CFGLR &= ~(0xF<<24); \
                  GPIOC -> CFGLR |= GPIO_CFGLR_MODE6; 


extern volatile uint8_t DAT_Copia, DAT_Pos; // variables de control para envio de datos
extern volatile bool RECONOCIMIENTO; // ACK(valor en 0) o NACK(valor en 1)
extern uint32_t SCL_Frec;
extern void (*Trama_Protocolo)(void);

/*  FUNCIONES PROTOTIPO */
void I2C_SOFT_Init(uint32_t Frecuencia);
void SysTick_Handler() __attribute__((interrupt("WCH-Interrupt-fast")));
void SysTick_Enable(void);
void SysTick_Disable(void);
void I2C_SOFT_Enviar(uint8_t DAT);
uint8_t I2C_SOFT_Leer(bool ACK);
void Inicio_Trama(void);
void Fin_Trama(void);
void Trama_I2C_Enviar(void);
void Trama_I2C_Leer(void);

#ifdef __cplusplus
 }
#endif //__I2C_SOFTWARE_H__
#endif