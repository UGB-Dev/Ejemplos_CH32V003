/*

    PCA9685.h

*/

#ifndef __PCA9685_H__
#define __PCA9685_H__

#ifdef __cplusplus
 extern "C" {
#endif

#include "stdint.h"
#include "stdbool.h"
#include "I2C_SOFTWARE.h"

///////////////////////////////////////////////////////////////////
/////////////////////  VARIABLES GLOBALES  ////////////////////////

#define ADDR_PCA9685_W 0x80 // direccion + escritura 
#define MODE1 0x00
#define PRE_SCALE 0xFE
#define LED0_ON_L 0x06

#define _POS 0
#define _NEG 1

///////////////////////////////////////////////////////////////////
////////////////////  FUNCIONES PROTOTIPO  ////////////////////////

void PCA9685_SERVO_INIT(void);
void PCA9685_SERVO_GRADOS(uint16_t Grado, bool signo, uint8_t Canal);
void PCA9685_SERVO_SIMPLE(uint8_t REG, uint8_t Valor);


#ifdef __cplusplus
 }
#endif //__PCA9685_H__
#endif
