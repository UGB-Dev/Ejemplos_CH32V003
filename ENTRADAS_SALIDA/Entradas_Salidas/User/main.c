/********************************** (C) COPYRIGHT *******************************
 * File Name          : main.c
 * Author             : Uriel Garnica
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

#define LED_ON (GPIOD -> BSHR = GPIO_BSHR_BS2) // Set pin PD2
#define LED_OFF (GPIOD -> BSHR = GPIO_BSHR_BR2) // Set pin PD2

/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */
int main(void){

    /* ACTIVACION DE PERIFERICOS */
    RCC -> APB2PCENR |= RCC_IOPCEN | RCC_IOPDEN; // Habilitacion del reloj en puerto C y D

    /* CONFIGURACION DE LOS PINES DEL PUERTO C */
    GPIOC -> CFGLR &=  ~(GPIO_CFGLR_MODE0 | GPIO_CFGLR_CNF0); // Borra configuraciones iniciales
    GPIOC -> CFGLR |= GPIO_CFGLR_CNF0_1; // PC0 como entrada en modo Pull-Up y Pull-Down
    GPIOC -> OUTDR |= GPIO_OUTDR_ODR0; // Habilita resistencia de Pull-Up en PC0

    /* CONFIGURACION DE LOS PINES DEL PUERTO D */
    GPIOD -> CFGLR &= ~(GPIO_CFGLR_MODE2 | GPIO_CFGLR_CNF2); // Borra configuraciones iniciales
    GPIOD -> CFGLR |= GPIO_CFGLR_MODE2; // PD2 como salida a 30 MHz en modo Push-Pull
   
    while(1){
        /* ACTIVA EL LED SI HAY UN 0 LOGICO EN PC0 */
        if ( (GPIOC -> INDR & GPIO_INDR_IDR0) == 0){ 
            LED_ON;
        }
        else {
            LED_OFF;
        }
    }
}
