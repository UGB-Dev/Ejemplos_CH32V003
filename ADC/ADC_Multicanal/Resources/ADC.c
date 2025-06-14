/*

    ADC.c

*/

#include "ADC.h"

uint16_t ADC_VALUE[2] = {0};
uint16_t PWM_H = 0, PWM_L = 0;

// CONVERSION MULTIPLE
void ADC_Init_Multiple(void){
    /* INICIALIZACION DE PERIFERICOS */
    RCC -> APB2PCENR |= (RCC_IOPDEN | RCC_ADC1EN); // Se habilita el reloj del ADC y puerto D
    RCC -> CFGR0 &= ~(0x1F<<11);
    RCC -> CFGR0 |= (2<<13); // 24 MHz / 4 = 6 MHz

    /* CONFIGURACION DEL PUERTO D */
    GPIOD -> CFGLR &= ~((0xF<<12) | (0xF<<24)); // PD3(A4) y PD6(A6) como entrada analogica

    /* CONFIGURACION DEL ADC */
    ADC1 -> CTLR1 |= ADC_SCAN; //Se habilita el bit modo ESCANEO de canales
    ADC1 -> CTLR2 |= ADC_CONT; // Se habilita el bit modo CONTINUO
    ADC1 -> RSQR1 = (1<<20); // Dos conversiones (Nconv - 1) 

    /*         Primera Conversion
                       |  Segunda Conversion     
                       |       |
                       v       v           */
    ADC1 -> RSQR3 = (4<<0) | (6<<5) ; // Posicion del canal CH4 y CH6 para la conversion 

    /* MUESTREO DE 241 CICLOS */
    ADC1 -> SAMPTR2 &= ~((0x7<<12) | (0x7<<18));
    ADC1 -> SAMPTR2 |= (0x7<<12) | (0x7<<18);

    ADC1 -> CTLR2 |= ADC_ADON  ; // Se habilita ADC
    Delay_Ms(5);
    ADC1 -> CTLR2 |= ADC_ADON  ; 

    /* PROCESO DE CALIBRACION */
    ADC1 -> CTLR2 |= ADC_RSTCAL;
    while (ADC1 -> CTLR2 & ADC_RSTCAL);
    ADC1 -> CTLR2 |= ADC_CAL;
    while (ADC1 -> CTLR2 & ADC_CAL);  
}

void ADC_Read_Multiple(void){
    /*
        Tconv = sampling time + 11 TADCCLK
        Tconv = 241 ciclos + 11(1/12 MHz)
    */
    ADC1 -> CTLR2 |= ADC_ADON; 
    for (uint8_t i = 0; i<2 ; i++ ) {
        while ( !(ADC1 -> STATR & ADC_EOC)); // Espera hasta que finalizo la conversion
        Delay_Us(8);
        ADC_VALUE[i] = ADC1 ->RDATAR; // Se asigna el valor de cada conversion
        while ( ADC1 -> STATR & ADC_EOC); 
    }
}
