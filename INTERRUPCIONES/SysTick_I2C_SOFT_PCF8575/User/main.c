/********************************** (C) COPYRIGHT *******************************
 * File Name          : main.c
 * Author             : Uriel Garnica
 * Version            : V1.0.0
 * Date               : 2023/12/25
 * Description        : PCF8575; Se crea el codigo necesario para expandir 16 pines de
                        entrada y salida usando I2C por Software.
 *********************************************************************************/
 /*
    Configuracion en archivo sytem_ch32v00x.c
 
    comentar esta linea -> #define SYSCLK_FREQ_48MHz_HSE   48000000
    descomentar esta linea -> #define SYSCLK_FREQ_24MHZ_HSI   HSI_VALUE

    El MCU CH32V003 trabajara a 24 MHz y los perifericos a 24 MHz
 */

#include "debug.h"
#include "I2C_SOFTWARE.h"
#include "PCF8575.h"

#define LED_BLUE 1

/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */
int main(void){
    Delay_Init();
    I2C_SOFT_Init(100000); // 100 Kb/s

    uint8_t Toggle=0;

    while(1){
        Toggle ^= (1<<(LED_BLUE-1));
        PCF8575_OUTPUT(Toggle, 0x80);
        Delay_Ms(500);
    }
}
