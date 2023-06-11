/*
 * uart.c
 *
 * Created: 09/06/2023 18:52:36
 *  Author: sleepydogo
 */

#include "uart.h"

char RX_buffer[100];
char TX_buffer[100];
uint8_t TXindice_lectura = 0, TXindice_escritura = 0;
uint8_t RXindice_lectura = 0, RXindice_escritura = 0;

void UART_Write_Char_To_Buffer (const char data, int * Error_code){
	
	if (TXindice_escritura < TX_BUFFER_LENGTH){ 
		TX_buffer [TXindice_escritura] = data;
		TXindice_escritura++;
	}else{ 
		// Write buffer is full
		*Error_code = ERROR_UART_FULL_BUFF;
	}
}

void UART_Write_String_To_Buffer(const char* STR_PTR){
	char i = 0;
	int aux = 0;
	while (STR_PTR[i] != '\0'){
		UART_Write_Char_To_Buffer (STR_PTR[i],&aux);
		i++;
	}
}

char UART_Get_Char_From_Buffer(char *ch){
    // Hay nuevo dato en el buffer?
    if (RXindice_lectura < RXindice_escritura){
        *ch = RX_buffer[RXindice_lectura];
        RXindice_lectura++;
        return 1; // Hay nuevo dato
    }
    else
    {
        RXindice_lectura = 0;
        RXindice_escritura = 0;
        return 0; // No Hay
    }
}

void UART_Update(int * Error_code){
    char dato;
	if (FLAG_datos_recibidos) {
		 
	}
    if (TXindice_lectura < TXindice_escritura) { // Hay byte en el buffer Tx para transmitir?
        UART_Send_Char(TX_buffer[TXindice_lectura]);
        TXindice_lectura++;
    }else{ // No hay datos disponibles para enviar
        TXindice_lectura = 0;
        TXindice_escritura = 0;
    }
    // se ha recibido algún byte?
    if (UART_Receive_data(&dato) != 0)
    { // Byte recibido. Escribir byte en buffer de entrada
        if (RXindice_escritura < RX_BUFFER_LENGTH){
            RX_buffer[RXindice_escritura] = dato;	// Guardar dato en buffer
            RXindice_escritura++;	// Inc sin desbordar buffer
        }
        else
            *Error_code = ERROR_UART_FULL_BUFF;
    }
}

// Estas son las funciones que encapsulan el Hardware
void UART_Init(uint8_t baud){
		// config = 0x33 ==> Configuro UART 9600bps, 8 bit data, 1 stop @ F_CPU = 8MHz.
		// config = 0x25 ==> Configuro UART 9600bps, 8 bit data, 1 stop @ F_CPU = 4Hz.
		UCSR0B = 0;
		UCSR0C = (1<<UCSZ01) | (1<<UCSZ00);
		UBRR0H = (unsigned char)(baud>>8);
		UBRR0L = (unsigned char)baud;
		
		//TX Enable
		UCSR0B |= (1<<TXEN0);
		//RX Enable
		UCSR0B |= (1<<RXEN0);
}

void UART_Send_Char (char dato)
{
	long Timeout = 0;
	while ( ( ++Timeout ) && ((UCSR0A & (1<<UDRE0))==0));
	if (Timeout != 0)
	UDR0 = dato;
	else {
		
	}
	return;
}

char UART_Receive_data(char *dato){
    if (UCSR0A & (1 << RXC0)){
        *dato = UDR0;
        return 1;
    }else
		return 0;
}


ISR(USART_TX_vect) {
	UDR0 = TX_buffer[TXindice_lectura];
	TXindice_lectura++;
	if (TXindice_lectura == TX_BUFFER_LENGTH) {
		TXindice_lectura = 0;
		UCSR0B &= ~(1<<TXCIE0); // deshabilita la interrupción de transmisión completa en el registro de control de la USART, asegurándose de que el microcontrolador no sea interrumpido cuando se completa la transmisión de datos a través de la USART.
	}
}

ISR(USART_RX_vect) {
	RX_buffer[RXindice_escritura++] = UDR0;
	if (RXindice_escritura == RX_BUFFER_LENGTH) {
		FLAG_datos_recibidos = 1;
	}
}