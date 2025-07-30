/*

    DHT_11.c

*/

#include "DHT_11.h"

uint8_t DHT_11(uint8_t* Buff, uint8_t medicion){

	GPIOC -> CFGLR &= ~(GPIO_CFGLR_MODE7 | GPIO_CFGLR_CNF7); // Borra configuraciones iniciales
	GPIOC -> CFGLR |= GPIO_CFGLR_MODE7; // Pin PC7 como salida a 30 MHz en modo Push-Pull

	GPIOC -> BSHR = GPIO_BSHR_BR7; // Pin PC7 en 0 logico
	Delay_Ms(18);

	GPIOC -> CFGLR &= ~(GPIO_CFGLR_MODE7 | GPIO_CFGLR_CNF7); // Borra configuraciones anteriores
	GPIOC -> CFGLR |= GPIO_CFGLR_CNF7_0; // Pin PC7 como entrada en modo flotante

	uint8_t bits[]={0,0,0,0,0};
	uint8_t auxi=0;

	/* VERIFICA SI HAY UN ERROR EN LA LINEA DE DATOS */
	while( GPIOC -> INDR & GPIO_INDR_IDR7 ){
		Delay_Us(1);
		if (auxi > 60){ // Si supera los 60 us error en la linea
			return 0;
		}
		auxi++;
	}

	auxi=0;
	/* VERIFICA SI HAY UN ERROR EN LA LINEA DE DATOS */
	while( !(GPIOC -> INDR & GPIO_INDR_IDR7)){
		Delay_Us(1);
		if (auxi > 100){ // Si supera los 100 us error en la linea
			return 0;
		}
		auxi++;
	}
	
	auxi=0;
	/* VERIFICA SI HAY UN ERROR EN LA LINEA DE DATOS */
	while( GPIOC -> INDR & GPIO_INDR_IDR7){
		Delay_Us(1); 
		if (auxi > 100){ // Si supera los 100 us error en la linea
			return 0;
		}
		auxi++;
	}

	/* CAPTURA DE DATOS */
	for (uint8_t j=0; j<5; j++){
		uint8_t result=0;
		for (uint8_t i=0; i<8; i++){
			while( !(GPIOC -> INDR & GPIO_INDR_IDR7) ); 
			Delay_Us(35); 
			if (GPIOC -> INDR & GPIO_INDR_IDR7)result |=(1<<(7-i));
			while(GPIOC -> INDR & GPIO_INDR_IDR7);
		}
		bits[j] = result;
	}

	/* DETERMINAR SI HAY ERROR EN LA CAPTURA */
	if (bits[0] + bits[1] + bits[2] + bits[3] == bits[4] ) {
		if (medicion == Temperatura) {
			Buff[0] = bits[2]; // Parte entera
			Buff[1] = bits[3]; // Parte decimal
		}
		else if (medicion == Humedad) {
			Buff[0] = bits[0]; // Parte entera
			Buff[1] = bits[1]; // Parte decimal
		}
	}
	return 1; // Retorna a uno para saber que la recepcion de datos fue exitosa
}
