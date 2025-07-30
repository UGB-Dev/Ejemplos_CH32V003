/********************************** (C) COPYRIGHT *******************************
 * File Name          : main.c
 * Author             : Uriel Garnica
 * Version            : V1.0.0
 * Date               : 2023/12/25
 * Description        : ADXL345; Se genera la trama I2C por software que usa el sensor ADXL345 
                        para enviar y recibir los datos que estan o estaran alojados en sus
                        registros para el correcto funcionamiento, de igual manera se usa UART
                        por software para enviar los valores de los ejex X, Y y Z para visualizarlos
                        en un terminal serial llamado Termite
 *********************************************************************************/
/*
    Configuracion en archivo sytem_ch32v00x.c
 
    comentar esta linea -> #define SYSCLK_FREQ_48MHz_HSE   48000000
    descomentar esta linea -> #define SYSCLK_FREQ_24MHZ_HSI   HSI_VALUE

    El MCU CH32V003 trabajara a 24 MHz y los perifericos a 24 MHz
*/

#include "debug.h"
#include "I2C_SOFTWARE.h"
#include "UART_SOFTWARE.h"
#include "ADXL345.h"

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
    I2C_SOFT_Init(100000); // velocidad a 100 Kb/s

    Escribir_Registro(POWER_CTL, 0x08); // Activacion del ADXL345

    while(1){
        UART_SOFT_Str("########################################################\n");
        UART_SOFT_Str("Ejes Sensor ADXL345\r\n\n");
        UART_SOFT_Str("Eje X= ");
        Capturar_Eje(DATAX0);
        UART_SOFT_Str("  EJE Y= ");
        Capturar_Eje(DATAY0);
        UART_SOFT_Str("  EJE Z= ");
        Capturar_Eje(DATAZ0);
        UART_SOFT_Str("\n########################################################");
        UART_SOFT_Str("\r\n\n");
        Delay_Ms(350);
    }
}
