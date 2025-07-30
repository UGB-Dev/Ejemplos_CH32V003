/*

    LCD_16x2.h

*/

#include "LCD_16x2.h"

void LCD_16x2_INIT(){
    
    /* CONFIGURACION DE PERIFERICO */
    RCC -> APB2PRSTR |= RCC_IOPCRST;
    RCC -> APB2PRSTR &= ~RCC_IOPCRST;
    RCC -> APB2PCENR |= RCC_IOPCEN;

    /* CONFIGURACION DEL PUERTO C COMO SALIDA */
    GPIOC -> CFGLR &= ~(0xFFFFFF << 0);
    GPIOC -> CFGLR |= GPIO_CFGLR_MODE0 | GPIO_CFGLR_MODE1 | GPIO_CFGLR_MODE2| 
                      GPIO_CFGLR_MODE3 | GPIO_CFGLR_MODE4 | GPIO_CFGLR_MODE5;
    
    /* CONFIGURACION DEL LCD PARA USAR 4 BITS*/
    Delay_Ms(40);

    Nibble_L_CMD(0x03);
    EN_PIN_DISABLE;
    Delay_Ms(5);

    Nibble_L_CMD(0x03);
    EN_PIN_DISABLE;
    Delay_Ms(100);
    
    Nibble_L_CMD(0x03);
    EN_PIN_DISABLE;
    Delay_Ms(100);

    Nibble_L_CMD(0x02);
    EN_PIN_DISABLE;
    Delay_Ms(100);

    /* CONFIGURACION DE PARAMETROS DEL LCD */
    Enviar_CMD_4_Bits(0x28); // entrada de 4 bits y 2 lineas
	Enviar_CMD_4_Bits(0x08); // cursor apagado
	Enviar_CMD_4_Bits(clear); // limpia el lcd
	Enviar_CMD_4_Bits(0x06); // modo incremento sin desplazamiento
	Enviar_CMD_4_Bits(ON_DISPLAY); // display encendido
}

void Enviar_CMD_4_Bits(uint8_t CMD){ // divide y envia el comando en 2 nibles
    Nibble_H_CMD(CMD); // se envia la parte alta del byte
	EN_PIN_DISABLE;
	Nibble_L_CMD(CMD);  // se envia la parte baja del byte
	EN_PIN_DISABLE;
	
	if (CMD == clear || CMD == home){ // si el comando es clear o home
		Delay_Ms(2); // espera 2 ms
	} 
	else{ // de lo contrario espera 60 ms
		Delay_Ms(60);
	}
}

void Enviar_DAT_4_Bits(uint8_t DAT){ // divide y envia el dato en 2 nibles
	Nibble_H_DAT(DAT); // se envia la parte alta del byte
	EN_PIN_DISABLE;
	Nibble_L_DAT(DAT); // se envia la parte baja del byte
	EN_PIN_DISABLE;
	Delay_Ms(60);
}

void LCD_Cursor(uint8_t X, uint8_t Y){ // posicion del cursor
	(Y == 0)? (X = 0x80 + X) : (X = 0xC0 + X);
	Enviar_CMD_4_Bits(X);
}

void LCD_Print_Str(const char* DAT){ // escribe la cadena de texto
	while(*DAT!=0){ // si no es diferente de cero
		Enviar_DAT_4_Bits(*DAT); // envia el caracter
		DAT++; // incrementa al siguiente caracter
	}
}

void LCD_Print_Char(uint8_t DAT){ // escribe un caracter
	Enviar_DAT_4_Bits(DAT); // envia elcaracter
}

