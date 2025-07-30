/*

    PCF8575.h

*/

#include "PCF8575.h"

void PCF8575_OUTPUT(uint8_t Reg_P0, uint8_t Reg_P1){
    Inicio_Trama();
    I2C_SOFT_Enviar(ADDR_PCF575_W);
    I2C_SOFT_Enviar(Reg_P0); // Se envia direccion alta
    I2C_SOFT_Enviar(Reg_P1); // Se envia direccion baja
    Fin_Trama();
}