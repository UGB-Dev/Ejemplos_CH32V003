/********************************** (C) COPYRIGHT *******************************
 * File Name          : main.c
 * Author             : Uriel Garnica
 * Version            : V1.0.0
 * Date               : 2023/12/25
 * Description        : LCD 16x2: Se configuran los registros del puerto C para controlar
 *                      las lineas de datos hacia el LCD, de igual menera la configuracion
 *                      del LCD mediante el datasheet.
 *********************************************************************************/
/*
    Configuracion en archivo sytem_ch32v00x.c
 
    comentar esta linea -> #define SYSCLK_FREQ_48MHz_HSE   48000000
    descomentar esta linea -> #define SYSCLK_FREQ_24MHZ_HSI   HSI_VALUE

    El MCU CH32V003 trabajara a 24 MHz y los perifericos a 24 MHz
 */

#include "debug.h"
#include "LCD_16x2.h"

/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */
int main(void){
    Delay_Init();
    LCD_16x2_INIT();

    /* CONFIGURACION DEL PIN PC7 */
    GPIOC -> CFGLR &= ~(GPIO_CFGLR_MODE7 | GPIO_CFGLR_CNF7); // Borra configuraciones iniciales
    GPIOC -> CFGLR |= GPIO_CFGLR_MODE7; // PC7 como salida a 30 MHz en modo Push-Pull

    while(1){
        LCD_Clear();
        LCD_Print_Str("HOLA MUNDO");
        Delay_Ms(2000);

        LCD_Clear();
        LCD_Cursor(7,0);
        LCD_Print_Char('D');
        Delay_Ms(2000);

        LCD_Print_Char(49);
        Delay_Ms(2000);

        LCD_Clear();
        LCD_Print_Str("Numero = ");
        /* ENVIO DE LOS DATOS NUMERICOS HACIA EL LCD */
        for (uint16_t i=0; i<500; i++) {
            LCD_Cursor(9,0);
            LCD_Print_Int_Str(i);
            GPIOC -> OUTDR ^= GPIO_OUTDR_ODR7;
        } 
    }
}
