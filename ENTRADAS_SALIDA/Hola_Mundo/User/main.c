/********************************** (C) COPYRIGHT *******************************
 * File Name          : main.c
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2023/12/25
 * Description        : Hola Mundo; Parpadeo de un led (PD5).
 *********************************************************************************/
 
 /*
    Configuracion en archivo sytem_ch32v00x.c
 
    comentar esta linea -> #define SYSCLK_FREQ_48MHz_HSE   48000000
    descomentar esta linea -> #define SYSCLK_FREQ_24MHZ_HSI   HSI_VALUE

    El MCU CH32V003 trabajara a 24 MHz y los perifericos a 24 MHz
 */

#include "debug.h"

#define LED_ON (GPIOD -> BSHR = GPIO_BSHR_BS5) // set pin PD5
#define LED_OFF (GPIOD -> BSHR = GPIO_BSHR_BR5) // reset pin PD5

/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */
int main(void){
    Delay_Init(); // inicializa los tiempos us y ms

    /* Configuracion del Puerto D*/
    RCC -> APB2PRSTR |= RCC_IOPDRST; // Reset del puerto D activo
    RCC -> APB2PRSTR &= ~RCC_IOPDRST; // Reset del puerto D inactivo
    RCC -> APB2PCENR |= RCC_IOPDEN; // se habilita el reloj del puerto D
    GPIOD -> CFGLR &= ~(0xF<<20); // reset del pin PD5
    GPIOD -> CFGLR |= GPIO_CFGLR_MODE5; // salida push-pull a 30 MHz max
    
    while(1){
        LED_ON; // PD5 en 1 logico
        Delay_Ms(500);
        LED_OFF; // PD5 en 0 logico
        Delay_Ms(500);
    }
}
