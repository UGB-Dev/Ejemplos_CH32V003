/********************************** (C) COPYRIGHT *******************************
 * File Name          : main.c
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2023/12/25
 * Description        : Leds WS2812; Se realiza la trama de bits con las especificaciones
                        descritas en su hoja de datos mediante bit-banging para simular
                        el llenado de un recipiente.
 *********************************************************************************/
 /*
    Configuracion en archivo sytem_ch32v00x.c
 
    comentar esta linea -> #define SYSCLK_FREQ_48MHz_HSE   48000000
    descomentar esta linea -> #define SYSCLK_FREQ_24MHZ_HSI   HSI_VALUE

    El MCU CH32V003 trabajara a 24 MHz y los perifericos a 24 MHz
 */

 /* DETERMINAR EL TIEMPO DE INSTRUCCION USANDO EL REGISTRO BSHR Y OUTDR */
 /* PARA BSHR  1 Y 0 LOGICO SON 83.5 ns 
    PARA OUTDR 1 Y 0 LOGICO SON 208.5 ns (deb usarse operadores logicos)
    LA INSTRUCCION asm nop SON 41.5 ns                                               */

#include "debug.h"
#include "WS2812.h"

uint8_t Secuencia_llenado[] = { 5, 6, 7, 8,
                               3, 2, 1, 4 };

                            
/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */
int main(void){
    Delay_Init();

    /*ACTIVACION DE PERIFERICOS*/
    RCC -> APB2PRSTR |= RCC_IOPCRST; // activar reset en puerto C
    RCC -> APB2PRSTR &= ~RCC_IOPCRST; // desactivar reset en puerto C
    RCC -> APB2PCENR |= RCC_IOPCEN; // se habilita el reloj del puerto C

    /*CONFIGURACION DE LOS PINES DEL PUERTO C */
    GPIOC -> CFGLR &= ~(0xF<<4); // reset en el pin PC1
    GPIOC -> CFGLR |= GPIO_CFGLR_MODE1; // Pin PC1 como salida push-pull a 30 MHz

    /*  COLOR AZUL EN TODA LA MATRIZ 8*8  */
    for (uint8_t i=1; i<9; i++){
        Posicion_Color(i,1,azul);
        Posicion_Color(i,2,azul);
        Posicion_Color(i,3,azul);
        Posicion_Color(i,4,azul);
        Posicion_Color(i,5,azul);
        Posicion_Color(i,6,azul);
        Posicion_Color(i,7,azul);
        Posicion_Color(i,8,azul);
    }
    
    Borrar_Buffer(); 

    /*  COLOR AMARILLO EN LA ESQUINA SUPERIOR DERECHA  */
    Posicion_Color(6,1, amarillo);
    Posicion_Color(7,1, amarillo);
    Posicion_Color(8,1, amarillo);
    Posicion_Color(7,2, amarillo);
    Posicion_Color(8,2, amarillo);
    Posicion_Color(8,3, amarillo);

    uint8_t y=9, x=0, aux_Conteo=0;

    while(1){
        /*  SE GENERA LA CAIDA DE AGUA  */
        for (uint8_t i=1; i<y; i++){
            Activar_LED(4,i);
            Delay_Ms(150);
            Borrar_LED(4,i);
        }
        
        /*  SECUENCIA DE LLENADO CADA 4 SEGUNDOS */
        if (aux_Conteo == 3) {
            aux_Conteo = 0;
            Activar_LED(Secuencia_llenado[x++], y-1);
            /*  AJUSTA LAS VARIABLES "x" Y "y" DESPUES DE LLENAR LA FILA  */
            if (x == 8){ 
                x=0;    --y;
            } 
            /*  SE BORRA EL BUFFER DESPUES DE TRES FILAS LLENAS DE AGUA  */
            if (y == 6) {
                y=9;    Borrar_Buffer();

                /*  SE ACTIVAN LOS LEDS EN LA ESQUINA SUPERIOR DERECHA  */
                Activar_LED(6,1);
                Activar_LED(7,1);
                Activar_LED(8,1);
                Activar_LED(7,2);
                Activar_LED(8,2);
                Activar_LED(8,3);
            }
        }
        else ++aux_Conteo;
    }
}
