/*
 * uart.c
 *
 * Created: 09/06/2023 18:52:36
 *  Author: sleepydogo
 */

#include "uart.h"

uint8_t FLAG_datos_recibidos = 0;
uint8_t FLAG_errorBuffer = 0;
uint8_t auxe = 0;
char RX_buffer[RX_BUFFER_LENGTH];
char TX_buffer[TX_BUFFER_LENGTH];
uint8_t TXindice_lectura = 0, TXindice_escritura = 0;
uint8_t RXindice_lectura = 0, RXindice_escritura = 0;

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

void UART_Write_Char_To_Buffer (const char data, int * Error_code){
	
	if (TXindice_escritura < TX_BUFFER_LENGTH){ 
		TX_buffer [TXindice_escritura] = data;
		TXindice_escritura++;
		UART_TX_Interrupt_Enable();
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

void UART_Send_Char (char dato)
{
	long Timeout = 0;
	while ( ( ++Timeout ) && ((UCSR0A & (1<<UDRE0))==0));
	if (Timeout != 0)
	UDR0 = dato;
	else {
		
	}
}

char UART_Receive_data(char *dato){
	if (UCSR0A & (1 << RXC0)){
		*dato = UDR0;
		return 1;
	} else
	return 0;
}

void UART_TX_Interrupt_Enable(void){
	UCSR0B |= (1<<UDRIE0);
}

void UART_TX_Interrupt_Disable(void)
{
	UCSR0B &=~(1<<UDRIE0);
}

void UART_RX_Interrupt_Enable(void){
	UCSR0B |= (1<<RXCIE0);
}

void UART_RX_Interrupt_Disable(void){
	UCSR0B &= ~(1<<RXCIE0);
}



void UART_Update(int * Error_code){
    UART_RX_Interrupt_Enable();
	if (FLAG_datos_recibidos) {
		MENU_Command_Update(RX_buffer);
		RXindice_escritura = 0;
		FLAG_datos_recibidos = 0;
	}
}

// Foreground - Consumidor, esperamos a que la tarea de background genere los datos y los transmisitmos
ISR(USART_UDRE_vect) {
	if (TXindice_lectura == TXindice_escritura) {
		// Se han transmitido todos los datos en el buffer
		UCSR0B &= ~(1 << UDRIE0);  // Deshabilitar la interrupción de registro de datos vacío
		} else {
		if (TX_buffer[TXindice_lectura] != '\0') {
			UDR0 = TX_buffer[TXindice_lectura++];  // Enviar el siguiente carácter del buffer
			} else {
			// Fin de mensaje (carácter nulo '\0')
			UCSR0B &= ~(1 << UDRIE0);  // Deshabilitar la interrupción de registro de datos vacío
			UART_TX_Interrupt_Disable();  // Habilitar interrupción de recepción USART
		}
	}
}

// Foreground - Productor
ISR(USART_RX_vect) {
	char aux = UDR0;
	if ((aux != '\r') && (aux != '\n')){
		RX_buffer[RXindice_escritura] = aux;
		RXindice_escritura++;
	}else{
		if (aux == '\r'){
			RX_buffer[RXindice_escritura] = '\0';
			RXindice_escritura++;
		}
		if (aux == '\n'){
			FLAG_datos_recibidos = 1;
			UART_RX_Interrupt_Disable();	
		}
	}
	UART_Write_Char_To_Buffer(aux,FLAG_errorBuffer);
}	