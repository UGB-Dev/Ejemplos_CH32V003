/*

    PCF8575.h

*/

#ifndef __PCF8575_H__
#define __PCF8575_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "I2C_SOFTWARE.h"
#include "stdint.h"

///////////////////////////////////////////////////////////////////
//////////////////  REGISTROS DE EEPROM  ////////////////////////

#define ADDR_PCF575_W 0x40  // Direccion + Escritura
#define ADDR_PCF575_R 0x41	// Direccion + Lectura

///////////////////////////////////////////////////////////////////
////////////////////  FUNCIONES PROTOTIPO  ////////////////////////

void PCF8575_OUTPUT(uint8_t Reg_P0, uint8_t Reg_P1);

#ifdef __cplusplus
 }
#endif //__PCF8575_H__
#endif




