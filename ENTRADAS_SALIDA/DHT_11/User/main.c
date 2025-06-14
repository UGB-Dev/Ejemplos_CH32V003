/********************************** (C) COPYRIGHT *******************************
 * File Name          : main.c
 * Author             : Uriel Garnica
 * Version            : V1.0.0
 * Date               : 2023/12/25
 * Description        : DHT-11; generar el codigo para controlar el sensor de temperatura
                        y humedad.
 *********************************************************************************/
/*
    Configuracion en archivo sytem_ch32v00x.c
 
    comentar esta linea -> #define SYSCLK_FREQ_48MHz_HSE   48000000
    descomentar esta linea -> #define SYSCLK_FREQ_24MHZ_HSI   HSI_VALUE

    El MCU CH32V003 trabajara a 24 MHz y los perifericos a 24 MHz

    NOTA: No usar el pin PC7 ya que se usa para controlar el sensor DHT11  
 */

#include "debug.h"
#include "DHT_11.h"

/*  VARIABLES GLOBALES  */
uint8_t TEMP[2], HUME[2] ;

/*  PROTOTIPO DE FUNCION  */
void PuertoC_Init(void);


/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */
int main(void){
    Delay_Init();
    PuertoC_Init();

    while(1){
         Delay_Ms(500);

         /*  DETERMINA SI LA TRANSMISION DE DATOS FUE EXITOSA */
        if (DHT_11(TEMP, Temperatura) == 1) {

            /*  ACTIVACION DE PC1 SI LA TEMPERATURA ES MAYOR A 25 GRADOS*/
            if(TEMP[0] > 25){
                GPIOC -> BSHR = GPIO_BSHR_BS1; // PC1 a 1 logico
            }
            else{
                GPIOC -> BSHR = GPIO_BSHR_BR1; // PC1 a 0 logico
            }
        }
        else {
            GPIOC -> OUTDR ^= GPIO_OUTDR_ODR1; // parpadeo en PC1
        }
    }
}

void PuertoC_Init(void){
    /*  CONFIGURACION DE PERIFERICOS  */
    RCC -> APB2PRSTR |= RCC_IOPCRST; // activacion de reset en puerto C
    RCC -> APB2PRSTR &= ~RCC_IOPCRST; // desactivacion de reset en puerto C
    RCC -> APB2PCENR |= RCC_IOPCEN; // se habilita el reloj de puerto C

    /*  CONFIGURACION DE LOS PINES DEL PUERTO C  */
    GPIOC -> CFGLR &= ~(0xF<<4); // reset en el pin PC1
    GPIOC -> CFGLR |= GPIO_CFGLR_MODE1; // Pin PC1 como salida
}
