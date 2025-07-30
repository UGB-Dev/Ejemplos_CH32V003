/********************************** (C) COPYRIGHT *******************************
 * File Name          : main.c
 * Author             : Uriel Garnica
 * Version            : V1.0.0
 * Date               : 2023/12/25
 * Description        : Salida de display; visualizar el conteo en dos display usando
                        las entradas PD4 y PD3 los cuales serviran para incrementar o
                        decrementar la cuenta.
 *********************************************************************************/
 /*
    Configuracion en archivo sytem_ch32v00x.c
 
    comentar esta linea -> #define SYSCLK_FREQ_48MHz_HSE   48000000
    descomentar esta linea -> #define SYSCLK_FREQ_24MHZ_HSI   HSI_VALUE

    El MCU CH32V003 trabajara a 24 MHz y los perifericos a 24 MHz
 */

#include "debug.h"

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

    /*  CONFIGURACION DE PERIFERICOS  */
    RCC -> APB2PCENR |= RCC_IOPCEN | RCC_IOPDEN; // Se habilita el reloj del puerto C y D

    /*  CONFIGURACION DE LOS PINES DEL PUERTO C  */
    GPIOC -> CFGLR &= ~(GPIO_CFGLR_MODE | GPIO_CFGLR_CNF); // Borra configuraciones iniciales
    GPIOC -> CFGLR |= GPIO_CFGLR_MODE; // Puerto C como salida a 30 MHz en modo Push-Pull

    /*  CONFIGURACION DE LOS PINES DEL PUERTO D  */
    GPIOD -> CFGLR &= ~(GPIO_CFGLR_MODE0 | GPIO_CFGLR_CNF0 | GPIO_CFGLR_MODE2 | GPIO_CFGLR_CNF2 |
                        GPIO_CFGLR_MODE3 | GPIO_CFGLR_CNF3 | GPIO_CFGLR_MODE4 | GPIO_CFGLR_CNF4 ); // Borra configuraciones iniciales
    
    GPIOD -> CFGLR |= GPIO_CFGLR_CNF3_1 | GPIO_CFGLR_CNF4_1 | GPIO_CFGLR_MODE0 | GPIO_CFGLR_MODE2; // PD3 y PD4 como entrada en modo Pull-Down por defecto, PD0 y PD2 como salida a 30 MHz en modo Push-Pull

    uint8_t Cuenta=50; // variable local de control

    while(1){
        /* SE DETRMINA QUE EL PIN DE ENTRADA SEA PD3 O PD4 */
        if (GPIOD -> INDR & GPIO_INDR_IDR3) {
            while (GPIOD -> INDR & GPIO_INDR_IDR3); // Antirrebote 
            Cuenta--;
        }
        if (GPIOD -> INDR & GPIO_INDR_IDR4){
            while (GPIOD -> INDR & GPIO_INDR_IDR4); // Antirrebote 
            Cuenta++;
        }
        
        /* IMPRIME DECENAS Y UNIDADES */
        GPIOD -> BSHR = GPIO_BSHR_BS0 | GPIO_BSHR_BR2;
        GPIOC -> OUTDR = Display[Cuenta/10]; // Decenas
        Delay_Ms(50);
        
        GPIOD -> BSHR = GPIO_BSHR_BS2 | GPIO_BSHR_BR0;
        GPIOC -> OUTDR = Display[Cuenta%10]; // Unidades
        Delay_Ms(50);
        
        /* RESTABLECE LA VARIABLE CUENTA */
        if (Cuenta>99 || Cuenta == 0) {
            Cuenta=50;
        }
    }
}
