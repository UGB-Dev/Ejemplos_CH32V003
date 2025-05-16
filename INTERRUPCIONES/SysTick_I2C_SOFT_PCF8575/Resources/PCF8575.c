/*

    PCF8575.h

*/

#include "PCF8575.h"

void PCF8575_OUTPUT(uint8_t Reg_P0, uint8_t Reg_P1){
	Inicio_Trama(); //1
	I2C_SOFT_Enviar(ADDR_PCF575_W); // 8
	I2C_SOFT_Enviar(Reg_P0); // se envia direccion alta 8
	I2C_SOFT_Enviar(Reg_P1); // se envia direccion baja 8
	Fin_Trama(); //1 26*10ms =260ms
}