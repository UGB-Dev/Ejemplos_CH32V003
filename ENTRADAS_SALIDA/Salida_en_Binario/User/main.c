/********************************** (C) COPYRIGHT *******************************
 * File Name          : main.c
 * Author             : WCH
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
    RCC -> APB2PRSTR |= RCC_IOPCRST | RCC_IOPDRST; // activacion de reset en puerto C y D
    RCC -> APB2PRSTR &=~(RCC_IOPCRST | RCC_IOPDRST); // desactivacion de reset en puerto C y D
    RCC -> APB2PCENR |= RCC_IOPCEN | RCC_IOPDEN; // se habilita el reloj de puerto C y D

    /*  CONFIGURACION DE LOS PINES DEL PUERTO C  */
    GPIOC -> CFGLR = 0; // reset en todo el puerto C  
    GPIOC -> CFGLR = 0x33333333; // Puerto C como salida push-pull a 30MHz 

    /*  CONFIGURACION DE LOS PINES DEL PUERTO D  */
    GPIOD -> CFGLR &= ~(0xF<<16); // se borra configuracion por defecto
    GPIOD -> CFGLR |= GPIO_CFGLR_CNF4_1; // PD4 como entrada en modo pull-down por defecto
    uint8_t Cuenta=0;
    while(1){
        if ( GPIOD -> INDR & GPIO_INDR_IDR4 ) {
            GPIOC -> OUTDR = Cuenta++; // se envia el valor de cuenta hacia el puerto C
            Delay_Ms(250); // espera 500 ms para evitar rebotes   
        }

    }
}
