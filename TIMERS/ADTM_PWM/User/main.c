/********************************** (C) COPYRIGHT *******************************
 * File Name          : main.c
 * Author             : Uriel Garnica
 * Version            : V1.0.0
 * Date               : 2023/12/25
 * Description        : ADTM(TIM1); Usar los registros necesarios para el modo de salida PWM
 *                      en el pin PA1.
 *********************************************************************************/
/*
    Configuracion en archivo sytem_ch32v00x.c
 
    comentar esta linea -> #define SYSCLK_FREQ_48MHz_HSE   48000000
    descomentar esta linea -> #define SYSCLK_FREQ_24MHZ_HSI   HSI_VALUE

    El MCU CH32V003 trabajara a 24 MHz y los perifericos a 24 MHz
 */

#include "debug.h"
#include "Timer1_PWM.h"


/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */
int main(void){
    Delay_Init();
    PWM_Init();

    uint8_t i=1;
    while(1){
        PWM_Set(60, i++, 1000); // F=50Hz; i=Duty_cycle; Base_Time=1KHz=1ms
        Delay_Ms(10);

        if (i==99) {
            i=1;
        }

    }
}

