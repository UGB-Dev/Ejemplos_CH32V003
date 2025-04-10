/*

    UART_SOFTWARE.c

*/

#include "UART_SOFTWARE.h"

volatile uint8_t DAT_Copia=0, DAT_Pos=0; // variables de control para envio de datos

void SysTick_Enable(void){
    SysTick -> SR &= ~(1<<0); // se borra bandera 
    SysTick -> CMP = VALUE_CMP_us(416); // 416 us es para 2400 bauds
    SysTick -> CTLR = 0x0B;
    SysTick -> CNT = 0;
    NVIC_EnableIRQ(SysTicK_IRQn); // se habilita la peticion de interrupcion externa para SysTick
}

void SysTick_Disable(void){
    NVIC_DisableIRQ(SysTicK_IRQn); // se deshabilita la peticion de interrupcion externa para SysTick
}

void PORTC_init(void){
    /*  CONFIGURACION DEL PERIFERICO  */  
    RCC -> APB2PRSTR |= RCC_IOPCRST; // activacion de reset en puerto C
    RCC -> APB2PRSTR &= ~RCC_IOPCRST; // desactivacion de reset en puerto C
    RCC -> APB2PCENR |= RCC_IOPCEN; // se habilita el reloj de puerto C

    /*  CONFIGURACION DE LOS PINES DEL PUERTO C  */
    GPIOC -> CFGLR &= ~(0xF<<4); // reset en pin PC1
    GPIOC -> CFGLR |= GPIO_CFGLR_MODE1; // pin PC1 como salida push-pull a 30MHz

    UART_H; // se mantiene en 1 logico la salida
}

void UART_Char(uint8_t DAT){
    SysTick_Enable(); // se habilita la interrupcion del SysTick 
    DAT_Copia = DAT; 
    DAT_Pos = 1;
    while (DAT_Pos); // se mantiene hasta enviar el bit de parada
    SysTick_Disable(); // se deshabilita la interrupcion del SysTick 
}

void UART_Str(const char* DAT){
    /*  ENVIA LOS DATOS DE UN ARRAY*/
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

    SysTick -> SR = 0; // se borra la bandera para salir de la interrupcion
}
