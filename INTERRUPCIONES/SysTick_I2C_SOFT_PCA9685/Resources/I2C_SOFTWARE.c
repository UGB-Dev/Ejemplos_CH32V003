/*

    I2C_SOFTWARE.c

*/

#include "I2C_SOFTWARE.h"

volatile uint8_t DAT_Copia=0, DAT_Pos=0; // variables de control para envio de datos
volatile bool RECONOCIMIENTO=0; // ACK(valor en 0) o NACK(valor en 1)
uint32_t SCL_Frec=0;
void (*Trama_Protocolo)(void);

/*********************************************************************
 * @fn      I2C_SOFT_Init
 *
 * @brief   Pin PC6 (SDA) and PC7 (SCL) for I2C,
 * @brief   Standard Frequency 100 Kb/s to 200 Kb/s
 */
void I2C_SOFT_Init(uint32_t Frecuencia){
    SCL_Frec = Frecuencia;

    /*  CONFIGURACION DEL PERIFERICO */  
    RCC -> APB2PCENR |= RCC_IOPCEN;

    /* CONFIGURACION DE NIVEL 0 LOGICO EN SDA Y SCL */
    GPIOC -> OUTDR &= ~GPIO_OUTDR_ODR6 | ~GPIO_OUTDR_ODR7;   
}

void I2C_SOFT_Enviar(uint8_t DAT){
    Trama_Protocolo = Trama_I2C_Enviar; 
    SysTick_Enable();
    DAT_Copia = DAT;
    DAT_Pos = 1;
    while (DAT_Pos);
    SysTick_Disable();
}

uint8_t I2C_SOFT_Leer(bool ACK){
    Trama_Protocolo = Trama_I2C_Leer;
    SysTick_Enable();
    RECONOCIMIENTO = ACK; // Se inicializa para determinar un ACK(1) o NACK(0)
    DAT_Copia = 0; // Se inicializa Byte_copia 
    DAT_Pos = 1; // Se inicializa el contador
    SDA_ON(); // Pin PC6(SDA) como entrada
    while(DAT_Pos); // Mantenemos hasta enviar el NACK
    SysTick_Disable(); 
    return DAT_Copia; // Retorna el valor de Byte_copia
}

///////////////////////////////////////////////////////////////////
/////////////  CONDICIONES DE INICIO Y FIN DE TRAMA  //////////////

void Inicio_Trama(void){
    SDA_ON(); // PC6(SDA) a "1" logico
    SCL_ON(); // PC7(SCL) a "1" logico
    SDA_OFF(); // PC6(SDA) a "0" logico
    SCL_OFF(); // PC7(SCL) a "0" logico
}

void Fin_Trama(void){
    SDA_OFF(); // PC6(SDA) a "0" logico
    SCL_ON(); // PC7(SCL) a "1" logico
    SDA_ON(); // PC6(SDA) a "1" logico
}

void Trama_I2C_Enviar(void){
    switch(DAT_Pos){ // Switch UART
        case 1 ... 8:
        // Se enviar el bit menos significativo primero
            if( DAT_Copia & 0x80 ){ SDA_ON(); }
            else { SDA_OFF(); } 
            SCL_ON();
            DAT_Copia <<= 1;
            ++DAT_Pos;
            break;
			
        case 9: // Bit de ACK
            SDA_ON();
            SCL_ON();
            DAT_Pos = 0;

            while (GPIOC -> INDR & GPIO_INDR_IDR6){
                if (!(GPIOC -> INDR & GPIO_INDR_IDR6)) { RECONOCIMIENTO = 0; }
                else { RECONOCIMIENTO = 1;   break; }
            }
            break;
    } // Fin de switch
    SCL_OFF();
}

void Trama_I2C_Leer(void){
    switch(DAT_Pos){
        case 1 ... 8: // Registra el valor logico para cada bit
            SCL_ON(); // PB2(SCL) a "1" logico
            if (GPIOC -> INDR & GPIO_INDR_IDR6){ // Si en PB1(SDA) es "1" logico
                DAT_Copia |= (1 << (8-DAT_Pos)); // Almacena el bit desde MSB a LSB
            }
            DAT_Pos++; // Incrementa Byte_contador
            break;
		
        case 9:
            if(RECONOCIMIENTO == 1){ SDA_OFF();}
            else{ SDA_ON(); }	 
            SCL_ON();		// PB2(SCL) a "1" logico
            DAT_Pos=0; // Incrementa Byte_contador
            break;
    }
    SCL_OFF();
}

void SysTick_Enable(void){
    SysTick -> SR &= ~(1<<0); // Se borra bandera 
    SysTick -> CMP = VALUE_CMP(SCL_Frec);
    SysTick -> CTLR = 0x0B;
    SysTick -> CNT = 0;
    NVIC_EnableIRQ(SysTicK_IRQn); // Se habilita la peticion de interrupcion externa para SysTick
}

void SysTick_Disable(void){
    SysTick -> CTLR = 0;
    NVIC_DisableIRQ(SysTicK_IRQn); // Se deshabilita la peticion de interrupcion externa para SysTick
}

void SysTick_Handler(){ // Rutina de interrupcion SysTick
    Trama_Protocolo(); // Ejecuta el Protocolo de comunicacion
    SysTick -> SR = 0; // Borra la bandera para salir
}