/********************************** (C) COPYRIGHT *******************************
 * File Name          : main.c
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2023/12/25
 * Description        : UART por software y DHT11; configuracion  del pin PC7 (DHT11) y
                        pin PC1 (SALIDA UART) para capturar la temperatura y Humedad 
                        del sensor la cual se podra observar en la computadora.
 *********************************************************************************/
/*
    Configuracion en archivo sytem_ch32v00x.c
 
    comentar esta linea -> #define SYSCLK_FREQ_48MHz_HSE   48000000
    descomentar esta linea -> #define SYSCLK_FREQ_24MHZ_HSI   HSI_VALUE

    El MCU CH32V003 trabajara a 24 MHz y los perifericos a 24 MHz

    NOTA: No usar el pin PC7 ya que se usa para controlar el sensor DHT11  
 */
 
#include "debug.h"
#include "UART_SOFTWARE.h"
#include "DHT_11.h"

uint8_t TEMP[2], HUME[2] ;

/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */
int main(void){

    Delay_Init();
    PORTC_init();
    
    uint8_t ERROR_T=0, ERROR_H=0;
    while(1){
        ERROR_T = DHT_11(TEMP, Temperatura);
        Delay_Ms(500);
        ERROR_H = DHT_11(HUME, Humedad);
        Delay_Ms(500);

        /*  DETERMINA SI LA TRANSMISION DE DATOS FUE EXITOSA */
        if ( (ERROR_H == 1) && (ERROR_T == 1)) {
            UART_Str("TEMPERATURA: ");
            UART_Char( (TEMP[0]/10) +'0'); //Se envia la Decena
            UART_Char( (TEMP[0]%10) + '0'); // Se envia la Unidad
            UART_Char( 32 ); // Caracter espacio
            UART_Char(176); // Caracter de бу
            UART_Str("C\r\n");

            UART_Str("HUMEDAD: ");
            UART_Char( (HUME[0]/10) +'0'); //Se envia la Decena
            UART_Char( (HUME[0]%10) + '0'); // Se envia la Unidad
            UART_Str("\r\n\n");
        }
        else {
            UART_Str(" !ERROR DE COMUNICACION! \r\n\n");
        } 
    }
}
