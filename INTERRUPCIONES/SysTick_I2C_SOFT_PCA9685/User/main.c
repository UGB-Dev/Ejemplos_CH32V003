/********************************** (C) COPYRIGHT *******************************
 * File Name          : main.c
 * Author             : Uriel Garnica
 * Version            : V1.0.0
 * Date               : 2023/12/25
 * Description        : PCA9685; Se genera el codigo de acuerdo al datasheet del PCA9685
                        para generar el PWM que se usara para controlar servo motores SG90
                        mediante I2C por software.
 *********************************************************************************/
  /*
    Configuracion en archivo sytem_ch32v00x.c
 
    comentar esta linea -> #define SYSCLK_FREQ_48MHz_HSE   48000000
    descomentar esta linea -> #define SYSCLK_FREQ_24MHZ_HSI   HSI_VALUE

    El MCU CH32V003 trabajara a 24 MHz y los perifericos a 24 MHz
 */

#include "debug.h"
#include "I2C_SOFTWARE.h"
#include "PCA9685.h"


/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */
int main(void){
    Delay_Init();
    I2C_SOFT_Init(100000);
    PCA9685_SERVO_INIT();

    while(1){
        for (uint16_t i=0; i<360; i+=4){
            PCA9685_SERVO_GRADOS(i, _NEG, 0);
            Delay_Ms(10);
        }
        for (uint16_t i=0; i<360; i+=4){
            PCA9685_SERVO_GRADOS(i, _NEG, 1);
            Delay_Ms(10);
        }

        for (uint16_t i=0; i<360; i+=4){
            PCA9685_SERVO_GRADOS(360-i, _NEG, 0);
            Delay_Ms(10);
        }

        for (uint16_t i=0; i<360; i+=4){
            PCA9685_SERVO_GRADOS(360-i, _NEG, 1);
            Delay_Ms(10);
        }

        for (uint16_t i=0; i<360; i+=4){
            PCA9685_SERVO_GRADOS(i, _POS, 0);
            Delay_Ms(10);
        }
        for (uint16_t i=0; i<360; i+=4){
            PCA9685_SERVO_GRADOS(i, _POS, 1);
            Delay_Ms(10);
        }

        for (uint16_t i=0; i<360; i+=4){
            PCA9685_SERVO_GRADOS(360-i, _POS, 0);
            Delay_Ms(10);
        }

        for (uint16_t i=0; i<360; i+=4){
            PCA9685_SERVO_GRADOS(360-i, _POS, 1);
            Delay_Ms(10);
        }
    }
}
