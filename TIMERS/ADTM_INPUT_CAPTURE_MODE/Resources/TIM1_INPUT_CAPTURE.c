/*

    TIM1_INPUT_CAPTURE.c

*/

#include "TIM1_INPUT_CAPTURE.h"

void Input_Capture_Init(void){
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
    TIM1 -> CHCTLR1 |= TIM_CC2S_0  ; // Se cambia de CC (Compare Capture) a IC (Input Capture) y se mapea a TI2
    TIM1 -> CHCTLR1 &= ~TIM_IC2F; // Sin filtro digital
    TIM1 -> CCER &= ~TIM_CC2P; // Deteccion del flanco; 0 es por subida y 1 es por bajada 
    TIM1 -> SMCFGR |= TIM_TS_TI2FP2 |  TIM_SMS; // Modo reloj externo y disparo en TI2 
    TIM1 -> ATRLR = 100; // Cuenta maxima a la cual llegara CNT
    
    /*
        ATRLR: Registro cuya funcion es almacenar el valor maximo para el CNT, al sobrepasar este valor
               CNT inicia desde 0.
    */

    /* ACTIVACION DEL TIMER 1 */ 
    TIM1 -> CTLR1 |= TIM_CEN; // Se habilita el contador CNT
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
     
    TIM1 -> ATRLR = Ncuenta; 
}

/*########################################################################################
    SMS; Input Compare en Modo Encoder
    
/*
                    --------      ----------      ---------       --------      ---------      --------
    TI1 Y TI2 #----| FILTRO |----|   EDGE   |----| TRIGGER |-----| SELECT |----| COUNTER |----| ENABLE |
                    --------     | DETECTOR |     ---------      |  INPUT |----|   MAX   |----|   CNT  |
      CHCTLR1        CHCTLR1      ----------       SMCFGR         --------      ---------      --------   
      CC2S_0         IC2F=0          CCER           TS=0           SMCFGR         ATRLR          CTLR1
      AND/OR         IC1F=0        CC2P AND                       SMS=b001        0 TO           CEN=1
      CC1S_0                         CC1P                         SMS=b010        65535              
                                                                  SMS=b011       

     
    /* CONFIGURACION DEL TIMER1 CANAL 2 PARA EL PIN PA1 Y PD0
    TIM1 -> CHCTLR1 &= ~( TIM_CC2S  | TIM_CC1S); // Borra configuraciones por defecto
    TIM1 -> CHCTLR1 |= TIM_CC2S_0  | TIM_CC1S_0; // Se cambia de CC (Compare Capture) a IC (Input Capture) y se mapea a TI2 y TI1
    TIM1 -> CHCTLR1 &= ~(TIM_IC2F | TIM_IC1F); // Sin filtro en TI2 y TI1
    TIM1 -> CCER &= ~(TIM_CC2P| TIM_CC1P); // Deteccion del flanco; 0 es por subida y 1 es por bajada
    TIM1 -> SMCFGR &= ~TIM_TS; // Internal Trigger 0
    TIM1 -> SMCFGR &= TIM_SMS ; // Borra las configuraciones por defecto
    TIM1 -> SMCFGR |=  TIM_SMS_0 | TIM_SMS_1  ; // Encoder mode 3; 
    TIM1 -> ATRLR = 1000; // Cuenta maxima a la cual llegara CNT

    /*
        ATRLR: Registro cuya funcion es almacenar el valor maximo para el CNT, al sobrepasar este valor
               CNT inicia desde 0.
    
    TIM1 -> CTLR1 |= TIM_CEN; // Se habilita el contador CNT
##########################################################################################*/

/*########################################################################################
    SMS; Input Compare en Modo Reset (PWM INPUT CAPTURE)
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

    /* CONFIGURACION DEL TIMER1 CANAL 1 Y CANAL 2
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
    
    /* ACTIVACION DE LAS SALIDAS DEL CANAL 1 
    TIM1 -> CTLR1 |= TIM_CEN; // Se habilita el contador CNT

##########################################################################################*/

/*########################################################################################
    SMS; Input Compare en Modo Disparo (Single Pulse Mode)

/*
                 --------      ----------      ---------      --------      ---------     |\
      TI2 #-----| Filtro |----|   EDGE   |----| TRIGGER |----| SELECT |----| ON MODE |----| \  
                 --------     | DETECTOR |     ---------     | INPUT  |    |  PULSE  |----| /
     CHCTLR1      CHCTLR1      ----------       SMCFGR        --------      ---------     |/             
     CCxS_0       ICxF=0          CCER          TS=b110        SMCFGR         CTLR1
                               CCxP=0 OR 1                    SMS=b110        OPM=1
       

    /* CONFIGURACION DEL TIMER1 CANAL 2 
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


    /* CONFIGURACION DEL TIMER1 CANAL 1 
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
    
    
    /* ACTIVACION DE LAS SALIDAS DEL CANAL 1 
    TIM1 -> BDTR |= TIM_MOE; // Se habilita la salida principal OC1(TIM1CH1)
    TIM1 -> CTLR1 |= TIM_CEN; // Se habilita el contador CNT
##########################################################################################*/

/*########################################################################################
    SMS; Input Compare en Modo External Clock

/*
               --------      ----------      ---------       --------      ---------      --------
    TI2 #-----| FILTRO |----|   EDGE   |----| TRIGGER |-----| SELECT |----| COUNTER |----| ENABLE |
               --------     | DETECTOR |     ---------      |  INPUT |----|   MAX   |----|   CNT  |
    CHCTLR1     CHCTLR1      ----------        SMCFGR        --------      ---------      --------   
    CCxS_0      ICxF=0          CCER          TS=0b110        SMCFGR         ATRLR          CTLR1
                                CCxP                         SMS=b111        0 TO           CEN=1
                                                                             65535                    


    /* CONFIGURACION DEL TIMER1 CANAL 2 PARA EL PIN PA1 
    TIM1 -> CHCTLR1 |= TIM_CC2S_0  ; // Se cambia de CC (Compare Capture) a IC (Input Capture) y se mapea a TI2
    TIM1 -> CHCTLR1 &= ~TIM_IC2F; // Sin filtro digital
    TIM1 -> CCER &= ~TIM_CC2P; // Deteccion del flanco; 0 es por subida y 1 es por bajada 
    TIM1 -> SMCFGR |= TIM_TS_TI2FP2 |  TIM_SMS; // Modo reloj externo y disparo en TI2 
    TIM1 -> ATRLR = 100; // Cuenta maxima a la cual llegara CNT
    
    /*
        ATRLR: Registro cuya funcion es almacenar el valor maximo para el CNT, al sobrepasar este valor
               CNT inicia desde 0.

    /* ACTIVACION DEL TIMER 
    TIM1 -> CTLR1 |= TIM_CEN; // Se habilita el contador CNT
##########################################################################################*/








