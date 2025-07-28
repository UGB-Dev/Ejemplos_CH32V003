/*

    TIM1_COUNTER_MODE.c

*/

#include "TIM1_COUNTER_MODE.h"

void Counter_Mode_Init(void){
    /* ACTIVACION DE PERIFERICOS */
    RCC -> APB2PCENR |= RCC_IOPAEN | RCC_TIM1EN ; // Habilitacion del reloj en puerto A y TIM1

    /* CONFIGURACION DEL PIN PA1 ; NO ES NECESARIO YA QUE POR DEFECTO ES UNA ENTRADA FLOTANTE*/
    //GPIOA -> CFGLR &= ~(GPIO_CFGLR_CNF1 | GPIO_CFGLR_MODE1); // Se borra configuraciones iniciales
    //GPIOA -> CFGLR |= GPIO_CFGLR_CNF1_0; // Se configura el pin PA1 como entrada flotante

/*
               --------      ----------      ---------       --------      ---------      --------
    TI2 #-----| FILTRO |----|   EDGE   |----| TRIGGER |-----| SELECT |----| COUNTER |----| ENABLE |
               --------     | DETECTOR |     ---------      |  INPUT |----|   MAX   |----|   CNT  |
    CHCTLR1     CHCTLR1      ----------        SMCFGR        --------      ---------      --------   
    CCxS_0      ICxF=0          CCER          TS=0b110        SMCFGR         ATRLR          CTLR1
                                CCxP                         SMS=b111        0 TO           CEN=1
                                                                             65535                    
*/ 

    /* CONFIGURACION DEL TIMER1 CANAL 2 PARA EL PIN PA1*/ 
    TIM1 -> CHCTLR1 |= TIM_CC2S_0 ; // Se cambia de CC (Compare Capture) a IC (Input Capture) y se mapea a TI2
    TIM1 -> CHCTLR1 &= ~TIM_IC2F; // Sin filtro digital
    TIM1 -> CCER &= ~TIM_CC2P; // Deteccion del flanco; 0 es por subida y 1 es por bajada 
    TIM1 -> SMCFGR |= TIM_TS_TI2FP2 |  TIM_SMS; // Modo reloj externo y disparo en TI2 

    /* ACTIVACION DEL TIMER 1 */ 
    TIM1 -> CTLR1 |= TIM_CEN ; // Se habilita el contador CNT
}

/*********************************************************************
 * @fn      Set_Cuenta
 *
 * @brief   Ncuenta : Ajusta la cuenta maxima .
 *          Edge: Ajusta el flanco de conteo
 *
 * @return  none
 */
void Set_Cuenta(uint16_t Ncuenta, bool Edge){
    if (Edge == 0) {
        TIM1 -> CCER &= ~TIM_CC2P; // Salida del pulso a 0 logico
    }
    else{
        TIM1 -> CCER |= TIM_CC2P; // Salida del pulso a 1 logico
    }

    /*
        ATRLR: Registro cuya funcion es almacenar el numero delimitador del registro CNT para
               reiniciarce a 0 nuevamente, se genera el reinicio en CNT despues de sobrepasar 
               el valor de ATRLR.
    */
    TIM1 -> CNT = 0; 
    TIM1 -> ATRLR = Ncuenta; 
}
