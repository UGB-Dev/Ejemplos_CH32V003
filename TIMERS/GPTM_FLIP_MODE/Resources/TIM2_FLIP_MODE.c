/*

    TIM2_FLIP_MODE.c

*/

#include "TIM2_FLIP_MODE.h"

void Flip_Mode_Init(void){
    /*CONFIGURACION DE PERIFERICOS */
    RCC -> APB2PCENR |= RCC_IOPDEN; // Habilitacion del Puerto D
    RCC -> APB1PCENR |= RCC_TIM2EN; // Habilitacion del Timer 2
    
    /* CONFIGURACION DEL PIN PD3 */
    GPIOD -> CFGLR &= ~(GPIO_CFGLR_MODE3 | GPIO_CFGLR_CNF3); // Borra configuraciones iniciales
    GPIOD -> CFGLR |= GPIO_CFGLR_MODE3 | GPIO_CFGLR_CNF3_1; // PD3 como salida en Push-Pull a 30 MHz

/*
     --------      -----      ---------       ------      ---------      -------------------      -------------      --------     
    | SOURCE |----| DIV |----| PRESCAL |-----| TIME |----| SELECT  |----| ACTIVE OUTPUT AND |----| ACTIVE MAIN |----| ACTIVE |----### TIMxCHx or
    |  CLK   |----| CLK |----|   CNT   |-----| (T)  |----| MODE OC |----| LEVEL ADJUSTMENT  |----|    OUTPUT   |----|  CNT   |----### TIMxCHxN
     --------      -----      ---------       ------      ---------      -------------------      -------------      --------
      SMCFGR       CTLR1         PSC           ATRLR       CHCTLR1               CCER                 BDTR             CTLR1           
       SMS=0       CKD=0    (Fclk/Fbase)-1     1 TO        OCxM_1 AND         CCxP=0 OR 1             MOE=1            CEN=1             
                                               65535       OCxM_0               CCxE=1                                                                                                  
*/

    /* CONFIGURACION DEL TIMER 2 */
    TIM2 -> SMCFGR &= ~TIM_SMS; // Internal clock 24 MHz
    TIM2 -> CTLR1 &= ~TIM_CKD_DIV4; // Sin divisor
    TIM2 -> PSC = 24000-1; // Base de tiempo a 1ms
    TIM2 -> ATRLR = 50; // 50 * 1 ms = 50 ms
    TIM2 -> CHCTLR1 |= TIM_OC2M_1 | TIM_OC2M_0; // Modo flip
    TIM2 -> CCER &= ~TIM_CC2P ; // Valor logico en OC2REF directo a la salida OC2(TIM2CH2)
    TIM2 -> CCER |=  TIM_CC2E; // Activa la salida en OC2(TIM2CH2)    

    /*
        PSC: Prescalador; registro que sirve como divisor de la frecuencia Fclk para la base de tiempo 
             del Timer1 o Timer2 segun sea el caso.
        ATRLR: Registro cuya funcion es almacenar el valor que determina el cambio de nivel logico despues
               que el contador TNC es igual a ATRLR.

        Senal:
             ___________
            |           |           |
            |           |           |
            |           |           |
            |           |___________|
            
            |-- ATRLR --|-- ATRLR --|
        
    
    /* ACTIVACION DE LAS SALIDAS DEL CANAL 2 */ 
    
    TIM2 -> BDTR |= TIM_MOE; // Se habilita la salida principal OC2(TIM2CH2)
    TIM2 -> CTLR1 |= TIM_CEN; // Se habilita el contador CNT
}

