/********************************** (C) COPYRIGHT *******************************
 * File Name          : main.c
 * Author             : Uriel Garnica
 * Version            : V1.0.0
 * Date               : 2023/12/25
 * Description        : SSD1306; Se genera el codigo necesario para controlar el display
                        de 128*64 pixeles.
 *********************************************************************************/
/*
    Configuracion en archivo sytem_ch32v00x.c
 
    comentar esta linea -> #define SYSCLK_FREQ_48MHz_HSE   48000000
    descomentar esta linea -> #define SYSCLK_FREQ_24MHZ_HSI   HSI_VALUE

    El MCU CH32V003 trabajara a 24 MHz y los perifericos a 24 MHz
 */

#include "debug.h"
#include "I2C_SOFTWARE.h"
#include "OLED_SSD1306.h"

/*  */

 

/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */
int main(void){
    Delay_Init();

    I2C_SOFT_Init(100000);
    OLED_init();
    Delay_Ms(1000);

    uint8_t cuenta=0;
    
    OLED_clear();
    while(1){
        
       // OLED_cursor(0,0);
       // Print_OLED("!HOLA MUNDO! :)");
        OLED_Print_Pixel(cuenta, cuenta);
        //Delay_Ms(100);
        if (cuenta == 63) {
            cuenta = 0;
            OLED_clear();
        }
        else {
            ++cuenta;
        }
        
    }
}
