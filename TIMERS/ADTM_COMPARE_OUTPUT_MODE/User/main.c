/********************************** (C) COPYRIGHT *******************************
 * File Name          : main.c
 * Author             : Uriel Garnica
 * Version            : V1.0.0
 * Date               : 2023/12/25
 * Description        : ADTM; Se configuran los registros necesarios para que el Timer 1
*                       trabaje en modo Output Compare (OC) que servira para trabajar en 
*                       modo FIP, PWM1 y PWM2.
 *********************************************************************************/
  /*
    Configuracion en archivo sytem_ch32v00x.c
 
    comentar esta linea -> #define SYSCLK_FREQ_48MHz_HSE   48000000
    descomentar esta linea -> #define SYSCLK_FREQ_24MHZ_HSI   HSI_VALUE

    El MCU CH32V003 trabajara a 24 MHz y los perifericos a 24 MHz
 */

#include "debug.h"
#include "TIM1_OUTPUT_COMPARE.h"


/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */
int main(void){
    Output_Compare_Init(PWM2_MODE); 

    while(1){
    }
}
