/********************************** (C) COPYRIGHT *******************************
 * File Name          : main.c
 * Author             : Uriel Garnica
 * Version            : V1.0.0
 * Date               : 2023/12/25
 * Description        : Interrupcion SysTick; usar el reloj del sistema para generar
                        la base de tiempo de UART por software.
 *********************************************************************************/
 /*
    Configuracion en archivo sytem_ch32v00x.c
 
    comentar esta linea -> #define SYSCLK_FREQ_48MHz_HSE   48000000
    descomentar esta linea -> #define SYSCLK_FREQ_24MHZ_HSI   HSI_VALUE

    El MCU CH32V003 trabajara a 24 MHz y los perifericos a 24 MHz
 */

#include "debug.h"
#include "UART_SOFTWARE.h"

/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */

int main(void){
    Delay_Init();
    UART_SOFT_Init(2400);

    while(1){
        UART_Str("HOLA MUNDO\r\n");
        //Delay_Ms(500);
        UART_Str("ESTO ES UN MENSAJE DE UART POR SOFTWARE A 115200 BAUDS\r\n");
        //Delay_Ms(500);
        UART_Str("  :)  \r\n\n");
        Delay_Ms(500);
    }
}

