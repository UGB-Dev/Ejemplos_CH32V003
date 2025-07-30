/********************************** (C) COPYRIGHT *******************************
 * File Name          : main.c
 * Author             : Uriel_GB
 * Version            : V1.0.0
 * Date               : 2023/12/25
 * Description        : ADC.
 *********************************************************************************/
/*
    Configuracion en archivo sytem_ch32v00x.c
 
    comentar esta linea -> #define SYSCLK_FREQ_48MHz_HSE   48000000
    descomentar esta linea -> #define SYSCLK_FREQ_24MHZ_HSI   HSI_VALUE

    El MCU CH32V003 trabajara a 24 MHz y los perifericos a 24 MHz 
*/

#include "debug.h"
#include "UART_SOFTWARE.h"
#include "ADC.h"

/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */
int main(void){ 
    Delay_Init();
    UART_SOFT_Init(115200); // Velocidad a 115200 bauds
    ADC_Init_Simple();

    uint8_t buff_num[5] = {0}; // Buffer para almacenar el valor del ADC hacia UART

    while(1){
        uint16_t ADC_Value = ADC_Read_Simple();
        uint16_t AUX = ADC_Value;
        Delay_Ms(100);

        /* SEGMENTACION DEL VALOR ADC DEL CANAL 4 */
        for (uint8_t i=0 ; i<5 ; i++) {
            buff_num[i]=AUX%10 + 48;
            AUX/=10;
        }

        UART_SOFT_Str("##############################################################\r\n\n");

        /* ENVIO DEL VALOR ADC DEL CANAL 4 POR UART */
        UART_SOFT_Str("Valor ADC: ");
        for (uint8_t i=0 ; i<5 ; i++) {
            UART_SOFT_Char(buff_num[4-i]);
        }
        UART_SOFT_Str("    Valor de Voltaje: ");

        /* SEGMENTACION DEL VOLTAJE DEL CANAL 4 */
        AUX = (ADC_Value*3.3*1000)/1024;
        for (uint8_t i=0 ; i<5 ; i++) {
            if (i==3) {
                buff_num[i] = '.';
            }
            else {
                buff_num[i]=AUX%10 + 48;
                AUX/=10;
            }
        }

        /* ENVIO DEL VOLTAJE DEL CANAL 4 POR UART */
        for (uint8_t i=0 ; i<5 ; i++) {
            UART_SOFT_Char(buff_num[4-i]);
        }
        UART_SOFT_Str(" V\r\n");
    }
}

