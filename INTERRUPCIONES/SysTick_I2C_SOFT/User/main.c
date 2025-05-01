/********************************** (C) COPYRIGHT *******************************
 * File Name          : main.c
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2023/12/25
 * Description        : I2C por software: Se genera la trama I2C usando la tecnica de
                        bit-banging e interrupcion del SysTick a una velocidad de 100 a 200 Kb/s
                        para escanear la direccion del OLED SSD1306 (0x3C) y mediante un led 
                        en el pin PC1 poder visualizar que la comunicacion I2C fue exitosa.
 *********************************************************************************/
/*
    Configuracion en archivo sytem_ch32v00x.c
 
    comentar esta linea -> #define SYSCLK_FREQ_48MHz_HSE   48000000
    descomentar esta linea -> #define SYSCLK_FREQ_24MHZ_HSI   HSI_VALUE

    El MCU CH32V003 trabajara a 24 MHz y los perifericos a 24 MHz
 */

#include "debug.h"
#include "I2C_SOFTWARE.h"

/* */
#define PC1_ON()  GPIOC -> CFGLR &= ~(0xF<<4); \
                  GPIOC -> CFGLR |= GPIO_CFGLR_CNF1_0;

#define PC1_OFF() GPIOC -> CFGLR &= ~(0xF<<4); \
                  GPIOC -> CFGLR |= GPIO_CFGLR_MODE1; 

/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */
int main(void){
    Delay_Init();

    I2C_SOFT_Init(100000); // inicializacion del I2C a 100 Kb/s

    GPIOC -> OUTDR &= ~GPIO_OUTDR_ODR1; // PC1 con nivel logico 0
    PC1_OFF();
    while(1){

        /* ESCANEO DE 128 DIRECCIONES EN EL BUS I2C */
        for (uint8_t i=1; i<128; i++){
			Inicio_Trama();
			I2C_SOFT_Enviar((i<<1));
			Fin_Trama();
            
            /* TOOGLE EN PC1 SI LA DIRECCION I2C ES 60 (SSD1306) */
			if ( (RECONOCIMIENTO == 0 ) && (i==60)){
                PC1_ON();
                Delay_Ms(500);
                PC1_OFF();
                Delay_Ms(500);
			}
			else {
                RECONOCIMIENTO=0;
			}
		}       
    }
}
