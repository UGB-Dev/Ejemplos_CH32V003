/*

    Timer1_PWM.c

*/

#include "Timer1_PWM.h"

void PWM_Init(void){
     /*CONFIGURACION DE PERIFERICOS */
    RCC -> APB2PCENR |=  (RCC_IOPAEN | RCC_TIM1EN); // Habilitcion de reloj en Puerto A y Timer 1

    GPIOA -> CFGLR &= ~(GPIO_CFGLR_MODE1 | GPIO_CFGLR_CNF1);
    GPIOA -> CFGLR |= GPIO_CFGLR_MODE1 | GPIO_CFGLR_CNF1_1; // PA1 como salida en Push-Pull  a 30 MHz

    /*
        0b110 en OC1M es para habilitar el PWM en modo 1
        0b111 en OC1M es para habilitar el PWM en modo 2
    */

    TIM1 -> CHCTLR1 |= TIM_OC2M_2 | TIM_OC2M_1 | TIM_OC2PE; // PWM en modo 1
    TIM1 -> CTLR1 |= TIM_ARPE; // Habilitacion de la auto recarga del registro ATRLR

    /*
        PSC: Prescalador; registro que sirve como divisor de la frecuencia Fclk
             para la base de tiempo del Timer1 o Timer2 segun sea el caso.
        ATRLR: Registro cuya funcion es generar el periodo (T) de la senal PWM.
        CHxCVR: Registro cuya funcion es almacenar el tiempo en que la senal cambia
                de alto a bajo o bien de bajo a alto segun sea el caso.

        Senal PWM:
             ___________
            |           |           |
            |           |           |
            |           |           |
            |           |___________|
            
            |--CHxCVR --|

            |-------- ATRLR --------|

    */

    TIM1 -> PSC = 24000-1; // Base de tiempo a 1ms
    TIM1 -> ATRLR = 500; // 500 * 1 ms = 500 ms
    TIM1 -> CH2CVR = 250; // 50 % duty cycle
    
    /* ACTIVACION DE LAS SALIDAS DEL CANAL X */
    TIM1 -> CCER |= TIM_CC2E; // Habilita la salida del canal 2
    TIM1 -> BDTR |= TIM_MOE; // Se habilita la salida principal
    //TIM1 -> SWEVGR |= TIM_UG;
    TIM1 -> CTLR1 |= TIM_CEN; // Se habilita el contador
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