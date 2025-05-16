/*

    PCF8574T.h

*/


#ifndef __PCF8574T_H
#define __PCF8574T_H


#ifdef __cplusplus
extern "C" {
#endif

#define ADDR_LCD_ 0x4E // direccion del modulo i2c 01001110 ==>0x4E
#define RS_ 0x01
#define EN_ 0x04
#define LED 0x08
#define ON_DISPLAY	0x0C
#define clear	0x01
#define home	0x02

void Envio_Dat_Com_(uint8_t DATA);
void enviar_4_bits_CMD(uint8_t bits);
void enviar_4_bits_DAT(uint8_t bits);
void LCD_Init();
void LCD_Clear(void);
void LCD_Cursor(uint8_t xpos, uint8_t ypos);
void LCD_Print_Str(const char *dat);
void LCD_Print_Char(uint8_t dat);


#ifdef  __cplusplus
}
#endif 

#endif //__PCF8574T_H