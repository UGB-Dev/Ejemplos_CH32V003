/********************************** (C) COPYRIGHT *******************************
 * File Name          : main.c
 * Author             : Uriel Garnica
 * Version            : V1.0.0
 * Date               : 2023/12/25
 * Description        : ADTM; Se configuran los registros necesarios para que el Timer 1
 *                      funcione en modo input capture (IC), el cual servira para generar
 *                      un conteo de 0 a 99 ascendente y descendente el cual podra observarse 
 *                      en dos display de 8 segmentos mediante el puerto C y mediante un
 *                      monitor serial.                     
 *********************************************************************************/
  /*
    Configuracion en archivo sytem_ch32v00x.c
 
    comentar esta linea -> #define SYSCLK_FREQ_48MHz_HSE   48000000
    descomentar esta linea -> #define SYSCLK_FREQ_24MHZ_HSI   HSI_VALUE

    El MCU CH32V003 trabajara a 24 MHz y los perifericos a 24 MHz
 */

#include "debug.h"
#include "TIM1_COUNTER_MODE.h"
#include "UART_SOFTWARE.h"
#include "Display_Segment.h"


/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */
int main(void){
    Delay_Init();
    UART_SOFT_Init(115200); // UART en pin PC7
    Counter_Mode_Init();
    Set_Cuenta(99, Flanco_Bajada); // Cuenta de 0 a 99
    Display_Segment_Init();

    /* VARIABLES DE CONTROL PARA ENVIAR DATOS POR UART */
    uint16_t CONTEO = 0, Aux_Conteo = 0;
    uint8_t uart[4] = {0};

    while(1){
        /* CAMBIA EL SENTIDO DEL CONTADOR */
        if (GPIOD ->INDR & GPIO_INDR_IDR4) {
            while (GPIOD ->INDR & GPIO_INDR_IDR4); // Antirrebote
            TIM1 -> CTLR1 ^= TIM_DIR; // Contador UP - DOWN 
        }

        CONTEO = Aux_Conteo = TIM1 -> CNT ; // Se captura el valor del registro CNT
        
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
        Imprimir(Aux_Conteo);
    }
}
