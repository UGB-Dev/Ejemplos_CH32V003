/*

    TIM1_PWM_INPUT_MODE.c

*/

#include "TIM1_PWM_INPUT_MODE.h"

void PWM_Input_Mode_Init(void){
    /* ACTIVACION DE PERIFERICOS */
    RCC -> APB2PCENR |= RCC_IOPAEN | RCC_TIM1EN | RCC_IOPDEN; // Habilitacion del reloj en puerto A, D y TIM1

    /* CONFIGURACION DEL PIN PA1 ; NO ES NECESARIO YA QUE POR DEFECTO ES UNA ENTRADA FLOTANTE */
    //GPIOA -> CFGLR &= ~(GPIO_CFGLR_CNF1 | GPIO_CFGLR_MODE1); // Borra configuraciones iniciales
    //GPIOA -> CFGLR |= GPIO_CFGLR_CNF1_0; // Se configura el pin PA1 como entrada flotante

    /* CONFIGURACION DEL PIN PD2 ; NO ES NECESARIO YA QUE POR DEFECTO ES UNA ENTRADA FLOTANTE */
    //GPIOD -> CFGLR &= ~(GPIO_CFGLR_CNF2 | GPIO_CFGLR_MODE2); // Borra configuraciones iniciales
    //GPIOD -> CFGLR |= GPIO_CFGLR_CNF2_0; // Se configura el pin PD2 como entrada flotante


/*
                 ----------     
      TI1 #-----|   EDGE   |---------. 
                | DETECTOR |--------.|    
     CHCTLR1     ----------         ||         
     CC1S_0         CCER            ||     
                   CC1P=0          _||_
                                   \  /
                                    \/
                      CHCTLR1    --------      ---------      --------      -------------------      --------
                      IC1F=0    | Filtro |----| TRIGGER |----| SELECT |----| ENABLE CAPTURE IN |----| ACTIVE |  
                      IC2F=0     --------      ---------     | INPUT  |----| CH1CVR AND CH2CVR |----|  CNT   |
                                    /\           SMCFGR       --------      -------------------      --------     
                                   /__\         TS=b101        SMCFGR               CCER               CTLR1
                                    ||                        SMS=b100             CC1E=1              CEN=1
                 ----------         ||                                             CC2E=1
      TI2 #-----|   EDGE   |--------*| 
                | DETECTOR |---------*     
     CHCTLR1     ----------                  
     CC2S_1         CCER        
                   CC2P=1       
*/ 

    /* CONFIGURACION DEL TIMER1 CANAL 1 Y CANAL 2*/
    TIM1 -> CHCTLR1 |= TIM_CC1S_0 ; // Se cambia de CC (Compare Capture) a IC1 (Input Capture) y se mapea a TI1
    TIM1 -> CCER &= ~TIM_CC1P; // Deteccion por flanco de subida( rising edge)

    TIM1 -> CHCTLR1 |= TIM_CC2S_1 ; // Se cambia de CC (Compare Capture) a IC2 (Input Capture) y se mapea a TI1
    TIM1 -> CCER |= TIM_CC2P; // Deteccion por flanco de bajada( falling edge)

    TIM1 -> CHCTLR1 &= ~(TIM_IC2F|TIM_IC1F); // Sin filtro digital
    TIM1 -> SMCFGR |= TIM_TS_TI1FP1 | TIM_SMS_2; // Reset mode en cada flanco de subida
    TIM1 -> CCER |= TIM_CC1E | TIM_CC2E; // Habilita las capturas por CH1CVR y CH2CVR

     /*
        PSC: Prescalador; registro que sirve como divisor de la frecuencia Fclk para la base de tiempo 
             del Timer1 o Timer2 segun sea el caso.
        CHxCVR: Registro cuya funcion es almacenar el tiempo de la captura del canal.
    */
    
    /* ACTIVACION DE LAS SALIDAS DEL CANAL 1 */
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









