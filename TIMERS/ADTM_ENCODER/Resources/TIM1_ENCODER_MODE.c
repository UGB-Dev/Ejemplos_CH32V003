/*

    TIM1_ENCODER_MODE.c

*/

#include "TIM1_ENCODER_MODE.h"

void Encoder_Mod_Init(void){
    /* ACTIVACION DE PERIFERICOS */
    RCC -> APB2PCENR |= RCC_IOPAEN | RCC_TIM1EN | RCC_IOPDEN ; // Habilitacion del reloj en puerto A, D y TIM1

    /* CONFIGURACION DEL PIN PA1 ; NO ES NECESARIO YA QUE POR DEFECTO ES UNA ENTRADA FLOTANTE*/
    //GPIOA -> CFGLR &= ~(GPIO_CFGLR_CNF1 | GPIO_CFGLR_MODE1); // Se borra configuraciones iniciales
    //GPIOA -> CFGLR |= GPIO_CFGLR_CNF1_0; // Se configura el pin PA1 como entrada flotante
    
    /* CONFIGURACION DEL PIN PD2 ; NO ES NECESARIO YA QUE POR DEFECTO ES UNA ENTRADA FLOTANTE*/
    //GPIOD -> CFGLR &= ~(GPIO_CFGLR_CNF2 | GPIO_CFGLR_MODE2); // Se borra configuraciones iniciales
    //GPIOD -> CFGLR |= GPIO_CFGLR_CNF2_0; // Se configura el pin PD2 como entrada flotante

/*
                    --------      ----------      ---------       --------      ---------      --------
    TI1 Y TI2 #----| FILTRO |----|   EDGE   |----| TRIGGER |-----| SELECT |----| COUNTER |----| ENABLE |
                    --------     | DETECTOR |     ---------      |  INPUT |----|   MAX   |----|   CNT  |
      CHCTLR1        CHCTLR1      ----------       SMCFGR         --------      ---------      --------   
      CC2S_0         IC2F=0          CCER           TS=0           SMCFGR         ATRLR          CTLR1
      AND/OR         IC1F=0        CC2P AND                       SMS=b001        0 TO           CEN=1
      CC1S_0                         CC1P                         SMS=b010        65535              
                                                                  SMS=b011          
*/ 

    /* CONFIGURACION DEL TIMER1 CANAL 1 Y CANAL 2 PARA EL PIN PA1 Y PD2 */ 
    TIM1 -> CHCTLR1 &= ~( TIM_CC2S  | TIM_CC1S); // Borra configuraciones por defecto
    TIM1 -> CHCTLR1 |= TIM_CC2S_0  | TIM_CC1S_0; // Se cambia de CC (Compare Capture) a IC1 e IC2 (Input Capture) y se mapea a TI2 y TI1
    TIM1 -> CHCTLR1 &= ~(TIM_IC2F | TIM_IC1F); // Sin filtro en TI2 y TI1
    TIM1 -> CCER &= ~(TIM_CC2P| TIM_CC1P); // Deteccion del flanco; 0 es por subida y 1 es por bajada
    TIM1 -> SMCFGR &= ~TIM_TS; // Internal Trigger 0
    TIM1 -> SMCFGR &= TIM_SMS ; // Borra las configuraciones por defecto
    TIM1 -> SMCFGR |= TIM_SMS_0  ; // Encoder mode 1; 
    TIM1 -> ATRLR = 1000; // Cuenta maxima a la cual llegara CNT

    /*
        ATRLR: Registro cuya funcion es almacenar el valor maximo para el CNT, al sobrepasar este valor
               CNT inicia desde 0.
    */
    
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









