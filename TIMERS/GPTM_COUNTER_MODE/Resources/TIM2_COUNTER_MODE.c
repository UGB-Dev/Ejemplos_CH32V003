/*

    TIM2_COUNTER_MODE.c

*/

#include "TIM2_COUNTER_MODE.h"

void Counter_Mode_Init(void){
    /* ACTIVACION DE PERIFERICOS */
    RCC -> APB2PCENR |= RCC_IOPDEN; // Habilitacion del puerto D 
    RCC -> APB1PCENR |= RCC_TIM2EN; // Habilitacion del Timer 2

    /* CONFIGURACION DEL PIN PD3 ; NO ES NECESARIO YA QUE POR DEFECTO ES UNA ENTRADA FLOTANTE*/
    //GPIOD -> CFGLR &= ~(GPIO_CFGLR_CNF3 | GPIO_CFGLR_MODE3); // Se borra configuraciones iniciales
    //GPIOD -> CFGLR |= GPIO_CFGLR_CNF3_0; // Se configura el pin PD3 como entrada flotante

/*
               --------      ----------      ---------       --------      ---------      --------
    TI2 #-----| FILTRO |----|   EDGE   |----| TRIGGER |-----| SELECT |----| COUNTER |----| ENABLE |
               --------     | DETECTOR |     ---------      |  INPUT |----|   MAX   |----|   CNT  |
    CHCTLR1     CHCTLR1      ----------        SMCFGR        --------      ---------      --------   
    CCxS_0      ICxF=0          CCER          TS=0b110        SMCFGR         ATRLR          CTLR1
                                CCxP                         SMS=b111        0 TO           CEN=1
                                                                             65535                    
*/ 

    /* CONFIGURACION DEL TIMER1 CANAL 2 PARA EL PIN PD3*/ 
    TIM2 -> CHCTLR1 |= TIM_CC2S_0 ; // Se cambia de CC (Compare Capture) a IC (Input Capture) y se mapea a TI2
    TIM2 -> CHCTLR1 &= ~TIM_IC2F; // Sin filtro digital
    TIM2 -> CCER &= ~TIM_CC2P; // Deteccion del flanco; 0 es por subida y 1 es por bajada 
    TIM2 -> SMCFGR |= TIM_TS_TI2FP2 |  TIM_SMS; // Modo reloj externo y disparo en TI2 

    /* ACTIVACION DEL TIMER 2 */ 
    TIM2 -> CTLR1 |= TIM_CEN ; // Se habilita el contador CNT
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
        TIM2 -> CCER &= ~TIM_CC2P; // Salida del pulso a 0 logico
    }
    else{
        TIM2 -> CCER |= TIM_CC2P; // Salida del pulso a 1 logico
    }

    /*
        ATRLR: Registro cuya funcion es almacenar el numero delimitador del registro CNT para
               reiniciarce a 0 nuevamente, se genera el reinicio en CNT despues de sobrepasar 
               el valor de ATRLR.
    */
    TIM2 -> CNT = 0; 
    TIM2 -> ATRLR = Ncuenta; 
}
