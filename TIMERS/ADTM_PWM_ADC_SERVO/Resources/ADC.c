/*

    ADC.h

*/

#include "ADC.h"

void ADC_Init_Simple(void){
    /* HABILITACION DE PERIFERICOS */
    RCC -> APB2PCENR |= (RCC_ADC1EN | RCC_IOPDEN); // Se habilita el reloj del ADC y puerto D
    RCC -> CFGR0 &= ~(0x1F<<11);
    RCC -> CFGR0 |= (2<<13); // 24 MHz / 4 = 6 MHz

    /* CONFIGURACION DEL PUERTO D  */
    GPIOD -> CFGLR &= ~(0xF<<12); // Entrada analogica en PD3(AN4)

    /* CONFIGURACION DEL ADC */
    ADC1 -> RSQR3 = 4; // Conversion ADC en el canal 4

    //SAMPLING DE 3 CICLOS ADC POR DEFECTO
    ADC1 -> CTLR2 |= ADC_ADON;
    Delay_Ms(5);

    /* PROCESO DE CALIBRACION */
    ADC1 -> CTLR2 |= ADC_RSTCAL;
    while (ADC1 -> CTLR2 & ADC_RSTCAL);
    ADC1 -> CTLR2 |= ADC_CAL;
    while (ADC1 -> CTLR2 & ADC_CAL);

}

uint16_t ADC_Read_Simple(void){
    ADC1 -> CTLR2 |= ADC_ADON; // Habilitacion del ADC
    while ( !(ADC1 -> STATR & ADC_EOC));
    return ADC1 -> RDATAR;
}