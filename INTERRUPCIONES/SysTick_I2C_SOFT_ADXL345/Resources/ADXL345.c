/*

    ADXL345.h

*/

#include "I2C_SOFTWARE.h"
#include "ADXL345.h"

uint8_t Dato_I2C;

void Leer_Registro(uint8_t REGISTRO){
		Inicio_Trama();
		I2C_SOFT_Enviar(ADDR_ADXL345_W);
		I2C_SOFT_Enviar(REGISTRO);
		Inicio_Trama();
		I2C_SOFT_Enviar(ADDR_ADXL345_R);
		Dato_I2C=I2C_SOFT_Leer(0);
		Fin_Trama();
}

/*
void Leer_Registros(uint8_t reg_init, uint8_t N_reg, uint8_t *dir_mem){
	inicio_Trama(); // condicion de inicio
	I2C_SOFT_Enviar(ADDR_ADXL345_W, I2C); // se envia la direccion + escritura
	I2C_SOFT_Enviar(reg_init, I2C); // envia la direccion del registro
	inicio_Trama(); // condicion de inicio
	I2C_SOFT_Enviar(ADDR_ADXL345_R, I2C); // se envia la direccion + lectura
	
	for (int a=0; a<N_reg; a++){ // se lee n registros
		if (a == (N_reg-1)){ // si el ultimo registro se lee envia un NACK
			dir_mem[a]=leer(0); // lee el registro y guarda en la ultima casilla
		}
		else{  // de lo contrario envia un ACK
			dir_mem[a]=leer(1); // lee el registro y guarda el valor en su casilla respectiva
		}
	}
	
	fin_Trama(); // condicion de fin de trama
}*/

void Escribir_Registro(uint8_t REGISTRO, uint8_t DATO){
	Inicio_Trama();
	I2C_SOFT_Enviar(ADDR_ADXL345_W);
	I2C_SOFT_Enviar(REGISTRO);
	I2C_SOFT_Enviar(DATO);
	Fin_Trama();
}