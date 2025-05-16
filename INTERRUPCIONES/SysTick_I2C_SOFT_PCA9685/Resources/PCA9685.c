/*

    PCA9685.h

*/

#include "PCA9685.h"

void PCA9685_SERVO_INIT(){
	PCA9685_SERVO_SIMPLE(MODE1,0x30);
	PCA9685_SERVO_SIMPLE(PRE_SCALE, 0x79);
	PCA9685_SERVO_SIMPLE(MODE1, 0x20);
}

void PCA9685_SERVO_SIMPLE(uint8_t REG, uint8_t Valor){
	Inicio_Trama();
	I2C_SOFT_Enviar(ADDR_PCA9685_W); // 0x80
	I2C_SOFT_Enviar(REG);
	I2C_SOFT_Enviar(Valor);
	Fin_Trama();
}

/*
void PCA9685_LED_LH(uint16_t Valor){
	I2C_SOFT_Enviar(0x00);
	I2C_SOFT_Enviar(0x00);
	I2C_SOFT_Enviar(Valor&0xFF); // parte baja en 0 log
	I2C_SOFT_Enviar(Valor>>8); // parte baja en 0 log
	
}*/

void PCA9685_SERVO_GRADOS(uint16_t Grado, bool signo, uint8_t Canal){
	
	if (signo == _POS){
		Grado = 307 + ( (Grado*220)/360 );
	}
	else{
		Grado = 307 - ( (Grado*220)/360 );
	}
	
	Inicio_Trama();
	I2C_SOFT_Enviar(ADDR_PCA9685_W);
	I2C_SOFT_Enviar(LED0_ON_L + (4* Canal)); // 0x06 + 4*0
	I2C_SOFT_Enviar(0x00); // parte baja en 1 log
	I2C_SOFT_Enviar(0x00); // parte alta en 1 log
	I2C_SOFT_Enviar((Grado & 0xFF)); // parte baja en 0 log
	I2C_SOFT_Enviar((Grado >> 8)); // parte alta en 0 log
	Fin_Trama();
	//_delay_ms();
}