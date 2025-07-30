/*

    DHT_11.h

*/

#ifndef __DHT_11_H__
#define __DHT_11_H__

#ifdef __cplusplus
 extern "C" {
#endif

#include "ch32v00x.h"

#define Temperatura 1
#define Humedad     2

uint8_t DHT_11(uint8_t* Buff, uint8_t medicion);

#ifdef __cplusplus
 }
#endif // __DHT_11_H__
#endif
