/*

    Modulo_433MHz.h

*/

#ifndef MODULO_433MHZ_H_
#define MODULO_433MHZ_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

///////////////////////////////////////////////////////////////////
/////////////////////  FUNCIONES PROTOTIPO  ///////////////////////

void Trama_433_MHz(const char* Dato, bool int_str);
void Cod_Manchester(uint8_t Dato, uint8_t* Save_Dato);
void Contenido(uint8_t Dato);


#ifdef __cplusplus
}
#endif

#endif /* MODULO_433MHZ_H_ */