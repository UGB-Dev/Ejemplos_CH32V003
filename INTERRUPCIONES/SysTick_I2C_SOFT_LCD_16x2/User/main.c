/********************************** (C) COPYRIGHT *******************************
 * File Name          : main.c
 * Author             : Uriel Garnica
 * Version            : V1.0.0
 * Date               : 2023/12/25
 * Description        : LCD; Se genera el codigo para controlar el modulo PCF8574T el cual
                        permite controlar el LCD 16x2 mediante I2C para ahorrar pines del
                        MCU CH32V003.
 *********************************************************************************/
/*
    Configuracion en archivo sytem_ch32v00x.c
 
    comentar esta linea -> #define SYSCLK_FREQ_48MHz_HSE   48000000
    descomentar esta linea -> #define SYSCLK_FREQ_24MHZ_HSI   HSI_VALUE

    El MCU CH32V003 trabajara a 24 MHz y los perifericos a 24 MHz
*/

#include "debug.h"
#include "I2C_SOFTWARE.h"
#include "PCF8574T.h"

/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */
int main(void){
    Delay_Init();
    I2C_SOFT_Init(1000000); // Velocidad a 100 Kb/s
    
    LCD_Init();

    while(1){
        LCD_Cursor(0,0);
        LCD_Print_Str("Hola Mundo  :)");
        Delay_Ms(1000);
        LCD_Cursor(0,1);
        LCD_Print_Str("LCD y MCU CH32V003");
        Delay_Ms(1000);
    }
}