/*

    UART_SOFTWARE.c

*/

#include "UART_SOFTWARE.h"

volatile uint8_t DAT_Copia=0, DAT_Pos=0; // variables de control para envio de datos
uint32_t Baud_Rate=0;
void (*Trama_Protocolo)(void);

/*********************************************************************
 * @fn      UART_SOFT_Init
 *
 * @brief   Pin PC7 for UART
 * @brief   8N1 defaul UART (8 bit, no parity, 1 stop bit)
 * @brief   Max BAUD RATE is 230400
 */
void UART_SOFT_Init(uint32_t Bauds){
    /*  ACTIVACION DE PERIFERICOS  */ 
    RCC -> APB2PCENR |= RCC_IOPCEN; // Se habilita el reloj del puerto C

    /*  CONFIGURACION DE LOS PINES DEL PUERTO C  */
    GPIOC -> CFGLR &= ~(GPIO_CFGLR_MODE1 | GPIO_CFGLR_CNF1); // Borra configuraciones iniciales 
    GPIOC -> CFGLR |= GPIO_CFGLR_MODE1; // PC1 como salida a 30 MHz en modo Push-Pull

    Baud_Rate = Bauds;
    UART_H; // Se mantiene en 1 logico la salida
}

void SysTick_Enable(void){
    SysTick -> SR &= ~(1<<0); // Borra bandera
    SysTick -> CMP = VALUE_CMP(Baud_Rate); // Valor de comparacion
    SysTick -> CTLR |= Systick_STRE | Systick_STIE | SysTick_STE; // Interrupcion y autorecarga del SisTick habilitada
    SysTick -> CNT = 0; // Contador inicia en cero
    NVIC_EnableIRQ(SysTicK_IRQn); // Se habilita la peticion de interrupcion externa para SysTick
}

void SysTick_Disable(void){
    SysTick -> CTLR = 0;
    NVIC_DisableIRQ(SysTicK_IRQn);
}

void UART_SOFT_Char(uint8_t DAT){
    Trama_Protocolo = Trama_UART;
    SysTick_Enable();
    DAT_Copia = DAT;
    DAT_Pos = 1;
    while (DAT_Pos);
    SysTick_Disable();
}

void UART_SOFT_Str(const char* DAT){
    while (*DAT != 0) {
        UART_SOFT_Char(*DAT++);
    }
}

void Trama_UART(void){
    switch(DAT_Pos){ // Switch UART
        case 1: 
        // Bit de start
            DAT_Pos++;
            UART_H;
            UART_L;
            break;

        case 2 ... 9:
        // Se enviar el bit menos significativo primero
            ( DAT_Copia & 0x01 )? UART_H : UART_L;
            DAT_Copia >>= 1;
            DAT_Pos++;
            break;
			
        case 10: // Bit de stop
            UART_H;
            DAT_Pos =0;
            break;
    }//Fin de switch
}

void SysTick_Handler(){ // Rutina de interrupcion SysTick
    Trama_Protocolo(); // Ejecuta el Protocolo de comunicacion
    SysTick -> SR = 0; // Borra la bandera para salir
}
