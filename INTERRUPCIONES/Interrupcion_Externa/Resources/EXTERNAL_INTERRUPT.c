/*

    EXTERNAL_INTERRUPT.c

*/

#include "EXTERNAL_INTERRUPT.h"

volatile uint8_t Interupcion=5;
const uint8_t Secuencia1[]={ 0x40, 0x20, 0x10, 0x8, 0x4, 0x2, 0x1};
const uint8_t Secuencia2[]={ 0x40, 0x22, 0x14, 0x8, 0x0};
const uint8_t Secuencia3[]={ 0x79, 0x0, 0x79, 0x0, 0x79, 0x0, 0x4F, 0x0,  0x4F, 0x0, 0x4F, 0x0};

void Extern_Interrupt_Init(void){
    /*  CONFIGURACION DE PERIFERICOS  */
    RCC -> APB2PCENR |= RCC_IOPCEN | RCC_IOPDEN | RCC_AFIOEN; // Se habilita el reloj del puerto C, D y Funcion alternativa (Interrupcion)

    /*  CONFIGURACION DE LOS PINES DEL PUERTO C  */
    GPIOC -> CFGLR &= ~(GPIO_CFGLR_MODE | GPIO_CFGLR_CNF); // Borra configuraciones iniciales
    GPIOC -> CFGLR |= GPIO_CFGLR_MODE; // Puerto C como salida a 30 MHz en modo Push-Pull

    /*  CONFIGURACION DE LOS PINES DEL PUERTO D  */
    GPIOD -> CFGLR &= ~(GPIO_CFGLR_MODE0 | GPIO_CFGLR_CNF0 | 
                        GPIO_CFGLR_MODE3 | GPIO_CFGLR_CNF3 |
                        GPIO_CFGLR_MODE4 | GPIO_CFGLR_CNF4 ); // Borra configuraciones iniciales
    
    GPIOD -> CFGLR |= GPIO_CFGLR_CNF0_1 | GPIO_CFGLR_CNF3_1 | GPIO_CFGLR_CNF4_1 ; // PD0, PD3 y PD4 como entrada en modo Pull-Down por defecto

    /*  CONFIGURACION DE PINES DE INTERRUPCION EXTERNA  */
    EXTI -> INTENR |= EXTI_INTENR_MR0 | EXTI_INTENR_MR3 | EXTI_INTENR_MR4; // Se habilita la interrupcion para el pin PD0, PD3 y PD4
    EXTI -> FTENR |= EXTI_FTENR_TR0 | EXTI_FTENR_TR3 | EXTI_FTENR_TR4; // Se ajusta la captura por flanco de bajada
    AFIO -> EXTICR |= AFIO_EXTICR1_EXTI0_PD | AFIO_EXTICR1_EXTI3_PD | AFIO_EXTICR1_EXTI4_PD; // Se habilita la interrupcion externa en PD0, PD3 y PD4
    
    NVIC_EnableIRQ(EXTI7_0_IRQn); // Se habilita la peticion de interrupcion externa de 0 a 7 independientemente del puerto A, C o D
}

void EXTI7_0_IRQHandler(void){
    /*  DETERMINA SI LA INTERRUPCION FUE EN PD0  */
    if (EXTI -> INTFR & EXTI_INTF_INTF0) {
        Interupcion = 0;
        EXTI -> INTFR = EXTI_INTF_INTF0;
    }
    
    /*  DETERMINA SI LA INTERRUPCION FUE EN PD3  */
    if (EXTI -> INTFR & EXTI_INTF_INTF3) {
        Interupcion = 3;
        EXTI -> INTFR = EXTI_INTF_INTF3;
    }

    /*  DETERMINA SI LA INTERRUPCION FUE EN PD4  */
    if (EXTI -> INTFR & EXTI_INTF_INTF4) {
        Interupcion = 4;
        EXTI -> INTFR = EXTI_INTF_INTF4; 
    }
}