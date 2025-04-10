/********************************** (C) COPYRIGHT *******************************
 * File Name          : main.c
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2023/12/25
 * Description        : Interrupcion Externa; se generan tres secuencias que se ejecutaran
                        en tres pines configurados como interrupciones externas (PD0, PD3 y PD4), 
                        las secuencias se visualizaran en leds conectados en todo el puerto C.
 *********************************************************************************/
/*
    Configuracion en archivo sytem_ch32v00x.c
 
    comentar esta linea -> #define SYSCLK_FREQ_48MHz_HSE   48000000
    descomentar esta linea -> #define SYSCLK_FREQ_24MHZ_HSI   HSI_VALUE

    El MCU CH32V003 trabajara a 24 MHz y los perifericos a 24 MHz
 */

#include "debug.h"

/*  VARIABLES GLOBALES  */
volatile uint8_t Interupcion = 5;
const u_int8_t Secuencia1[]={ 0x40, 0x20, 0x10, 0x8, 0x4, 0x2, 0x1};
const u_int8_t Secuencia2[]={ 0x40, 0x22, 0x14, 0x8, 0x0};
const u_int8_t Secuencia3[]={ 0x79, 0x0, 0x79, 0x0, 0x79, 0x0, 0x4F, 0x0,  0x4F, 0x0, 0x4F, 0x0};

/*  PROTOTIPO DE FUNCION  */
void EXTI7_0_IRQHandler(void) __attribute__((interrupt));


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
    RCC -> APB2PRSTR |= RCC_IOPCRST | RCC_IOPDRST | RCC_AFIORST; // activacion de reset en puerto C, D y Funcion alternativa (Interrupcion)
    RCC -> APB2PRSTR &= ~(RCC_IOPCRST | RCC_IOPDRST | RCC_AFIORST); // desactivacion de reset en puerto C, D y Funcion alternativa (Interrupcion)
    RCC -> APB2PCENR |= RCC_IOPCEN | RCC_IOPDEN | RCC_AFIOEN; // se habilita el reloj de puerto C, D y Funcion alternativa (Interrupcion)

    /*  CONFIGURACION DE LOS PINES DEL PUERTO C  */
    GPIOC -> CFGLR = 0; // reset en todo el puerto C
    GPIOC -> CFGLR = 0x33333333; // Puerto C como salida push-pull a 30MHz

    /*  CONFIGURACION DE LOS PINES DEL PUERTO D  */
    GPIOD -> CFGLR &= ~(0xFF00F<<0); // se borra configuracion por defecto en PD0, PD3 y PD4
    GPIOD -> CFGLR |= GPIO_CFGLR_CNF0_1 | GPIO_CFGLR_CNF3_1 | GPIO_CFGLR_CNF4_1; // PD0, PD3 y PD4 como entrada en modo pull-down por defecto

    /*  CONFIGURACION DE PINES DE INTERRUPCION EXTERNA  */
    EXTI -> INTENR |= EXTI_INTENR_MR0 | EXTI_INTENR_MR3 | EXTI_INTENR_MR4; // se habilita la interrupcion para el pin PD0, PD3 y PD4
    EXTI -> FTENR |= EXTI_FTENR_TR0 | EXTI_FTENR_TR3 | EXTI_FTENR_TR4; // se ajusta la captura por flanco de bajada
    AFIO -> EXTICR |= AFIO_EXTICR1_EXTI0_PD | AFIO_EXTICR1_EXTI3_PD | AFIO_EXTICR1_EXTI4_PD; // se habilita la interrupcion externa en PD0, PD3 y PD4
    

    NVIC_EnableIRQ(EXTI7_0_IRQn); // se habilita la peticion de interrupcion externa de 0 a 7 independientemente del puerto A, C o D

    while(1){
        /*  SECUENCIA 1  */
        if (Interupcion == 0) {
            for (uint8_t t=0; t<7; t++) {
                GPIOC -> OUTDR = Secuencia1[t];
                Delay_Ms(500);
            }
        }

        /*  SECUENCIA 2  */
        if (Interupcion == 3) {
            for (uint8_t t=0; t<5; t++) {
                GPIOC -> OUTDR = Secuencia2[t];
                Delay_Ms(150);
            }
        }

        /*  SECUENCIA 3  */
        if (Interupcion == 4) {
            for (uint8_t t=0; t<12; t++) {
                GPIOC -> OUTDR = Secuencia3[t];
                Delay_Ms(150);
            }
        }  
    }
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
