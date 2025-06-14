/********************************** (C) COPYRIGHT *******************************
 * File Name          : main.c
 * Author             : Uriel Garnica
 * Version            : V1.0.0
 * Date               : 2023/12/25
 * Description        : Escaner I2C: Se genera el escaneo de dispositivos I2C el cual
                        despues de ser capturado se envia por UART para visualizar la
						direccion de o los dispositivos conectados en el bus
 *********************************************************************************/
 
 /*
    Configuracion en archivo sytem_ch32v00x.c
 
    comentar esta linea -> #define SYSCLK_FREQ_48MHz_HSE   48000000
    descomentar esta linea -> #define SYSCLK_FREQ_24MHZ_HSI   HSI_VALUE

    El MCU CH32V003 trabajara a 24 MHz y los perifericos a 24 MHz
 */

#include "debug.h"
#include "UART_SOFTWARE.h"
#include "I2C_SOFTWARE.h"

const char hex[]={'A', 'B', 'C', 'D', 'E', 'F'};

#define PC1_ON()  GPIOC -> CFGLR &= ~(0xF<<4); \
                  GPIOC -> CFGLR |= GPIO_CFGLR_CNF1_0;

#define PC1_OFF() GPIOC -> CFGLR &= ~(0xF<<4); \
                  GPIOC -> CFGLR |= GPIO_CFGLR_MODE1; 

void inttohex(uint8_t dato){
	/* ENVIA LA PARTE ALTA DEL BYTE */
	if ((dato >> 4) > 9) {
		UART_SOFT_Char(hex[(dato >>4)-10]);
	}
	else {
		UART_SOFT_Char((dato >> 4)+'0');
	}

	/* ENVIA LA PARTE BAJA DEL BYTE */
	if ((dato & 0xF) > 9) {
		UART_SOFT_Char(hex[(dato & 0xF)-10]);
	}
	else {
		UART_SOFT_Char((dato & 0xF)+'0');
	}
}

/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */
int main(void){
    Delay_Init();
    UART_SOFT_Init(9600);
    I2C_SOFT_Init(100000);
    
    Delay_Ms(1000);
 
    GPIOC -> OUTDR &= ~GPIO_OUTDR_ODR1; // pin PC1 a GND
    uint8_t num;

    while(1){
        for (uint8_t i=1; i<128; i++){
            num = i;
            Inicio_Trama();
            I2C_SOFT_Enviar((i<<1));
            Fin_Trama();
            if (RECONOCIMIENTO == 0){
                UART_SOFT_Str("******************************************************\r\n\n");
                UART_SOFT_Str("La Direccion I2C es: ");
                UART_SOFT_Char(num/100 +48);
                num%=100;
                UART_SOFT_Char(num/10+48);
                UART_SOFT_Char(num%10+48);
                UART_SOFT_Str(" (0x");
                inttohex(i);
                UART_SOFT_Str(")\r\n\n");
                Delay_Ms(1000);
            }
            else {
                RECONOCIMIENTO=0;
            }
        } 

		/* TOGGLE DE 1 SEGUNDO EN PC1 */
        PC1_ON();
        Delay_Ms(500);
        PC1_OFF();
        Delay_Ms(500);
    }
}
