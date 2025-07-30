/********************************** (C) COPYRIGHT *******************************
 * File Name          : main.c
 * Author             : Uriel Garnica
 * Version            : V1.0.0
 * Date               : 2023/12/25
 * Description        : Display de 7 segmentos; imprimir en el puerto C los numeros 
                        del 0 al 9 de manera ascendente o descendente mediante PD4 
                        o PD3 respectivamente 
 *********************************************************************************/
  
  /*
    Configuracion en archivo sytem_ch32v00x.c
 
    comentar esta linea -> #define SYSCLK_FREQ_48MHz_HSE   48000000
    descomentar esta linea -> #define SYSCLK_FREQ_24MHZ_HSI   HSI_VALUE

    El MCU CH32V003 trabajara a 24 MHz y los perifericos a 24 MHz
 */

#include "debug.h"

#define descendente 1
#define ascendente 2

uint8_t Display[]={ 0x7E, 0x30, 0x6D, 0x79, 0x33, 0x5B, 0x5F, 0x70, 0x7F, 0x73 }; // Codificacion de display catodo comun 

/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */
int main(void){
    Delay_Init();

    /* CONFIGURACION DE PERIFERICOS */
    RCC -> APB2PCENR |= RCC_IOPCEN | RCC_IOPDEN; // Se habilita el reloj de puerto C y D

    /* CONFIGURACION DE LOS PINES DEL PUERTO C */
    GPIOC -> CFGLR &= ~(GPIO_CFGLR_MODE | GPIO_CFGLR_CNF); // Borra configuraciones iniciales
    GPIOC -> CFGLR |= GPIO_CFGLR_MODE; // Puerto C como salida a 30 MHz en modo Push-Pull

    /* CONFIGURACION DE LOS PINES DEL PUERTO D */
    GPIOD -> CFGLR &= ~(GPIO_CFGLR_MODE3 | GPIO_CFGLR_CNF3 | 
                        GPIO_CFGLR_MODE4 | GPIO_CFGLR_CNF4 );  // Borra configuraciones iniciales
    GPIOD -> CFGLR |= GPIO_CFGLR_CNF3_1 | GPIO_CFGLR_CNF4_1; // PD3 y PD4 como entrada en modo pull-down por defecto

    uint8_t Cuenta=0, Flag_modo_display=0; // variables locales de control

    while(1){
        /* SE DETRMINA QUE EL PIN DE ENTRADA SEA PD3 O PD4 */
        if (GPIOD -> INDR & GPIO_INDR_IDR3){
            while (GPIOD -> INDR & GPIO_INDR_IDR3); // Antirrebote 
            Flag_modo_display = descendente;
            Cuenta = 9;
        }
        else if (GPIOD -> INDR & GPIO_INDR_IDR4){
            while (GPIOD -> INDR & GPIO_INDR_IDR4); // Antirrebote
            Flag_modo_display = ascendente;
            Cuenta = 0;
        }

        GPIOC -> OUTDR = Display[Cuenta % 10]; // Envia el valor de cuenta hacia el puerto C
 
        /* PARA LA CUENTA DESPUES DE LLEGAR AL LIMITE INFERIOR O SUPERIOR */
        if ( ( (Flag_modo_display == ascendente) && (Cuenta == 9) ) || ( (Flag_modo_display == descendente) && (Cuenta == 0) ) ) {
            Flag_modo_display = 0;
        }

        /* INCREMENTA LA VARIABLE CUENTA SEGUN SEA EL CASO (ASCENDENTE O DESCENDENTE) */ 
        if (Flag_modo_display == ascendente) {
            Cuenta++;
            Delay_Ms(500);
        }
        else if (Flag_modo_display == descendente){
           Cuenta--;
           Delay_Ms(500);
        }   
    }
} 
