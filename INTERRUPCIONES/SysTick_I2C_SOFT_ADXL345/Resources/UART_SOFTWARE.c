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
 * @brief   pin PC5 for UART,
 *          8N1 defaul UART (8 bit, no parity, 1 stop bit)
 *
 * @return  none
 */
void UART_SOFT_Init(uint32_t Bauds){
    Baud_Rate = 1000000/Bauds;
    
    /*  CONFIGURACION DEL PERIFERICO  */  
    RCC -> APB2PCENR |= RCC_IOPCEN; // se habilita el reloj de puerto C

    /*  CONFIGURACION DE LOS PINES DEL PUERTO C  */
    GPIOC -> CFGLR &= ~(0xF<<20); // reset en pin PC5
    GPIOC -> CFGLR |= GPIO_CFGLR_MODE5; // pin PC5 como salida push-pull a 30MHz

    UART_H; // se mantiene en 1 logico la salida
}

void SysTick_Enable(uint32_t CMP_){
    SysTick -> SR &= ~(1<<0); // se borra bandera 
    SysTick -> CMP = VALUE_CMP_us(CMP_);
    SysTick -> CTLR = 0x0B;
    SysTick -> CNT = 0;
    NVIC_EnableIRQ(SysTicK_IRQn); // se habilita la peticion de interrupcion externa para SysTick
}

void SysTick_Disable(void){
    SysTick -> CTLR = 0;
    NVIC_DisableIRQ(SysTicK_IRQn); // se deshabilita la peticion de interrupcion externa para SysTick
}


void UART_SOFT_Char(uint8_t DAT){
    Trama_Protocolo = Trama_UART;
    SysTick_Enable(Baud_Rate); // se habilita la interrupcion del SysTick 
    DAT_Copia = DAT; 
    DAT_Pos = 1;
    while (DAT_Pos); // se mantiene hasta enviar el bit de parada
    SysTick_Disable(); // se deshabilita la interrupcion del SysTick 
}

void UART_SOFT_Str(const char* DAT){
    /*  ENVIA LOS DATOS DE UN ARRAY*/
    while (*DAT != 0) {
        UART_SOFT_Char(*DAT++);
    }
}

void Trama_UART(void){
    switch(DAT_Pos){ // switch UART
        case 1: 
        // bit de start
            DAT_Pos++;
            UART_H;
            UART_L;
            break;

        case 2 ... 9:
        // se enviar el bit menos significativo primero
            ( DAT_Copia & 0x01 )? UART_H : UART_L;
            DAT_Copia >>= 1;
            DAT_Pos++;
            break;
			
        case 10: // bit de stop
            UART_H;
            DAT_Pos =0;
            break;
    }//fin de switch
}

