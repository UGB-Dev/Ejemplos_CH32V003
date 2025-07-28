/********************************** (C) COPYRIGHT *******************************
 * File Name          : main.c
 * Author             : Uriel Garnica
 * Version            : V1.0.0
 * Date               : 2023/12/25
 * Description        : ADTM; Se configuran los registros asociados del timer 1 para
 *                      usar el modo encoder en los pines PA1(CH2) y PD0(CH1) el cual
 *                      genera un conteo de 0 a 1000 ascendente o descendente y podra
 *                      ser observado en un monitor serial.
 *********************************************************************************/
 /*
    Configuracion en archivo sytem_ch32v00x.c
 
    comentar esta linea -> #define SYSCLK_FREQ_48MHz_HSE   48000000
    descomentar esta linea -> #define SYSCLK_FREQ_24MHZ_HSI   HSI_VALUE

    El MCU CH32V003 trabajara a 24 MHz y los perifericos a 24 MHz
 */

#include "debug.h"
#include "TIM1_ENCODER_MODE.h"
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
    Encoder_Mod_Init();
    UART_SOFT_Init(9600); // UART en pin PC7 

    /* VARIABLES DE CONTROL PARA ENVIAR DATOS POR UART */
    uint16_t CONTEO = 10;
    uint8_t uart[4] = {0};

    while(1){
        CONTEO = TIM1 -> CNT ; // Se captura el valor del registro CNT

        UART_SOFT_Str("Valor de CNT: ");

        /* SE DIVIDE CADA UNIDAD DE LA VARIABLE CONTEO */
        for (uint8_t t=0; t<4; t++) {
            uart[t] = CONTEO % 10 + 48;
            CONTEO /= 10;
        }

        /* SE ENVIA EL VALOR DE LA VARIABLE CONTEO POR UART*/
        for (uint8_t t=0; t<4; t++) {
            UART_SOFT_Char(uart[3-t]);
        }
        UART_SOFT_Str(" \r\n");
        
        Delay_Ms(150);
    }
}
