/*

    TIM1_PWM.c

*/

#include "TIM1_PWM.h"

void PWM_Init(uint8_t MODE){
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
                                               65535       OCxM_2               CCxE=1                                                                                                  
*/

    /* CONFIGURACION DEL TIMER 1 */
    TIM1 -> SMCFGR &= ~TIM_SMS; // Internal clock 24 MHz
    TIM1 -> CTLR1 &= ~TIM_CKD_DIV4; // Sin divisor
    TIM1 -> PSC = 24000-1; // Base de tiempo a 1ms
    TIM1 -> ATRLR = 500; // 1000 * 1 ms = 500 ms
    TIM1 -> CH2CVR = 250; // 50 % duty cycle canal 2
    
    if (MODE == PWM1_MODE) {
        TIM1 -> CHCTLR1 |=  TIM_OC2M_2 | TIM_OC2M_1 ; // PWM en modo 1
    }
    else if (MODE == PWM2_MODE) {
        TIM1 -> CHCTLR1 |=  TIM_OC2M_2 | TIM_OC2M_1 | TIM_OC2M_0 ; // PWM en modo 2
    }
    
    TIM1 -> CCER &= ~TIM_CC2P; // Valor logico en OC2REF directo a la salida OC2(TIM1CH2)
    TIM1 -> CCER |=  TIM_CC2E; // Activa la salida en OC2(TIM1CH2)

    /*
        PSC: Prescalador; registro que sirve como divisor de la frecuencia Fclk para la base de tiempo 
             del Timer1 o Timer2 segun sea el caso.
        ATRLR: Registro cuya funcion es almacenar el valor para generar el periodo (T) de la senal PWM.
        CHxCVR: Registro cuya funcion es almacenar el tiempo en que la senal cambia de alto a bajo o 
                bien de bajo a alto segun sea el caso.

        Senal PWM:
             ___________
            |           |           |
            |           |           |
            |           |           |
            |           |___________|
            
            |--CHxCVR --|

            |-------- ATRLR --------|
    */

    /* ACTIVACION DE LAS SALIDAS DEL CANAL 2 */
    TIM1 -> BDTR |= TIM_MOE; // Se habilita la salida principal OC2(TIM1CH2)
    TIM1 -> CTLR1 |= TIM_CEN; // Se habilita el contador CNT
}

void PWM_Set(uint16_t Freq, uint8_t Duty_Cycle, uint32_t Base_Time){
    /*
                   1                                            PSC+1
        ATRLR = -------- = Base_Time * X     ;   Base_Time = ----------
                  Fpwm                                          Fclk

        
                    1                             Fclk
        X = -----------------   ==>     X = -----------------
                      (PSC+1)                 Fpwm * (PSC+1)
              Fpww * ---------
                       Fclk
        

        Nota: X sera el valor que debera contener el registro ATRLR.
    */

    TIM1 -> PSC = (SystemCoreClock/Base_Time) - 1 ; // Base de tiempo
    TIM1 -> ATRLR = SystemCoreClock / (Freq * (TIM1 -> PSC )) ; // Frecuencia de salida
    TIM1 -> CH2CVR = ( (TIM1 -> ATRLR) * Duty_Cycle)/100; // duty cycle
}
