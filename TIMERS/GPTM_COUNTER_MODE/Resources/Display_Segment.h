/*

    Display_Segment.h

*/

#ifndef __Display_Segment_H__
#define __Display_Segment_H__

#ifdef __cplusplus
 extern "C" {
#endif

#include "ch32v00x.h"


extern uint8_t Display[];

void Display_Segment_Init(void);
void Imprimir(uint8_t Cont);

#ifdef __cplusplus
 }
#endif //__Display_Segment_H__
#endif