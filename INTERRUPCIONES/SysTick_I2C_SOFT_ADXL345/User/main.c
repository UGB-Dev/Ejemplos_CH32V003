/********************************** (C) COPYRIGHT *******************************
 * File Name          : main.c
 * Author             : Uriel Garnica
 * Version            : V1.0.0
 * Date               : 2023/12/25
 * Description        : ADXL345; Se genera la trama I2C por software que usa el sensor ADXL345 
                        para enviar y recibir los datos que estan o estaran alojados en sus
                        registros para el correcto funcionamiento, de igual manera se usa UART
                        por software para enviar los valores de los ejex X, Y y Z para visualizarlos
                        en un terminal serial llamado Termite
 *********************************************************************************/
 /*
    Configuracion en archivo sytem_ch32v00x.c
 
    comentar esta linea -> #define SYSCLK_FREQ_48MHz_HSE   48000000
    descomentar esta linea -> #define SYSCLK_FREQ_24MHZ_HSI   HSI_VALUE

    El MCU CH32V003 trabajara a 24 MHz y los perifericos a 24 MHz
 */

#include "debug.h"
#include "I2C_SOFTWARE.h"
#include "UART_SOFTWARE.h"
#include "ADXL345.h"


void envio_adxl(){ // Se envia el valor del eje 
	UART_SOFT_Char((Dato_I2C/100)+48);
	Dato_I2C%=100;
	//Imprimir(".");
	UART_SOFT_Char((Dato_I2C/10)+48);
	UART_SOFT_Char((Dato_I2C%10)+48);
	//UART_SOFT_Str(",");
}

void CAPTURAR_REGISTRO(uint8_t REG){
	Leer_Registro(REG+1);
	
	if (Dato_I2C == 0xFF){ // determina si es negativo
		Leer_Registro(REG); 
		Dato_I2C = Dato_I2C*(-1); 
		UART_SOFT_Str("-");
	}
	else{
		Leer_Registro(REG); 
	}
	if(RECONOCIMIENTO == 0){
		envio_adxl();
	}
	else{RECONOCIMIENTO=0;}
	
}

/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */
int main(void){
    Delay_Init();
    UART_SOFT_Init(115200); // 115200 bauds
    I2C_SOFT_Init(100000); // 100 Kb/s

    Escribir_Registro(POWER_CTL, 0x08); // activacion del ADXL345

    while(1){
        UART_SOFT_Str("########################################################\n");
        UART_SOFT_Str("Ejes Sensor ADXL345\r\n\n");
        UART_SOFT_Str("Eje X= ");
        CAPTURAR_REGISTRO(DATAX0);
		UART_SOFT_Str("  EJE Y= ");
		CAPTURAR_REGISTRO(DATAY0);
		UART_SOFT_Str("  EJE Z= ");
		CAPTURAR_REGISTRO(DATAZ0);
		UART_SOFT_Str("\n########################################################");
        UART_SOFT_Str("\r\n\n");
		Delay_Ms(350);

    }
}
