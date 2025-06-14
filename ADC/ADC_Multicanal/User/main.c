/********************************** (C) COPYRIGHT *******************************
 * File Name          : main.c
 * Author             : Uriel Garnica
 * Version            : V1.0.0
 * Date               : 2023/12/25
 * Description        : ADC; Usar los registros necesarios para convertir el nivel analogico
                        del canal 4 y 6 a un nivel logico que sera transmitido mediante el
                        protocolo UART por software.
 *********************************************************************************/

#include "debug.h"
#include "ADC.h"
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
    UART_SOFT_Init(115200); // UART a 115200 bauds
    ADC_Init_Multiple();

    uint8_t buff_num[5] = {0}; // Buffer para almacenar el valor del ADC
    uint16_t AUX=0; // Variable auxiliar para segmentar el valor ADC

    while(1){
        ADC_Read_Multiple(); 

        Delay_Ms(150);
        
        /* SEGMENTACION DEL VALOR ADC DEL CANAL 4 */
        AUX = ADC_VALUE[0];
        for (uint8_t i=0 ; i<5 ; i++) {
            buff_num[i]=AUX%10 + 48;
            AUX/=10;
        }
        UART_Str("##############################################################\r\n\n");

        /* ENVIO DEL VALOR ADC DEL CANAL 4 POR UART */
        UART_Str("Valor ADC CH4: ");
        for (uint8_t i=0 ; i<5 ; i++) {
            UART_Char(buff_num[4-i]);
        }
        UART_Str("\r\n");

        /* SEGMENTACION DEL VALOR ADC DEL CANAL 6 */
        AUX = ADC_VALUE[1];
        for (uint8_t i=0 ; i<5 ; i++) {
            buff_num[i]=AUX%10 + 48;
            AUX/=10;
        }

        /* ENVIO DEL VALOR ADC DEL CANAL 6 POR UART */
        UART_Str("Valor ADC CH6: ");
        for (uint8_t i=0 ; i<5 ; i++) {
            UART_Char(buff_num[4-i]);
        }
        UART_Str("\r\n\n");
    }
}



 