void LCD_Clear(){ // limpia caracteres del LCD
	Enviar_CMD_4_Bits(clear); // /*

    LCD_16x2.h

*/

#include "LCD_16x2.h"

void LCD_16x2_INIT(){
    
    /* CONFIGURACION DE PERIFERICO */
    RCC -> APB2PCENR |= RCC_IOPCEN;

    /* CONFIGURACION DEL PUERTO C COMO SALIDA */
    GPIOC -> CFGLR &= ~(GPIO_CFGLR_MODE0 | GPIO_CFGLR_CNF0 | GPIO_CFGLR_MODE1 | GPIO_CFGLR_CNF1 | 
                        GPIO_CFGLR_MODE2 | GPIO_CFGLR_CNF2 | GPIO_CFGLR_MODE3 | GPIO_CFGLR_CNF3 | 
                        GPIO_CFGLR_MODE4 | GPIO_CFGLR_CNF4 | GPIO_CFGLR_MODE5 | GPIO_CFGLR_CNF5 ); // Borra configuraciones iniciales

    GPIOC -> CFGLR |= GPIO_CFGLR_MODE0 | GPIO_CFGLR_MODE1 | GPIO_CFGLR_MODE2| 
                      GPIO_CFGLR_MODE3 | GPIO_CFGLR_MODE4 | GPIO_CFGLR_MODE5; // PC0 a PC5 como salida a 30 MHz en modo Push-Pull
    
    /* CONFIGURACION DEL LCD PARA USAR 4 BITS*/
    Delay_Ms(40);

    Nibble_L_CMD(0x03);
    EN_PIN_DISABLE;
    Delay_Ms(5);

    Nibble_L_CMD(0x03);
    EN_PIN_DISABLE;
    Delay_Ms(100);
    
    Nibble_L_CMD(0x03);
    EN_PIN_DISABLE;
    Delay_Ms(100);

    Nibble_L_CMD(0x02);
    EN_PIN_DISABLE;
    Delay_Ms(100);

    /* CONFIGURACION DE PARAMETROS DEL LCD */
    Enviar_CMD_4_Bits(0x28); // Entrada de 4 bits y 2 lineas
    Enviar_CMD_4_Bits(0x08); // Cursor apagado
    Enviar_CMD_4_Bits(clear); // Limpia el lcd
    Enviar_CMD_4_Bits(0x06); // Modo incremento sin desplazamiento
    Enviar_CMD_4_Bits(ON_DISPLAY); // Display encendido
}

void Enviar_CMD_4_Bits(uint8_t CMD){ // Divide y envia el comando en 2 nibles
    Nibble_H_CMD(CMD); // Se envia la parte alta del byte
    EN_PIN_DISABLE;
    Nibble_L_CMD(CMD);  // Se envia la parte baja del byte
    EN_PIN_DISABLE;
	
    if (CMD == clear || CMD == home){ // Si el comando es clear o home
        Delay_Ms(2); // Espera 2 ms
    } 
    else{ // de lo contrario espera 60 ms
        Delay_Ms(60);
    }
}

void Enviar_DAT_4_Bits(uint8_t DAT){ // Divide y envia el DATo en 2 nibles
    Nibble_H_DAT(DAT); // Se envia la parte alta del byte
    EN_PIN_DISABLE;
    Nibble_L_DAT(DAT); // Se envia la parte baja del byte
    EN_PIN_DISABLE;
    Delay_Ms(60);
}

void LCD_Cursor(uint8_t X, uint8_t Y){ // Posicion del cursor
    (Y == 0)? (X = 0x80 + X) : (X = 0xC0 + X);
    Enviar_CMD_4_Bits(X);
}

void LCD_Print_Str(const char* DAT){ // escribe la cadena de texto
    while(*DAT!=0){ // Si no es diferente de cero
        Enviar_DAT_4_Bits(*DAT); // Envia el caracter
        DAT++; // Incrementa al siguiente caracter
    }
}

void LCD_Print_Char(uint8_t DAT){ // Escribe un caracter
    Enviar_DAT_4_Bits(DAT); // Envia elcaracter
}

void LCD_Clear(){ // Limpia caracteres del LCD
    Enviar_CMD_4_Bits(clear); // Envia comando clear
}

void LCD_Print_Int_Str(uint32_t NUMERO){
    uint8_t unidad=0;
    char BUFF[10];
    if(NUMERO == 0){
        LCD_Print_Char('0');
    }
    else {
        while (NUMERO > 0) {
            BUFF[unidad++]=NUMERO%10;
            NUMERO/=10;
        }
    }
    
    for (uint8_t i=unidad; i>0; i--) {
        LCD_Print_Char( BUFF[i-1] + '0');
    }
}envia comando clear
}

void LCD_Print_Int_Str(uint32_t NUMERO){
    uint8_t unidad=0;
    char BUFF[10]; 
    if(NUMERO == 0){
        LCD_Print_Char('0');
    }
    else {
        while (NUMERO > 0) {
            BUFF[unidad++]=NUMERO%10; 
            NUMERO/=10;
        }
    }
    
    for (uint8_t i=unidad; i>0; i--) {
        LCD_Print_Char( BUFF[i-1] + '0');
    }
}
