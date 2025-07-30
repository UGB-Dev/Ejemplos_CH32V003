/********************************** (C) COPYRIGHT *******************************
 * File Name          : main.c
 * Author             : Uriel Garnica
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
#include "EXTERNAL_INTERRUPT.h"

/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */
int main(void){
    Delay_Init();
    Extern_Interrupt_Init();
    
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
