/*

    UART_SOFTWARE.c

*/

#include "UART_SOFTWARE.h"

volatile uint8_t DAT_Copia=0, DAT_Pos=0; // variables de control para envio de datos
uint32_t Baud_Rate=0;

void SysTick_Enable(void){
    SysTick -> SR &= ~(1<<0); // BORRAR BANDERA 
    SysTick -> CMP = VALUE_CMP_us(Baud_Rate);
    SysTick -> CTLR = 0x0B;
    SysTick -> CNT = 0;
    NVIC_EnableIRQ(SysTicK_IRQn);
}

void SysTick_Disable(void){
    SysTick -> CTLR = 0;
    NVIC_DisableIRQ(SysTicK_IRQn);
}

/*********************************************************************
 * @fn      UART_SOFT_Init
 *
 * @brief   pin PC7 for UART
 *          8N1 defaul UART (8 bit, no parity, 1 stop bit)
 *
 * @return  none
 */
void UART_SOFT_Init(uint32_t Bauds){
    /*  Configuracion del periferico ( puerto C ) */  
    RCC -> APB2PRSTR |= RCC_IOPCRST;
    RCC -> APB2PRSTR &= ~RCC_IOPCRST;
    RCC -> APB2PCENR |= RCC_IOPCEN; 

    GPIOC -> CFGLR &= ~(0xF<<28);
    GPIOC -> CFGLR |= GPIO_CFGLR_MODE7;

    Baud_Rate = 1000000/Bauds;
    UART_H; // se mantiene en 1 logico la salida
}

void UART_Char(uint8_t DAT){
    SysTick_Enable();
    DAT_Copia = DAT;
    DAT_Pos = 1;
    while (DAT_Pos);
    SysTick_Disable();
}

void UART_Str(const char* DAT){
    while (*DAT != 0) {
        UART_Char(*DAT++);
    }
}

void SysTick_Handler(){ // Rutina de interrupcion SysTick
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

    SysTick -> SR = 0; // BORRAR BANDERA PARA SALIR 
}
