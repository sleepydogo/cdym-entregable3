/*
a) Modificar el ejemplo visto en clase, para poder recibir una cadena de caracteres utilizando la interrupción de
UART (RXC). El manejador de la interrupción deberá recibir un byte a la vez y almacenarlo en un buffer FIFO
(arreglo estático en RAM). Notar que los datos transmitidos desde la terminal serie finalizan con el caracter '\r'
(ENTER). Por lo tanto, cuando se detecte el caracter '\r' debe reemplazarlo por un '\0' y dar aviso a la tarea de
segundo plano de que una nueva cadena de caracteres ha sido recibida. A su vez, retransmitir la cadena recibida
mediante la función SerialPort_Send_String(char *).

b) Agregue al ejercicio anterior la posibilidad de transmitir una cadena de caracteres utilizando la interrupción
del transmisor TXC. La cadena de caracteres debe estar almacenada en un buffer FIFO y finalizada con un '\0'
antes de habilitar la interrupción. El manejador de la interrupción deberá transmitir de a 1 byte a la vez, retornando
el control al programa principal y cuando detecte que no hay más bytes para enviar debe finalizar la comunicación
enviando los caracteres '\r' '\n'. Luego del último byte transmitido la rutina debe deshabilitar la interrupción de
TXC.
*/

#include <avr/io.h>
 #include <avr/interrupt.h>
 #include "serialPort.h"

 #define BR9600 (0x67)	// 0x67=103 configura BAUDRATE=9600@16MHz

 //mensajes de bienvenida y despedida
 char msg1[] = "Uso de interrup RXC, si presiona 's' termina el programa, por favor ingrese una tecla:\n\r";
 char msg2[] = "\n\rHasta luego!";

 //comunicación con la ISR
 volatile char RX_Buffer[51];
 
 //posicion del arreglo
 uint8_t pos = 0;

 int main(void)
 {
	 
	 SerialPort_Init(BR9600); 		// Inicializo formato 8N1 y BAUDRATE = 9600bps
	 SerialPort_TX_Enable();			// Activo el Transmisor del Puerto Serie
	 SerialPort_RX_Enable();			// Activo el Receptor del Puerto Serie
	 SerialPort_Send_String(msg1);   // Envío el mensaje de Bienvenida
	 SerialPort_RX_Interrupt_Enable();	// Activo Interrupción de recepcion.
	 sei();								// Activo la mascara global de interrupciones (Bit I del SREG en 1)

	 while(1)
	 {
		 if(RX_Buffer){ // recepción NO Bloqueante	
			 // Si presionan 's' se termina el programa
			 if(RX_Buffer == 's'){
				 SerialPort_Send_String(msg2);  // Envío el string de despedida
				 while(1);
			 }
			 else{	// Eco:
				 SerialPort_Wait_For_TX_Buffer_Free(); // Espero a que el canal de transmisión este libre (bloqueante)
				 SerialPort_Send_String(RX_Buffer);			  // Envío el dato recibido
			 }
			 RX_Buffer=0;
		 }
		 //otra tarea
	 }
	 return 0;
 }

 // Rutina de Servicio de Interrupción de Byte Recibido
 ISR(USART_RX_vect){
	 RX_Buffer[pos] = UDR0; //la lectura del UDR borra flag RXC
	 if (RX_Buffer[pos] == '\r'){
		 RX_Buffer[pos] = '\0';
	 }
	 else{
		 
	 }
	 
	 pos++;
 }