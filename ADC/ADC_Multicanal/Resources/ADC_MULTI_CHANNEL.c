/*

    ADC_MULTI_CHANNEL.c

*/

#include "ADC_MULTI_CHANNEL.h"

uint16_t ADC_VALUE[2] = {0};
uint16_t PWM_H = 0, PWM_L = 0;

// CONVERSION MULTIPLE
void ADC_Multi_Channel_Init(void){
    /* INICIALIZACION DE PERIFERICOS */
    RCC -> APB2PCENR |= (RCC_IOPDEN | RCC_ADC1EN); // Se habilita el reloj del ADC y puerto D
    RCC -> CFGR0 |=  RCC_ADCPRE_2 | RCC_ADCPRE_3 | RCC_ADCPRE_4; // AHBCLK dividido por 16 ; 1.5 MHz hacia reloj ADC

    /* CONFIGURACION DEL PUERTO D */
    GPIOD -> CFGLR &= ~(GPIO_CFGLR_CNF3 | GPIO_CFGLR_MODE3 | 
                        GPIO_CFGLR_CNF6 | GPIO_CFGLR_MODE6); // PD3(A4) y PD6(A6) como entrada analogica

/*
                ------      -----------      ------------      ----------      ----------      -------      -------------
  ADC_INx #----| SCAN |----| CONTINOUS |----|     N#     |----| SAMPLING |----|   ADC    |----|ON/OFF |----| CALIBRATION |
               | MODE |----|   MODE    |----| CONVERSION |     ----------     | SEQUENCE |----| ADC   |     -------------
                ------      -----------      ------------       SAMPTR2        ----------      -------          CTLR2
                 CTLR1         CTLR1            RSQR1          SMPx=b000          RSQ3          CTLR2          RSTCAL=1
                 SCAN=1        CONT=1         L=0 TO 15                          SQx=bxx        ADON=1          CAL=1

    NOTA: El prescalador del registro " RCC -> CFGR0 " debera ser configurado aun valor de 16 o superior,esto es
    necesario para que se pueda capturar los valores de la conversion de cada canal, de lo contrario use el DMA.
*/

    /* CONFIGURACION DEL ADC */
    ADC1 -> CTLR1 |= ADC_SCAN; // Se habilita el bit modo ESCANEO de canales
    ADC1 -> CTLR2 |= ADC_CONT; // Se habilita el bit modo CONTINUO
    ADC1 -> RSQR1 =  (1<<20); // Dos conversiones (Nconv - 1) 

    /*         Primera Conversion
                       |  Segunda Conversion     
                       |       |
                       v       v           */
    ADC1 -> RSQR3 = (4<<0) | (6<<5) ; // Posicion del canal CH4 y CH6 para la conversion 

    /* MUESTREO DE 241 CICLOS */
    ADC1 -> SAMPTR2 &= ~(ADC_SMP4 | ADC_SMP6); // Borra configuraciones iniciales
    ADC1 -> SAMPTR2 |= ADC_SMP4 | ADC_SMP6; // Muestreo de 241 ciclos

    ADC1 -> CTLR2 |= ADC_ADON  ; // Se habilita ADC
    Delay_Ms(5);

    /* PROCESO DE CALIBRACION */
    ADC1 -> CTLR2 |= ADC_RSTCAL;
    while (ADC1 -> CTLR2 & ADC_RSTCAL);
    ADC1 -> CTLR2 |= ADC_CAL; // Calibracion del ADC
    while (ADC1 -> CTLR2 & ADC_CAL);  
}

void ADC_Multi_Channel_Read(void){
    ADC1 -> CTLR2 |= ADC_ADON; 
    for (uint8_t i = 0; i<2 ; i++ ) {
        while ( !(ADC1 -> STATR & ADC_EOC)); // Espera hasta que finaliza la conversion
        ADC_VALUE[i] = ADC1 ->RDATAR; // Se asigna el valor de cada conversion
    }
}