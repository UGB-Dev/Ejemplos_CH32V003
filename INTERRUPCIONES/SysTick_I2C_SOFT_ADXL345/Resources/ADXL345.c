/*

    ADXL345.h

*/

#include "I2C_SOFTWARE.h"
#include "UART_SOFTWARE.h"
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

void Imprimir_Eje(void){ // Se envia el valor del eje
    UART_SOFT_Char((Dato_I2C/100)+48);
    Dato_I2C%=100;
    //Imprimir(".");
    UART_SOFT_Char((Dato_I2C/10)+48);
    UART_SOFT_Char((Dato_I2C%10)+48);
    //UART_SOFT_Str(",");
}

void Capturar_Eje(uint8_t REG){
    Leer_Registro(REG+1);
	
    if (Dato_I2C == 0xFF){ // Determina si es negativo
        Leer_Registro(REG); 
        Dato_I2C = Dato_I2C*(-1); 
        UART_SOFT_Str("-");
    }
    else{
        Leer_Registro(REG); 
    }
    if(RECONOCIMIENTO == 0){
        Imprimir_Eje();
    }
    else{RECONOCIMIENTO=0;}
}