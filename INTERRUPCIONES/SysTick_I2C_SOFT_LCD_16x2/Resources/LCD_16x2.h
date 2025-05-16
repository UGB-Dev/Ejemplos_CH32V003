/*

    LCD_16x2.h

*/


#ifndef __LCD_16X2_H
#define __LCD_16X2_H


#ifdef __cplusplus
extern "C" {
#endif

#include "ch32v00x.h"
#include <stdint.h>

/*  .------------------ ------------------.
    |  PINES LCD 16x2  |  PINES CH32V003  |
    |------------------|------------------|
    |        RS        |       PC0        |
    |------------------|------------------|
    |        EN        |       PC1        |
    |------------------|------------------|
    |        D4        |       PC2        |
    |------------------|------------------|
    |        D5        |       PC3        |
    |------------------|------------------|
    |        D6        |       PC4        |
    |------------------|------------------|
    |        D7        |       PC5        |
    '------------------ ------------------'
*/

/* ETIQUETAS PARA LOS PINES */
#define EN_PIN_DISABLE  (GPIOC -> BSHR = (GPIOC -> OUTDR & 0x3F)<< 16 )//(GPIOC -> BSHR = GPIO_BSHR_BR1)
#define Nibble_H_CMD(D) (GPIOC -> BSHR = EN_PIN | ((D & 0xF0)>>Data_Position))
#define Nibble_L_CMD(D) (GPIOC -> BSHR = EN_PIN | ((D & 0xF)<<Data_Position))
#define Nibble_H_DAT(D) (GPIOC -> BSHR = EN_PIN | RS_PIN | ((D & 0xF0)>>Data_Position))
#define Nibble_L_DAT(D) (GPIOC -> BSHR = EN_PIN | RS_PIN | ((D & 0xF)<<Data_Position))
#define Data_Position   0x02
#define RS_PIN          0x01
#define EN_PIN          0x02

/*COMANDOS DEL DISPLAY*/
#define ON_DISPLAY	    0x0C
#define clear	        0x01
#define home	        0x02

/*  FUNCIONES PROTOTIPO */
void LCD_16x2_INIT(void);
void Enviar_CMD_4_Bits(uint8_t CMD);
void Enviar_DAT_4_Bits(uint8_t DAT);
void LCD_Cursor(uint8_t X, uint8_t Y);
void LCD_Print_Str(const char* DAT);
void LCD_Print_Char(uint8_t DAT);
void LCD_Clear();
void LCD_Print_Int_Str(uint32_t NUMERO);

#ifdef  __cplusplus
}
#endif 

#endif //__LCD_16X2_H