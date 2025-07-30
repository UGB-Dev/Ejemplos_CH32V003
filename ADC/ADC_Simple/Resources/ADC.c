/*

    ADC.h

*/

#include "ADC.h"

void ADC_Init_Simple(void){
    /* HABILITACION DE PERIFERICOS */
    RCC -> APB2PCENR |= (RCC_ADC1EN | RCC_IOPDEN); // Se habilita el reloj del ADC y puerto D
    RCC -> CFGR0 |= RCC_ADCPRE_DIV2; // AHBCLK dividido por 2 ; 12 MHz

    /* CONFIGURACION DEL PUERTO D  */
    GPIOD -> CFGLR &= ~(GPIO_CFGLR_CNF3 | GPIO_CFGLR_MODE3); // Entrada analogica en PD3(AN4)

/*
                ----------      -------      -------------
  ADC_INx #----|   ADC    |----|ON/OFF |----| CALIBRATION |
               | SEQUENCE |----| ADC   |     -------------
                ----------      -------          CTLR2
                   RSQ3          CTLR2          RSTCAL=1
                 SQx=bxx         ADON=1          CAL=1



*/

    /* CONFIGURACION DEL ADC */
    ADC1 -> RSQR3 = 4; // Conversion ADC en el canal 4

    //SAMPLING DE 3 CICLOS ADC POR DEFECTO
    ADC1 -> CTLR2 |= ADC_ADON;
    Delay_Ms(5);

    /* PROCESO DE CALIBRACION */
    ADC1 -> CTLR2 |= ADC_RSTCAL; 
    while (ADC1 -> CTLR2 & ADC_RSTCAL); 
    ADC1 -> CTLR2 |= ADC_CAL; // Calibracion del ADC
    while (ADC1 -> CTLR2 & ADC_CAL);
}

uint16_t ADC_Read_Simple(void){
    ADC1 -> CTLR2 |= ADC_ADON; // Habilitacion del ADC
    while ( !(ADC1 -> STATR & ADC_EOC)); // Espera hasta que la conversion termine
    return ADC1 -> RDATAR; 
}