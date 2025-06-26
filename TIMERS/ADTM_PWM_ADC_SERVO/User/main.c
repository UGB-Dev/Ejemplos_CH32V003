/********************************** (C) COPYRIGHT *******************************
 * File Name          : main.c
 * Author             : Uriel Garnica
 * Version            : V1.0.0
 * Date               : 2023/12/25
 * Description        : ADTM y ADC; Se usaran los archivos de cabecera PWM y ADC para
 *                      manipular un servomotor mediante un potenciometro, los datos del
 *                      ADC y Duty cycle se observaran mediante un terminal serial.
 *********************************************************************************/
/*
    Configuracion en archivo sytem_ch32v00x.c
 
    comentar esta linea -> #define SYSCLK_FREQ_48MHz_HSE   48000000
    descomentar esta linea -> #define SYSCLK_FREQ_24MHZ_HSI   HSI_VALUE

    El MCU CH32V003 trabajara a 24 MHz y los perifericos a 24 MHz
 */

#include "debug.h"
#include "ADC.h"
#include "Timer1_PWM.h"
#include "UART_SOFTWARE.h"

#define Fpwm 50  // 50 Hz para la frecuencia del PWM
#define Fbase 1000000 // 1 MHz para la base de tiempo 

/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */
int main(void){
    Delay_Init();
    ADC_Init_Simple();
    PWM_Init();
    UART_SOFT_Init(115200); 

    uint16_t ADC_Value = 0, aux_adc=0,PWM_Duty = 0;
    uint8_t aux_duty=0, Buff_UART[4]={0};

    while(1){

        aux_adc = ADC_Value = ADC_Read_Simple();
        aux_duty = PWM_Duty = ( (ADC_Value * 9) / 1024 ) + 3 ; // Rango del PWM es de 3 a 12 ( 0.5ms a 2.5 ms)
        PWM_Set(Fpwm, PWM_Duty, Fbase); // Se asigna el valor del PWM

        /* SEGMENTACION DEL VALOR ADC */
        for (uint8_t iteracion=0; iteracion<4; iteracion++) {
            Buff_UART[iteracion] = aux_adc % 10 +48;
            aux_adc /=10;
        }

        /* ENVIO DEL VALOR ADC POR UART */
        UART_SOFT_Str("ADC: ");
        for (uint8_t iteracion=0; iteracion<4; iteracion++) {
            UART_SOFT_Char(Buff_UART[3-iteracion]);
        }

        /* SEGMENTACION DEL DUTY CYCLE */
        for (uint8_t iteracion=0; iteracion<3; iteracion++) {
            Buff_UART[iteracion] = aux_duty % 10 +48;
            aux_duty /=10;
        }

        /* ENVIO DEL DUTY CYCLE POR UART */
        UART_SOFT_Str("\r\nDuty Cycle: ");
        for (uint8_t iteracion=0; iteracion<3; iteracion++) {
            UART_SOFT_Char(Buff_UART[2-iteracion]);
        }

        UART_SOFT_Str(" %\r\n");
        UART_SOFT_Str("#################################################################\r\n\n");

        Delay_Ms(150);
    }
}
