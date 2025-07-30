/*

    Modulo_433MHz.c

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
    for (uint8_t t=0; t<8; t++){ // Se envia 8 veces el preambulo
        UART_SOFT_Char(0x55); // Preambulo = 0x55
    }
	
    UART_SOFT_Char(0xA4); //  Inicio de trama 0xA4
	
    if (int_str == 0){ // Se envia un valor entero
        Contenido(*Dato);
    }
    else{
        while(*Dato != 0){ // Se envia un string
            Contenido(*Dato); // Se envia el dato en codificacion Manchester
            Dato++;
        }// Fin de while
    }
	
    UART_SOFT_Char(csum); // Se envia el checksum
    UART_SOFT_Char(0xA0); // Fin de trama 0XA0
    csum=0;
}
	
void Cod_Manchester(uint8_t Dato, uint8_t* Save_Dato){
	for (uint8_t j=0; j<4; j++){ //
		if (Dato & 0x01){ // Si el bit presente es "1"
			*Save_Dato |= (0x01 << (2*j)); // Coloca "0b01" en la posicion respectiva
		}
		else{ // de lo contrario es "0"
			*Save_Dato |= (0x02 << (2*j)); // Coloca "0b10" en la posicion respectiva
		}
		Dato>>=1; // Recorre hacia la derecha una posicion
	}
}

void Contenido(uint8_t Dato){
	Cod_Manchester(Dato >> 4, &Manch_H); // Se codifica y guarda el valor del nibble alto
	Cod_Manchester(Dato & 0x0F, &Manch_L); // Se codifica y guarda el valor del nibble bajo
	UART_SOFT_Char(Manch_H); // Se envia el nible alto codificado
	UART_SOFT_Char(Manch_L); // Se envia el nible bajo codificado
	csum += Manch_H + Manch_L; // Se realiza la suma de cada nibble
	Manch_H = Manch_L = 0;
}