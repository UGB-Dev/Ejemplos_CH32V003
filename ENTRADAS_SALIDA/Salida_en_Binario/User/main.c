/********************************** (C) COPYRIGHT *******************************
 * File Name          : main.c
 * Author             : Uriel Garnica
 * Version            : V1.0.0
 * Date               : 2023/12/25
 * Description        : Salida en binario; conteo de pulsos en PD4 que se visualizaran en
                        el puerto C.
 *********************************************************************************/
/*
    Configuracion en archivo sytem_ch32v00x.c
 
    comentar esta linea -> #define SYSCLK_FREQ_48MHz_HSE   48000000
    descomentar esta linea -> #define SYSCLK_FREQ_24MHZ_HSI   HSI_VALUE

    El MCU CH32V003 trabajara a 24 MHz y los perifericos a 24 MHz
*/

#include "debug.h"

/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */
int main(void){
    Delay_Init();

    /*  ACTIVACION DE PERIFERICOS  */ 
    RCC -> APB2PCENR |= RCC_IOPCEN | RCC_IOPDEN; // Se habilita el reloj del puerto C y D

    /*  CONFIGURACION DE LOS PINES DEL PUERTO C  */
    GPIOC -> CFGLR &= ~(GPIO_CFGLR_MODE | GPIO_CFGLR_CNF); // Borra configuraciones iniciales  
    GPIOC -> CFGLR |= GPIO_CFGLR_MODE; // Puerto C como salida a 30 MHz en modo Push-Pull

    /*  CONFIGURACION DE LOS PINES DEL PUERTO D  */
    GPIOD -> CFGLR &= ~(GPIO_CFGLR_MODE4 | GPIO_CFGLR_CNF4); // Borra configuraciones iniciales
    GPIOD -> CFGLR |= GPIO_CFGLR_CNF4_1; // PD4 como entrada en modo pull-down por defecto

    uint8_t Cuenta=0; // variable local de control

    while(1){
        /* SE DETRMINA QUE EL PIN DE ENTRADA SEA PD4 */
        if ( GPIOD -> INDR & GPIO_INDR_IDR4 ) {
             while (GPIOD -> INDR & GPIO_INDR_IDR4); // Antirrebote
            GPIOC -> OUTDR = Cuenta++; // Se envia el valor de cuenta hacia el puerto C  
        }
    }
}
