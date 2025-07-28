/********************************** (C) COPYRIGHT *******************************
 * File Name          : main.c
 * Author             : Uriel Garnica
 * Version            : V1.0.0
 * Date               : 2023/12/25
 * Description        : ADTM; Se ajustan los registros asociados al Timer 1 para trabajar
 *                      en modo de captura PWM en los pines PA1 y PD2, los valores de captura
 *                      se observaran en un monitor serial mediante el uso de UART por software.
 *********************************************************************************/

#include "debug.h"
#include "TIM1_PWM_INPUT_MODE.h"
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
    PWM_Input_Mode_Init();
    UART_SOFT_Init(115200);

    uint16_t Value_CH1CVR = 0, Value_CH2CVR = 0, Freq = 0;
    uint8_t Aux_UART[5], Duty = 0;

    while(1){

        Value_CH1CVR = TIM1 -> CH1CVR;
        Value_CH2CVR = TIM1 -> CH2CVR;
        Freq = SystemCoreClock  / Value_CH1CVR;
        Duty = (Value_CH2CVR*100)/Value_CH1CVR;

//################################################################################## 
//######################  SE ENVIA EL VALOR DE LOS REGISTROS  ######################   

        /* DESCOMPONER EL VALOR CH1CVR */
        for (uint8_t i=0; i<5; i++) {
            Aux_UART[i] = Value_CH1CVR%10 + 48;
            Value_CH1CVR /=10;
        }

        /* ENVIA EL VALOR CH1CVR AL UART */  // 48000
        UART_SOFT_Str("Valor CH1CVR(PERIODO T): ");
        for (uint8_t i=0; i<5; i++) {
            UART_SOFT_Char(Aux_UART[4-i]);
        }
        UART_SOFT_Str("\r\n");

        /* DESCOMPONER EL VALOR CH2CVR */ 
        for (uint8_t i=0; i<5; i++) {
            Aux_UART[i] = Value_CH2CVR%10 + 48;
            Value_CH2CVR /=10;
        }

        /* ENVIA EL VALOR CH2CVR AL UART */ // 24000
        UART_SOFT_Str("Valor CH2CVR(Duty): ");
        for (uint8_t i=0; i<5; i++) {
            UART_SOFT_Char(Aux_UART[4-i]);
        }
        UART_SOFT_Str("\r\n");

//##################################################################################
//########################  SE ENVIA EL VALOR DEL PWM  ############################# 

        /* DESCOMPONER EL VALOR DE LA FRECUENCIA */
        for (uint8_t i=0; i<5; i++) {
            Aux_UART[i] = Freq%10 + 48;
            Freq /=10;
        }

        /* ENVIA EL VALOR DE LA FRECUENCIA AL UART */  
        UART_SOFT_Str("Frecuencia: ");
        for (uint8_t i=0; i<5; i++) {
            UART_SOFT_Char(Aux_UART[4-i]);
        }
        UART_SOFT_Str(" Hz\r\n");

        /* DESCOMPONER EL VALOR DEL DUTY CYCLE */
        for (uint8_t i=0; i<3; i++) {
            Aux_UART[i] = Duty%10 + 48;
            Duty /=10;
        }

        /* ENVIA EL VALOR DEL DUTY CYCLE AL UART */  
        UART_SOFT_Str("Duty Cycle: ");
        for (uint8_t i=0; i<3; i++) {
            UART_SOFT_Char(Aux_UART[2-i]);
        }
        UART_SOFT_Str(" %\r\n");
        UART_SOFT_Str("##################################################\r\n");
        TIM1 -> CNT = 0;
        Delay_Ms(1000);
    }
}
