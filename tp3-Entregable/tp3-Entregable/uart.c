/*
 * uart.c
 *
 * Created: 09/06/2023 18:52:36
 *  Author: Tomas E. Schattmann, Mariano A. Rodriguez Mesa
 */

#include "uart.h"

uint8_t FLAG_datos_recibidos = 0;

char RX_buffer[RX_BUFFER_LENGTH];
char TX_buffer[TX_BUFFER_LENGTH];

uint8_t TXindice_lectura = 0, TXindice_escritura = 0;
uint8_t RXindice_lectura = 0, RXindice_escritura = 0;

/*
	UART_Init: 
		Recibe como parametro un entero que determina los baudios a los que va a transmitir el dispositivo.
		Configura lso registros necesarios para inicializar la UART con 9600bps, 8 bit de datos, 1 stop.
*/
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

/*
	UART_Write_Char_To_Buffer:
		Si el indice de lectura del buffer TX es menor a la longitud total del buffer, se escribe en este el char
		pasado por parametro. Luego deshabilita las interrupciones por transmision.
*/
void UART_Write_Char_To_Buffer(const char data){
	if (TXindice_escritura < TX_BUFFER_LENGTH){ 
		TX_buffer [TXindice_escritura] = data;
		TXindice_escritura++;
		UART_TX_Interrupt_Enable();
	}
}

/*
	UART_Write_String_To_Buffer:
		Recibe un puntero a un string como parametro y transmite cada uno de los caracteres del mismo al buffer TX hasta 
		toparse con el fin de linea ('\0')
*/
void UART_Write_String_To_Buffer(const char* STR_PTR){
	char i = 0;
	while (STR_PTR[i] != '\0'){
		UART_Write_Char_To_Buffer (STR_PTR[i]);
		i++;
	}
}

/*
	UART_TX_Interrupt_Enable: 
		Activa las interrupciones de transmision.
*/
void UART_TX_Interrupt_Enable(void){
	UCSR0B |= (1<<UDRIE0);
}

/*
	UART_TX_Interrupt_Enable:
		Desactiva las interrupciones de transmision.
*/
void UART_TX_Interrupt_Disable(void)
{
	UCSR0B &=~(1<<UDRIE0);
}

/*
	UART_TX_Interrupt_Enable:
		Activa las interrupciones de recepcion.
*/
void UART_RX_Interrupt_Enable(void){
	UCSR0B |= (1<<RXCIE0);
}

/*
	UART_TX_Interrupt_Enable:
		Desactiva las interrupciones de recepcion.
*/
void UART_RX_Interrupt_Disable(void){
	UCSR0B &= ~(1<<RXCIE0);
}



/*
	UART_Update:
		Activa las interrupciones por recepcion y luego si esta activo el flag de recepcion se ejectuta el command update con el comando
		recibido, se reinicia el indice de escritura del buffer RX y se desactiva el flag de datos.
*/
void UART_Update(){
    UART_RX_Interrupt_Enable();
	if (FLAG_datos_recibidos) {
		MENU_Command_Update(RX_buffer);
		RXindice_escritura = 0;
		FLAG_datos_recibidos = 0;
	}
}

// Foreground - Consumidor, esperamos a que la tarea de background genere los datos y los transmisitmos
/*
	Rutina de interrupcion de transmision:
		Transmite los datos almacenados en el buffer TX al registro UDR0 uno por uno, hasta toparse con el fin de linea.
*/
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
			if (MENU_Compare_Command(RX_buffer, "STOP", 0)) { 
				RTTTL_set_flag_stop(1);
				UART_RX_Interrupt_Disable();
			}
		}
	}
	UART_Write_Char_To_Buffer(aux);
}	