/*

    Timer2_PWM.c

*/

#include "TIM2_OUTPUT_COMPARE.h"

void Output_Compare_Init(uint8_t MODE){
     /*CONFIGURACION DE PERIFERICOS */
    RCC -> APB2PCENR |=  RCC_IOPDEN ; // Habilitacion del Puerto D 
    RCC -> APB1PCENR |= RCC_TIM2EN; // Habilitacion del Timer 2

    /* CONFIGURACION DEL PIN PD3 */
    GPIOD -> CFGLR &= ~(GPIO_CFGLR_MODE3 | GPIO_CFGLR_CNF3); // Borra configuraciones iniciales
    GPIOD -> CFGLR |= GPIO_CFGLR_MODE3 | GPIO_CFGLR_CNF3_1; // PD3 como salida en Push-Pull  a 30 MHz
    

    /* CONFIGURACION DEL TIMER 2 */
    TIM2 -> SMCFGR &= ~TIM_SMS; // Internal clock 24 MHz
    TIM2 -> CTLR1 &= ~TIM_CKD_DIV4; // Sin divisor
    if (MODE == FLIP_MODE) {
        TIM2 -> CHCTLR1 |= TIM_OC2M_1 | TIM_OC2M_0; // Modo flip 
        TIM2 -> ATRLR = 250; // Senal a 500 ms 
    }
    else if (MODE == PWM1_MODE) {
        TIM2 -> CHCTLR1 |=  TIM_OC2M_2 | TIM_OC2M_1 ; // PWM en modo 1
        TIM2 -> ATRLR = 1000; // Senal a 1s
        TIM2 -> CH2CVR = 500; // 50 % duty cycle canal 2
    }
    else if (MODE == PWM2_MODE) {
        TIM2 -> CHCTLR1 |=  TIM_OC2M_2 | TIM_OC2M_1 | TIM_OC2M_0 ; // PWM en modo 2
        TIM2 -> ATRLR = 250; // Senal a 250 ms
        TIM2 -> CH2CVR = 125; // 50 % duty cycle canal 2
    }

    TIM2 -> PSC = 24000-1; // Base de tiempo a 1ms
    TIM2 -> CCER &= ~TIM_CC2P ; // Valor logico en OC2REF directo a la salida OC2(TIM2CH2)
    TIM2 -> CCER |=  TIM_CC2E; // Activa la salida en OC2(TIM2CH2)

    /* ACTIVACION DE LAS SALIDAS DEL CANAL X */
    TIM2 -> BDTR |= TIM_MOE; // Se habilita la salida principal
    TIM2 -> CTLR1 |= TIM_CEN; // Se habilita el contador
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

    TIM2 -> PSC = (SystemCoreClock/Base_Time) - 1 ; // Base de tiempo
    TIM2 -> ATRLR = SystemCoreClock / (Freq * (TIM2 -> PSC )) ; // Frecuencia de salida
    TIM2 -> CH2CVR = ( (TIM2 -> ATRLR) * Duty_Cycle)/100; // duty cycle
}

/*########################################################################################
    OCxM; Output Compare en modo de forzado de nivel valido

/*
     --------      -----      ---------       ------      ---------      -------------------      -------------      --------     
    | SOURCE |----| DIV |----| PRESCAL |-----| TIME |----| SELECT  |----| ACTIVE OUTPUT AND |----| ACTIVE MAIN |----| ACTIVE |----### TIMxCHx or
    |  CLK   |----| CLK |----|   CNT   |-----| (T)  |----| MODE OC |----| LEVEL ADJUSTMENT  |----|    OUTPUT   |----|  CNT   |----### TIMxCHxN
     --------      -----      ---------       ------      ---------      -------------------      -------------      --------
      SMCFGR       CTLR1         PSC           ATRLR       CHCTLR1               CCER                 BDTR             CTLR1           
       SMS=0       CKD=0    (Fclk/Fbase)-1     1 TO        OCxM_1 OR          CCxP=0 OR 1             MOE=1            CEN=1             
                                               65535       OCxM_0               CCxE=1                                                                                               

    /* CONFIGURACION DEL TIMER 2 
    TIM2 -> SMCFGR &= ~TIM_SMS; // Internal clock 24 MHz
    TIM2 -> CTLR1 &= ~TIM_CKD_DIV4; // Sin divisor
    TIM2 -> PSC = 24000-1; // Base de tiempo a 1ms
    TIM2 -> ATRLR = 50; // 50 * 1 ms = 50 ms
    TIM2 -> CHCTLR1 |= TIM_OC2M_1; // Forza la salida a nivel bajo despues que CNT = ATRLR
    TIM2 -> CCER &= ~TIM_CC2P; // Valor logico en OC2REF directo a la salida OC2(TIM2CH2)
    TIM2 -> CCER |=  TIM_CC2E; // Activa la salida en OC2(TIM2CH2)  

    /*
        PSC: Prescalador; registro que sirve como divisor de la frecuencia Fclk para la base de tiempo 
             del Timer1 o Timer2 segun sea el caso.
        ATRLR: Registro cuya funcion es almacenar el tiempo que tomara en pasar a un nivel alto(1 logico)
               o a un nivel bajo(0 logico), dependera si uno de los bits TIM_OCxM_0 (salida en alto) O 
               TIM_OCxM_1(salida en bajo) estan activos.

        Senal :
                         ___________              ____________
                        |                                     |
                        |                                     |
                        |                                     |
            ____________|                                     |___________
            
            |-- ATRLR --|                         |-- ATRLR --|

        Nota: Para esta configuracion se valida la comparacion y se mantiene en alto o bajo el pin,
              en el caso de la configuracion de invalidacion por comparacion se genera lo contrario,
              basicamente no son muy usados ya que el pin lo mantienen en un valor logico todo el tiempo
              por lo tanto no es recomendable su uso.
 
    
    /* ACTIVACION DE LAS SALIDAS DEL CANAL X 
    TIM2 -> BDTR |= TIM_MOE; // Se habilita la salida principal OC2(TIM2CH2)
    TIM2 -> CTLR1 |= TIM_CEN; // Se habilita el contador CNT
##########################################################################################*/

