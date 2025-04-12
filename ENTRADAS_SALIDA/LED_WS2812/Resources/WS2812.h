/*

    WS2812.h

*/

#ifndef __WS2812_H
#define __WS2812_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ch32v00x.h"
#include <stdint.h>

/*
    TIEMPOS DE BIT 
    T0H  ==>  400 ns
    T1H  ==>  800 ns
    T0L  ==>  850 ns
    T1L  ==>  450 ns
    RES  ==>  >50 us
    
    
                                        ESTRUCTURA DE LA TRAMA (24 BITS) 
.----.----.----.----.----.----.----.----.----.----.----.----.----.----.----.----.----.----.----.----.----.----.----.----.
| G7 | G6 | G5 | G4 | G3 | G2 | G1 | G0 | R7 | R6 | R5 | R4 | R3 | R2 | R1 | R0 | B7 | B6 | B5 | B4 | B3 | B2 | B1 | B0 |
'----'----'----'----'----'----'----'----'----'----'----'----'----'----'----'----'----'----'----'----'----'----'----'----'
*/

#define VERDE       0x330000
#define AMARILLO    0xA5FF00
#define AZUL        0xE837F4
#define BLANCO      0x333333
#define amarillo    0
#define verde       1
#define azul        2
#define blanco      3


void ENVIAR(uint32_t DAT);
void Imprimir(uint8_t* DAT, uint16_t TIEMPO, uint32_t Color);
void Posicion(uint8_t Pos_x, uint8_t Pos_y, uint32_t Color);
void LineaV(uint8_t Pos_x0, uint8_t Pos_y0, uint8_t Pos_x1, uint32_t Color);
void LineaH(uint8_t Pos_x0, uint8_t Pos_y0, uint8_t Pos_y1, uint32_t Color);
void Borrar_Buffer(void);
void Imprimir_Color(uint8_t* DAT, uint16_t* Color);
void Posicion_Color(uint8_t Pos_x, uint8_t Pos_y, uint8_t Color);
void Activar_LED(uint8_t Pos_x, uint8_t Pos_y);
void Borrar_LED(uint8_t Pos_x, uint8_t Pos_y);

#ifdef __cplusplus
}
#endif

#endif /*__WS2812_H */


