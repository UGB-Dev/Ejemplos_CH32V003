/********************************** (C) COPYRIGHT *******************************
 * File Name          : main.c
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2023/12/25
 * Description        : Tx 433 MHz: Generar la trama de datos usando UART y codificacion
                        Manchester para transmitir datos usando los modulos de 433 MHz.
 *********************************************************************************/
/*
    Configuracion en archivo sytem_ch32v00x.c
 
    comentar esta linea -> #define SYSCLK_FREQ_48MHz_HSE   48000000
    descomentar esta linea -> #define SYSCLK_FREQ_24MHZ_HSI   HSI_VALUE

    El MCU CH32V003 trabajara a 24 MHz y los perifericos a 24 MHz 
*/

#include "debug.h"
#include "UART_SOFTWARE.h"
#include "Modulo_433MHz.h"

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
        for (uint8_t i=0; i<10; ++i){
            Trama_433_MHz("HOLA MUNDO, UART POR SOFTWARE", 1); // Trama codificada para modulo 433 MHz
        }
        
        UART_SOFT_Str("\r\nHOLA MUNDO, UART POR SOFTWARE\r\n");
        Delay_Ms(500);
    }
}
