/*

    WS2812.c

*/

#include "WS2812.h"

/*
    eje y=posicion de array ; eje x=posicion del bit
        
             Posicion de matriz 8x8       01234567                              
   EJEMPLO: POSICION (7,7) y (4,3)  /   0b00000000  // Y=0 <==> Buffer[0]
                                   /    0b00000000  // Y=1 <==> Buffer[1]
    Buffer[7]= 0b00000001 = 0x01  /     0b00000000  // Y=2 <==> Buffer[2]
                                 {      0b00000000  // Y=3 <==> Buffer[3]
    Buffer[3]= 0b00001000 = 0x08  \     0b00001000  // Y=4 <==> Buffer[4]
                                   \    0b00000000  // Y=5 <==> Buffer[5]
                                    \   0b00000000  // Y=6 <==> Buffer[6]
                                     \  0b00000001  // Y=7 <==> Buffer[7]
*/
uint8_t Buffer[8] = {0}; 
uint16_t Buffer_Color[8] = {0};

void ENVIAR(uint32_t DAT){
    for (uint8_t i=0; i<24; i++) {  
        if (DAT & 0x800000) {
            // SE ENVIA EL TIEMPO ALTO DEL BIT PARA UN 1 LOGICO (800 ns)
            GPIOC -> OUTDR |= GPIO_OUTDR_ODR1 ; // 208.5 ns
            GPIOC -> OUTDR |= GPIO_OUTDR_ODR1 ; // 208.5 ns
            GPIOC -> OUTDR |= GPIO_OUTDR_ODR1 ; // 208.5 ns
            GPIOC -> OUTDR |= GPIO_OUTDR_ODR1 ; // 208.5 ns

            // SE ENVIA EL TIEMPO BAJO DEL BIT PARA UN 1 LOGICO (450 ns)
            GPIOC -> OUTDR &= ~GPIO_OUTDR_ODR1 ; // 208.5 ns
            GPIOC -> OUTDR &= ~GPIO_OUTDR_ODR1 ; // 208.5 ns
        }
        else {
            // SE ENVIA EL TIEMPO ALTO DEL BIT PARA UN 0 LOGICO (400 ns)
            GPIOC -> OUTDR |= GPIO_OUTDR_ODR1 ; // 208.5 ns
            GPIOC -> OUTDR |= GPIO_OUTDR_ODR1 ; // 208.5 ns

            // SE ENVIA EL TIEMPO BAJO DEL BIT PARA UN 1 LOGICO (850 ns)
            GPIOC -> OUTDR &= ~GPIO_OUTDR_ODR1 ; // 208.5 ns
            GPIOC -> OUTDR &= ~GPIO_OUTDR_ODR1 ; // 208.5 ns
            GPIOC -> OUTDR &= ~GPIO_OUTDR_ODR1 ; // 208.5 ns
            GPIOC -> OUTDR &= ~GPIO_OUTDR_ODR1 ; // 208.5 ns
        }
        DAT<<=1;
    }
}

void Imprimir(uint8_t* DAT, uint16_t TIEMPO, uint32_t Color){
    uint8_t aux;
    for (uint8_t i=0; i<8; i++){
        aux = DAT[i]; // Almacena el byte presente del buffer
        /*  SE ENVIA UN SOLO COLOR PARA TODA LA MATRIZ 8*8 */
        for(uint8_t j=0; j<8; j++){
            (aux & 0x1)? ENVIAR(Color) : ENVIAR(0);
            aux>>=1;
        }
    }
    Delay_Ms(1+TIEMPO);
}

void Posicion(uint8_t Pos_x, uint8_t Pos_y, uint32_t Color){
    Buffer[Pos_y-1] |= (1<<(Pos_x-1)); // Coloca 1 en la posicion (X,Y) para encender el led 
    Imprimir(Buffer, 250, Color);
    /*                ^     
                 Tiempo en ms       */
}

