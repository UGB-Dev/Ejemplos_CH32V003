/*

    TIM1_PULSE_MODE.c

*/

#include "TIM1_PULSE_MODE.h"

void Pulse_Mod_Init(void){
    /* ACTIVACION DE PERIFERICOS */
    RCC -> APB2PCENR |= RCC_IOPAEN | RCC_TIM1EN | RCC_IOPDEN; // Habilitacion del reloj en puerto A y TIM1

    /* CONFIGURACION DEL PIN PA1 ; NO ES NECESARIO YA QUE POR DEFECTO ES UNA ENTRADA FLOTANTE */
    //GPIOA -> CFGLR &= ~(GPIO_CFGLR_CNF1 | GPIO_CFGLR_MODE1); // Borra configuraciones iniciales
    //GPIOA -> CFGLR |= GPIO_CFGLR_CNF1_0; // Se configura el pin PA1 como entrada flotante

    /* CONFIGURACION DEL PIN PD2 */
    GPIOD -> CFGLR &= ~(GPIO_CFGLR_MODE2 | GPIO_CFGLR_CNF2); // Borra configuraciones iniciales
    GPIOD -> CFGLR |= GPIO_CFGLR_MODE2 | GPIO_CFGLR_CNF2_1; // PD2 como salida a 30 MHz push-pull multiplexada

/*
                 --------      ----------      ---------      --------      ---------     |\
      TI2 #-----| Filtro |----|   EDGE   |----| TRIGGER |----| SELECT |----| ON MODE |----| \  
                 --------     | DETECTOR |     ---------     | INPUT  |    |  PULSE  |----| /
     CHCTLR1      CHCTLR1      ----------       SMCFGR        --------      ---------     |/             
     CCxS_0       ICxF=0          CCER          TS=b110        SMCFGR         CTLR1
                               CCxP=0 OR 1                    SMS=b110        OPM=1
      
*/ 

    /* CONFIGURACION DEL TIMER1 CANAL 2 */
    TIM1 -> CHCTLR1 |= TIM_CC2S_0 ; // Se cambia de CC (Compare Capture) a IC (Input Capture) y se mapea a TI2
    TIM1 -> CHCTLR1 &= ~TIM_IC2F; // Sin filtro digital
    TIM1 -> CCER &= ~TIM_CC2P; // Deteccion por flanco de subida
    TIM1 -> SMCFGR |= TIM_TS_TI2FP2 | TIM_SMS_1 | TIM_SMS_2; // Trigger mode en TI2FP2(TI2) 
    TIM1 -> CTLR1 |= TIM_OPM; // Activacion Single Mode Pulse

/*
        |\  -----      ---------       -------      -------      ---------      -------------------      -------------      --------     
    ----| \| DIV |----| PRESCAL |-----| DELAY |----| PULSE |----| SELECT  |----| ACTIVE OUTPUT AND |----| ACTIVE MAIN |----| ACTIVE |----### TIMxCHx or
    ----| /| CLK |----|   CNT   |      -------      -------     | MODE OC |----| LEVEL ADJUSTMENT  |----|    OUTPUT   |----|  CNT   |----### TIMxCHxN
        |/  -----      ---------        CHxCVR       ATRLR       ---------      -------------------      -------------      --------
            CTLR1         PSC           1 TO         VALUE        CHCTLR1              CCER                 BDTR             CTLR1           
            CKD=0    (Fclk/Fbase)-1     65535          +         OCxM_1 AND         CCxP=0 OR 1             MOE=1            CEN=1             
                                                     CHxCVR       OCxM_2               CCxE=1                                                                                                  
*/


    /* CONFIGURACION DEL TIMER1 CANAL 1 */
    TIM1 -> CTLR1 &= ~TIM_CKD_DIV4; // Sin divisor
    TIM1 -> PSC = 24000-1; // Base de tiempo a 1ms (tick del contador)
    TIM1 -> CH1CVR = 2000; // Tiempo del delay 250 ms
    TIM1 -> ATRLR = 7000; // (CHV1CRV + 5000) * 1 ms = 7s  ; pulso de 5 segundos
    TIM1 -> CHCTLR1 |=  TIM_OC1M_2 | TIM_OC1M_1 ; // PWM en modo 1 
    TIM1 -> CCER |= TIM_CC1P | TIM_CC1E; // Se activa la salida en OC2(TIM1CH2) a nivel 0 logico  

     /*
        PSC: Prescalador; registro que sirve como divisor de la frecuencia Fclk para la base de tiempo 
             del Timer1 o Timer2 segun sea el caso.
        ATRLR: Registro cuya funcion es almacenar el tiempo del pulso en nivel alto o bajo, el cual se le debe 
               sumar el valor del registro CHxCVR.
        CHxCVR: Registro cuya funcion es almacenar el tiempo del delay.

        Senal pulso simple:                                    ------------ ----------- ----------------------------------------------------------
                                                              |     PWM    | POLARIDAD |                     DESCRIPCION                          |
                         _______________                       ------------ ----------- ----------------------------------------------------------
                        |               |                     |            | CCxP = 0  | Salida a nivel 1 logico y el pulso sera a nivel 0 logico |
            <--Tdelay-->|<----Tpulse--->|                     | PWM MODE 1 |----------- ----------------------------------------------------------
                        |               |                     |            | CCxP = 1  | Salida a nivel 0 logico y el pulso sera a nivel 1 logico |
            ____________|               |___________           ------------ ----------- ----------------------------------------------------------
                                                              |            | CCxP = 0  | Salida a nivel 0 logico y el pulso sera a nivel 1 logico |
            |--CHxCVR --|                                     | PWM MODE 2 |----------- ----------------------------------------------------------
                                                              |            | CCxP = 1  | Salida a nivel 1 logico y el pulso sera a nivel 0 logico |
            |---------- ATRLR ----------|                      ------------ ----------- ----------------------------------------------------------

        Nota: Los pulsos dependeran de la polaridad y el modo del PWM el cual a la salida se podra
        observar si el nivel logico esta en alto o en bajo, ejemplo, suponiendo que se esta usando el 
        PWM en modo 1 por lo tanto el nivel logico siempre empieza en alto y si el bit de polaridad esta 
        en 0 entonces el nivel logico a la salida estara en alto pero si el bit de polaridad esta en 1 
        entonces el nivel logico a la salida estara en bajo, para el primer caso entoces el pulso sera
        de nivel logico 0 y para el segundo caso el pulso sera de nivel logico 1 
    */
    
    /* ACTIVACION DE LAS SALIDAS DEL CANAL 1 */
    TIM1 -> BDTR |= TIM_MOE; // Se habilita la salida principal OC1(TIM1CH1)
    TIM1 -> CTLR1 |= TIM_CEN; // Se habilita el contador CNT
}

/*********************************************************************
 * @fn      Set_Pulse
 *
 * @brief   Ajusta el pulso.
 *
 * @return  none
 */
void Set_Pulse(uint16_t Tdelay, uint16_t Tpulso, bool Polaridad){
    if (Polaridad == 0) {
        TIM1 -> CCER &= ~TIM_CC1P; // Salida del pulso a 0 logico
    }
    else{
        TIM1 -> CCER |= TIM_CC1P; // Salida del pulso a 1 logico
    }
    TIM1 -> CH1CVR = Tdelay; // Tiempo del delay 250 ms
    TIM1 -> ATRLR = Tpulso + Tdelay; // 5000 * 1 ms = 5 s  ; pulso de 5 segundos
}