/*########################################################################################
    OCxM; Output Compare en modo flip 

/*
     --------      -----      ---------       ------      ---------      -------------------      -------------      --------     
    | SOURCE |----| DIV |----| PRESCAL |-----| TIME |----| SELECT  |----| ACTIVE OUTPUT AND |----| ACTIVE MAIN |----| ACTIVE |----### TIMxCHx or
    |  CLK   |----| CLK |----|   CNT   |-----| (T)  |----| MODE OC |----| LEVEL ADJUSTMENT  |----|    OUTPUT   |----|  CNT   |----### TIMxCHxN
     --------      -----      ---------       ------      ---------      -------------------      -------------      --------
      SMCFGR       CTLR1         PSC           ATRLR       CHCTLR1               CCER                 BDTR             CTLR1           
       SMS=0       CKD=0    (Fclk/Fbase)-1     1 TO        OCxM_1 AND         CCxP=0 OR 1             MOE=1            CEN=1             
                                               65535       OCxM_0               CCxE=1                                                                                                  

    /* CONFIGURACION DEL TIMER 2 
    TIM2 -> SMCFGR &= ~TIM_SMS; // Internal clock 24 MHz
    TIM2 -> CTLR1 &= ~TIM_CKD_DIV4; // Sin divisor
    TIM2 -> PSC = 24000-1; // Base de tiempo a 1ms
    TIM2 -> ATRLR = 250; // 250 * 1 ms = 250 ms
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
        
    
    /* ACTIVACION DE LAS SALIDAS DEL CANAL X 
    
    TIM2 -> BDTR |= TIM_MOE; // Se habilita la salida principal OC2(TIM2CH2)
    TIM2 -> CTLR1 |= TIM_CEN; // Se habilita el contador CNT
##########################################################################################*/

/*########################################################################################
    OCxM; Output Compare en modo PWM 1 y 2 

/*
     --------      -----      ---------       ------      ---------      -------------------      -------------      --------     
    | SOURCE |----| DIV |----| PRESCAL |-----| TIME |----| SELECT  |----| ACTIVE OUTPUT AND |----| ACTIVE MAIN |----| ACTIVE |----### TIMxCHx or
    |  CLK   |----| CLK |----|   CNT   |-----| (T)  |----| MODE OC |----| LEVEL ADJUSTMENT  |----|    OUTPUT   |----|  CNT   |----### TIMxCHxN
     --------      -----      ---------       ------      ---------      -------------------      -------------      --------
      SMCFGR       CTLR1         PSC           ATRLR       CHCTLR1               CCER                 BDTR             CTLR1           
       SMS=0       CKD=0    (Fclk/Fbase)-1     1 TO        OCxM_1 AND         CCxP=0 OR 1             MOE=1            CEN=1             
                                               65535       OCxM_2               CCxE=1                                                                                                  

    /* CONFIGURACION DEL TIMER 2 
    TIM2 -> SMCFGR &= ~TIM_SMS; // Internal clock 24 MHz
    TIM2 -> CTLR1 &= ~TIM_CKD_DIV4; // Sin divisor
    TIM2 -> PSC = 24000-1; // Base de tiempo a 1ms
    TIM2 -> ATRLR = 1000; // 1000 * 1 ms = 1s
    TIM2 -> CH2CVR = 500; // 50 % duty cycle canal 2
    TIM2 -> CHCTLR1 |=  TIM_OC2M_2 | TIM_OC2M_1 ; // PWM en modo 1 o agrega TIM_OC2M_0 para PWM en modo 2
    TIM2 -> CCER &= ~TIM_CC2P; // Valor logico en OC2REF directo a la salida OC2(TIM2CH2)
    TIM2 -> CCER |=  TIM_CC2E; // Activa la salida en OC2(TIM2CH2)

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

    /* ACTIVACION DE LAS SALIDAS DEL CANAL X 
    TIM2 -> BDTR |= TIM_MOE; // Se habilita la salida principal OC2(TIM2CH2)
    TIM2 -> CTLR1 |= TIM_CEN; // Se habilita el contador CNT

##########################################################################################*/
