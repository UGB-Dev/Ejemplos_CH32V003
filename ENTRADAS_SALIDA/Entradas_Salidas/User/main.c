/********************************** (C) COPYRIGHT *******************************
 * File Name          : main.c
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2023/12/25
 * Description        : Entradas y Salidas; Activacion de PD2 mediante PC0.
 *********************************************************************************/
 
  /*
    Configuracion en archivo sytem_ch32v00x.c
 
    comentar esta linea -> #define SYSCLK_FREQ_48MHz_HSE   48000000
    descomentar esta linea -> #define SYSCLK_FREQ_24MHZ_HSI   HSI_VALUE

    El MCU CH32V003 trabajara a 24 MHz y los perifericos a 24 MHz
 */

#include "debug.h"

#define LED_ON (GPIOD -> BSHR = GPIO_BSHR_BS2)
#define LED_OFF (GPIOD -> BSHR = GPIO_BSHR_BR2)



/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */
int main(void){

    /*ACTIVACION DE PERIFERICOS*/
    RCC -> APB2PRSTR |= RCC_IOPCRST | RCC_IOPDRST; // activar reset en puerto C y D
    RCC -> APB2PRSTR &= ~(RCC_IOPCRST | RCC_IOPDRST); // desactivar reset en puerto C y D
    RCC -> APB2PCENR |= RCC_IOPCEN | RCC_IOPDEN; // Habilitacion del reloj

    /*CONFIGURACION DE LOS PINES DEL PUERTO C */
    GPIOC -> CFGLR &=  ~(0xF<<0); // se desactivan configuraciones random
    GPIOC -> CFGLR |= GPIO_CFGLR_CNF0_1; // PC0 como entrada en modo pull-up y pull-down
    GPIOC -> OUTDR |= GPIO_OUTDR_ODR0; // pull-up en PC0

    /*CONFIGURACION DE LOS PINES DEL PUERTO D*/
    GPIOD -> CFGLR &= ~(0xF<<8); // se desactivan configuraciones random
    GPIOD -> CFGLR |= GPIO_CFGLR_MODE2; // PD2 como salida en push-pull a 30 MHz
   
    while(1){
        /*ACTIVA EL LED SI HAY UN 0 LOGICO EN PC0*/
        if ( (GPIOC -> INDR & GPIO_INDR_IDR0) == 0) { 
            LED_ON;
        }
        else {
            LED_OFF;
        }
    }
}
