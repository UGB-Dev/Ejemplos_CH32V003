/*

    OLED_SSD1306.h

*/

#ifndef __OLED_SSD1306_H__
#define __OLED_SSD1306_H__

#ifdef __cplusplus
 extern "C" {
#endif

#include "ch32v00x.h"
#include "I2C_SOFTWARE.h"

extern const uint8_t Config_OLED[];
extern const uint8_t OLED_FONT[];

void Dato_(void);
void Comando_(void);
void OLED_Init(void);
void OLED_Clear(void);
void OLED_Print_Str(const char* DATA);
void OLED_Cursor(uint8_t xpos, uint8_t ypos);
void OLED_Print_Pixel(uint8_t Pos_x, uint8_t Pos_y);

#ifdef __cplusplus
 }
#endif //__OLED_SSD1306_H__
#endif