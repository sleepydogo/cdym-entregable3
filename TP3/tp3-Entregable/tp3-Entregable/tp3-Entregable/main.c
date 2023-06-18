/*
 * main.h
 *
 * Created: 09/06/2023 16:14:22
 *  Author: Tomas E. Schattmann y Mariano A. Rodriguez Mesa
 */

#include "main.h"
uint8_t aux = 0;

// Esta funcion realiza un reset del sistema despues de 15 milisegundos.
void System_reset() {
	wdt_enable(WDTO_15MS); 
}


void main(void){
	UART_Init(BR9600); // Inicia la UART con una baud rate de 9600
	MENU_Show_Menu(); // Muestra las opciones de menu
	SEOS_Init_Timer(); // Inicia las interrupciones de timer
	_delay_ms(100); // Delay de 100 ms para esperar a que se termine de transmitir las canciones
	//MENU_Show_Canciones(); // Muestra las canciones
	
	while (1){
		SEOS_Dispatch_Tasks(); // Realiza las tareas programadas en una interrupcion
	}
}