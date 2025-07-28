/*
 * Modulo_433MHz.c
 *
 * Created: 16/02/2025 08:06:29 p. m.
 *  Author: UGB
 */ 


#include "Modulo_433MHz.h"
#include "UART_SOFTWARE.h"

///////////////////////////////////////////////////////////////////
/////////////////////  VARIABLES GLOBALES  ////////////////////////

uint8_t Manch_H, Manch_L, csum;

/*   
    Trama para el modulo de 433 MHz
      ----------- ----- ------- ---------- -----
     | PREAMBULO | SOF | DATOS | CHECKSUM | EOF |  
	  ----------- ----- ------- ---------- -----

    Donde:
          PREAMBULO: Se envia 0x55 para que el receptor tenga sincronia
		  SOF: Byte para inicio de trama (0xA4)
		  DATOS: Se envian los datos en codificacion manchester
		  CHECKSUM: Suma de los Ndatos para determinar errores en el receptor
		  EOF: Byte para fin de trama (0xA0) 
*/

void Trama_433_MHz(const char* Dato,  bool int_str){
	for (uint8_t t=0; t<8; t++){ // se envia 8 veces el preambulo
		UART_Char(0x55); // PREAMBULO = 0x55
	}
	
	UART_Char(0xA4); //  inicio de trama 0xA4
	
	if (int_str == 0){ // se envia un valor entero
		Contenido(*Dato);
	}
	else{
		while(*Dato != 0){ // se envia un string
			Contenido(*Dato); // se envia el dato en codificacion Manchester
			Dato++;
		}// fin de while
	}
	
	UART_Char(csum); // se envia el checksum
	UART_Char(0xA0); // fin de trama 0XA0
	csum=0;
}
	
void Cod_Manchester(uint8_t Dato, uint8_t* Save_Dato){
	for (uint8_t j=0; j<4; j++){ //
		if (Dato & 0x01){ // si el bit presente es "1"
			*Save_Dato |= (0x01 << (2*j)); // coloca "0b01" en la posicion respectiva
		}
		else{ // de lo contrario es "0"
			*Save_Dato |= (0x02 << (2*j)); // coloca "0b10" en la posicion respectiva
		}
		Dato>>=1; // recorre hacia la derecha una posicion
	}
}

void Contenido(uint8_t Dato){
	Cod_Manchester(Dato >> 4, &Manch_H); // se codifica y guarda el valor del nibble alto
	Cod_Manchester(Dato & 0x0F, &Manch_L); // se codifica y guarda el valor del nibble bajo
	UART_Char(Manch_H); // se envia el nible alto codificado
	UART_Char(Manch_L); // se envia el nible bajo codificado
	csum += Manch_H + Manch_L; // se realiza la suma de cada nibble
	Manch_H = Manch_L = 0;
}