/********************************** (C) COPYRIGHT *******************************
 * File Name          : main.c
 * Author             : Uriel Garnica
 * Version            : V1.0.0
 * Date               : 2023/12/25
 * Description        : GPTM; Se ajustan los registros asociados al Timer 2 para generar
 *                      un PWM de 50 Hz el cual controlara un servomotor en el pin PD3.
 *********************************************************************************/

#include "debug.h"
#include "TIM2_PWM.h"

#define Derecha 3 // (0.5ms *100) / 20ms
#define Izquierda 12 // (2.5ms * 100) / 20ms
#define Centro 7 // (1.5ms * 100) / 20ms

/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */
int main(void){
    Delay_Init(); 
    PWM_Init(PWM1_MODE); 

    while(1){
        PWM_Set(50, Derecha, 10000); // Servomotor giro hacia la Derecha
        Delay_Ms(2000);
        PWM_Set(50,Centro,10000); // Servomotor giro hacia el centro
        Delay_Ms(2000);
        PWM_Set(50, Izquierda, 10000); // Servomotor giro hacia la Izquierda
        Delay_Ms(2000);
        PWM_Set(50,Centro,10000); // Servomotor giro hacia el centro
        Delay_Ms(2000);
        
    }
}
