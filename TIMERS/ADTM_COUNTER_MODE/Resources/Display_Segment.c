/*

    Display_Segment.h

*/

#include "Display_Segment.h"
#include "UART_SOFTWARE.h"


uint8_t Display[]={ 0x7E, 0x30, 0x6D, 0x79, 0x33, 0x5B, 0x5F, 0x70, 0x7F, 0x73 }; // Codificacion de display catodo comun

void Display_Segment_Init(void){
    /* ACTIVACION DE PERIFERICOS */
    RCC -> APB2PCENR |= RCC_IOPCEN | RCC_IOPDEN ; // Habilitacion del reloj en puerto C y D

    /* CONFIGURACION DEL PIN PD2 Y PD3 */
    GPIOD -> CFGLR &= ~( GPIO_CFGLR_CNF2 | GPIO_CFGLR_MODE2 | 
                         GPIO_CFGLR_CNF3 | GPIO_CFGLR_MODE3 |
                         GPIO_CFGLR_CNF4 | GPIO_CFGLR_MODE4 ); // Borra configuraciones iniciales

    GPIOD -> CFGLR |=  GPIO_CFGLR_MODE2 | GPIO_CFGLR_MODE3 | //Pin PD2 y PD3 como salida a 30 MHz en Push-Pull
                       GPIO_CFGLR_CNF4_1; //  PD4 como entrada Pull-Down

    /* CONFIGURACION DE TODO EL PUERTO C */
    GPIOC -> CFGLR &= ~(GPIO_CFGLR_CNF | GPIO_CFGLR_MODE); // Borra configuraciones iniciales
    GPIOC -> CFGLR |=  GPIO_CFGLR_MODE; // Todo el puerto C como salida a 30 MHz en Push-Pull
}

void Imprimir(uint8_t Cont){
    for (uint8_t i=0; i< 8; i++ ) { //delay 500 ms por cuenta
        GPIOC -> OUTDR = Display[Cont/10] | GPIO_OUTDR_ODR7; // DECENAS
        GPIOD -> BSHR = GPIO_BSHR_BS2 | GPIO_BSHR_BR3;
        Delay_Ms(4); 

        GPIOC -> OUTDR = Display[Cont%10] | GPIO_OUTDR_ODR7; // UNIDADES
        GPIOD -> BSHR = GPIO_BSHR_BS3 | GPIO_BSHR_BR2;
        Delay_Ms(4);
    }  
}