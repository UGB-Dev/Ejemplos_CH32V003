/*

    TIM1_FLIP_MODE.c

*/

#include "TIM1_FLIP_MODE.h"

void Flip_Mode_Init(void){
    /*CONFIGURACION DE PERIFERICOS */
    RCC -> APB2PCENR |=  (RCC_IOPAEN | RCC_TIM1EN); // Habilitcion de reloj en Puerto A y Timer 1
    
    /* CONFIGURACION DEL PIN PA1 */
    GPIOA -> CFGLR &= ~(GPIO_CFGLR_MODE1 | GPIO_CFGLR_CNF1); // Borra configuraciones iniciales
    GPIOA -> CFGLR |= GPIO_CFGLR_MODE1 | GPIO_CFGLR_CNF1_1; // PA1 como salida en Push-Pull  a 30 MHz

/*
     --------      -----      ---------       ------      ---------      -------------------      -------------      --------     
    | SOURCE |----| DIV |----| PRESCAL |-----| TIME |----| SELECT  |----| ACTIVE OUTPUT AND |----| ACTIVE MAIN |----| ACTIVE |----### TIMxCHx or
    |  CLK   |----| CLK |----|   CNT   |-----| (T)  |----| MODE OC |----| LEVEL ADJUSTMENT  |----|    OUTPUT   |----|  CNT   |----### TIMxCHxN
     --------      -----      ---------       ------      ---------      -------------------      -------------      --------
      SMCFGR       CTLR1         PSC           ATRLR       CHCTLR1               CCER                 BDTR             CTLR1           
       SMS=0       CKD=0    (Fclk/Fbase)-1     1 TO        OCxM_1 AND         CCxP=0 OR 1             MOE=1            CEN=1             
                                               65535       OCxM_0               CCxE=1                                                                                                  
*/

    /* CONFIGURACION DEL TIMER 1 */
    TIM1 -> SMCFGR &= ~TIM_SMS; // Internal clock 24 MHz
    TIM1 -> CTLR1 &= ~TIM_CKD_DIV4; // Sin divisor
    TIM1 -> PSC = 24000-1; // Base de tiempo a 1ms
    TIM1 -> ATRLR = 50; // 50 * 1 ms = 50 ms
    TIM1 -> CHCTLR1 |= TIM_OC2M_1 | TIM_OC2M_0; // Modo flip
    TIM1 -> CCER &= ~TIM_CC2P ; // Valor logico en OC2REF directo a la salida OC2(TIM1CH2)
    TIM1 -> CCER |=  TIM_CC2E; // Activa la salida en OC2(TIM1CH2)    

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
    
    TIM1 -> BDTR |= TIM_MOE; // Se habilita la salida principal OC2(TIM1CH2)
    TIM1 -> CTLR1 |= TIM_CEN; // Se habilita el contador CNT
}

