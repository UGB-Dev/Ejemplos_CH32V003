/*

    PCF8574T.c

*/

#include "I2C_SOFTWARE.h"
#include "PCF8574T.h"

void Envio_Dat_Com_(uint8_t DATA){
    Inicio_Trama();
    I2C_SOFT_Enviar(ADDR_LCD_);
    I2C_SOFT_Enviar(DATA); // envia 0x00 para comandos
    Fin_Trama();
}

void Enviar_4_bits_CMD(uint8_t bits){
    /*uint8_t byte_LOW, byte_upper;
	
    byte_LOW = (bits<<4)|EN_|LED;
    byte_upper = (bits & 0xF0)|EN_|LED;*/
	
    Envio_Dat_Com_((bits & 0xF0)|EN_|LED);
    Envio_Dat_Com_((bits & 0xF0)|LED); // envia 0x00 para comandos
    Envio_Dat_Com_((bits<<4)|EN_|LED);
    Envio_Dat_Com_((bits<<4)|LED);
	
    if (bits == clear || bits == home){ Delay_Ms(2); } 
    else{ Delay_Ms(60); }
}

void Enviar_4_bits_DAT(uint8_t bits){
    /*uint8_t byte_LOW, byte_upper;
	
    byte_LOW = (bits<<4)|EN_|RS_|LED;
    byte_upper = (bits & 0xF0)|EN_|RS_|LED;*/
	
    Envio_Dat_Com_((bits & 0xF0)|EN_|RS_|LED);
    Envio_Dat_Com_((bits & 0xF0)|RS_|LED); // envia 0x00 para comandos
    Envio_Dat_Com_((bits<<4)|EN_|RS_|LED);
    Envio_Dat_Com_((bits<<4)|RS_|LED);
    //_delay_us(60);
}

void LCD_Init(){
    Delay_Ms(40);
	
    Envio_Dat_Com_(0x30|EN_);
    Envio_Dat_Com_(0x30);
    Delay_Ms(5);
	
    Envio_Dat_Com_(0x30|EN_);
    Envio_Dat_Com_(0x30);
    Delay_Ms(100);
	
    Envio_Dat_Com_(0x30|EN_);
    Envio_Dat_Com_(0x30);
    Delay_Ms(100);
	
    Envio_Dat_Com_(0x20|EN_);
    Envio_Dat_Com_(0x20);
    Delay_Ms(100);
	
    Enviar_4_bits_CMD(0x28); // Func set 4 bits 2 lines
    Enviar_4_bits_CMD(0x08); // Display off cursor
    Enviar_4_bits_CMD(clear); // LCD clear
    Enviar_4_bits_CMD(0x06); // Sin desplazamiento
    Enviar_4_bits_CMD(ON_DISPLAY);
}

void LCD_Clear(void){
    Enviar_4_bits_CMD(clear);
}

void LCD_Cursor(uint8_t xpos, uint8_t ypos){
    uint8_t DIR_MEM;
	
    switch(ypos){
        case 1: DIR_MEM = 0xC0; break;
        default: DIR_MEM = 0x80; break;
    }

    Enviar_4_bits_CMD(DIR_MEM+xpos);	
}

void LCD_ON_OFF(uint8_t on_off){
}

void LCD_Print_Str(const char *dat){
    while(*dat!=0){
        Enviar_4_bits_DAT(*dat);
        dat++;
    }
}

void LCD_Print_Char(uint8_t dat){
    Enviar_4_bits_DAT(dat);
}