void LineaV(uint8_t Pos_x0, uint8_t Pos_y0, uint8_t Pos_x1, uint32_t Color){
    /* INTERCAMBIA "Pos_x1" A "Pos_x0" SI "Pos_x1" ES INFERIOR A "Pos_x0" */
    if (Pos_x1 < Pos_x0 ) {
        uint8_t aux;
        aux = Pos_x0;
        Pos_x0 = Pos_x1;
        Pos_x1 = aux; 
    }

    /*  GUARDA E IMPRIME LA LINEA VERTICAL  */
    for (uint8_t x=0; x<(Pos_x1-Pos_x0+1); x++) {
        Buffer[Pos_y0-1] |= (1<<(Pos_x0+x-1)); // Coloca 1 en la posicion (X,Y) para encender el led 
        Imprimir(Buffer, 1, Color);
    }
}

void LineaH(uint8_t Pos_x0, uint8_t Pos_y0, uint8_t Pos_y1, uint32_t Color){
    /* INTERCAMBIA "Pos_y1" A "Pos_y0" SI "Pos_y1" ES INFERIOR A "Pos_y0" */
    if (Pos_y1 < Pos_y0 ) {
        uint8_t aux;
        aux = Pos_y0;
        Pos_y0 = Pos_y1;
        Pos_y1 = aux; 
    }

    /*  GUARDA E IMPRIME LA LINEA HORIZONTAL  */
    for (uint8_t y=0; y<(Pos_y1-Pos_y0+1); y++) {
        Buffer[Pos_y0+y-1] |= (1<<(Pos_x0-1)); // Coloca 1 en la posicion (X,Y) para encender el led 
        Imprimir(Buffer, 1, Color);
    }
}

void Borrar_Buffer(void){
    /*  ASIGNA 0 A CADA POSICION DEL BUFFER  */
    for (uint8_t y=0; y<8; y++) {
        Buffer[y] = 0;
    }
}

void Imprimir_Color(uint8_t* DAT, uint16_t* Color){
    uint8_t aux;
    uint16_t aux_Color;
    
    for (uint8_t i=0; i<8; i++){
        aux = DAT[i]; // Almacena el byte presente del buffer
        aux_Color = Color[i]; // Almacena el byte presente del buffer_color

        /*  TRANSMISION DEL COLOR DE CADA LED WS2812  */
        for(uint8_t j=0; j<8; j++){
            /* SE DETERMINA SI EL LED DEBE ESTAR ENCENDIDO  */
            if(aux & 0x1){ 
                /*  SE ENVIA EL COLOR DEL LED (X,Y) */
                switch (aux_Color & 0x3) {
                    case amarillo: // 0b00
                        ENVIAR(AMARILLO);
                        break;
                    case verde: // 0b01
                        ENVIAR(VERDE); 
                        break;
                    case azul: // 0b10
                        ENVIAR(AZUL); 
                        break;
                    case blanco: // 0b11
                        ENVIAR(BLANCO); 
                        break;
                } // Fin del swich
            }// Fin del if(aux & 0x1)
            else{
                ENVIAR(0);
            }
            aux >>= 1;
            aux_Color >>= 2;
        } // Fin del for
    }
    Delay_Ms(1); // Retardo para finalizar trama (>50 us)
}

void Posicion_Color(uint8_t Pos_x, uint8_t Pos_y, uint8_t Color){
    Buffer[Pos_y-1] |= (1<<(Pos_x-1)); // Coloca 1 en la posicion (X,Y) para encender el led 
    Buffer_Color[Pos_y-1] &= ~(3<<(((Pos_x-1)<<1))); // Borra el color anterior 
    Buffer_Color[Pos_y-1] |= (Color<<(((Pos_x-1)<<1))); // Se ajusta el color en la posicion (X,Y) del buffer_color
    Imprimir_Color(Buffer, Buffer_Color); 
}

void Activar_LED(uint8_t Pos_x, uint8_t Pos_y){
    Buffer[Pos_y-1] |= (1<<(Pos_x-1)); // Coloca 1 en la posicion (X,Y) para encender el led 
    Imprimir_Color(Buffer, Buffer_Color); 
}

void Borrar_LED(uint8_t Pos_x, uint8_t Pos_y){
    Buffer[Pos_y-1] &= ~(1<<(Pos_x-1)); // Coloca 0 en la posicion (X,Y) para apagar el led
}
