/********************************** (C) COPYRIGHT *******************************
 * File Name          : main.c
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2023/12/25
 * Description        : Display de 7 segmentos; multiplexado de dos display para el
                        conteo de 0 a 99 donde la salida de los datos es todo el puerto
                        C y el control es mediante los pines PD0 (DECENAS) y PD2 (UNIDADES),
                        solo se ejecutara una sola vez mediante PD4 (BOTON)
 *********************************************************************************/
/*
    Configuracion en archivo sytem_ch32v00x.c
 
    comentar esta linea -> #define SYSCLK_FREQ_48MHz_HSE   48000000
    descomentar esta linea -> #define SYSCLK_FREQ_24MHZ_HSI   HSI_VALUE

    El MCU CH32V003 trabajara a 24 MHz y los perifericos a 24 MHz
 */

#include "debug.h"

//codificacion de display catodo comun
uint8_t Display[]={ 0x7E, 0x30, 0x6D, 0x79, 0x33, 0x5B, 0x5F, 0x70, 0x7F, 0x73 }; 

/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */
int main(void){
    Delay_Init();

    /*  CONFIGURACION DE PERIFERICOS  */
    RCC -> APB2PRSTR |= RCC_IOPCRST | RCC_IOPDRST; // activacion de reset en puerto C y D
    RCC -> APB2PRSTR &= ~(RCC_IOPCRST | RCC_IOPDRST); // desactivacion de reset en puerto C y D
    RCC -> APB2PCENR |= RCC_IOPCEN | RCC_IOPDEN; // se habilita el reloj de puerto C y D

    /*  CONFIGURACION DE LOS PINES DEL PUERTO C  */
    GPIOC -> CFGLR = 0; // reset en todo el puerto C
    GPIOC -> CFGLR = 0x33333333; // Puerto C como salida push-pull a 30MHz

    /*  CONFIGURACION DE LOS PINES DEL PUERTO D  */
    GPIOD -> CFGLR &= ~(0xF0F0F<<0); // se borra configuracion por defecto en PD0, PD2 y PD4
    GPIOD -> CFGLR |= GPIO_CFGLR_CNF4_1 | GPIO_CFGLR_MODE0 | GPIO_CFGLR_MODE2; // PD4 como entrada en modo pull-down por defecto, PD0 y PD2 como salida push-pull a 30MHz

    uint8_t Cuenta=0, Flag_Cuenta=0; // variables locales de control

    while(1){
        // condicional para determinal el pin de entrada (PD4)
        if ( (Flag_Cuenta==0) && (GPIOD -> INDR & GPIO_INDR_IDR4)) {
            Flag_Cuenta = 1;
        }
        
        // condicional para el multiplexado
        if (Flag_Cuenta == 1) {
            for (uint8_t i=0; i< 31; i++ ) { //delay 500 ms por cuenta
                GPIOC -> OUTDR = Display[Cuenta/10]; // DECENAS
                GPIOD -> BSHR = GPIO_BSHR_BS0 | GPIO_BSHR_BR2;
                Delay_Ms(4); 

                GPIOC -> OUTDR = Display[Cuenta%10]; // UNIDADES
                GPIOD -> BSHR = GPIO_BSHR_BS2 | GPIO_BSHR_BR0;
                Delay_Ms(4);
            }  
            Cuenta++; 
        }
        
        // condicional para reiniciar variables
        if (Cuenta>99) {
            Cuenta = Flag_Cuenta = 0;
            GPIOD -> BSHR = GPIO_BSHR_BR0| GPIO_BSHR_BR2;
        }
       

    }
}
