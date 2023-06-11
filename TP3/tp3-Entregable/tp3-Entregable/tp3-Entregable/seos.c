/*
 * seos.c
 *
 * Created: 09/06/2023 18:57:26
 *  Author: Usuario
 */
 

#include "seos.h"

uint8_t UART_flag = 0;
uint8_t MENU_flag = 0;

uint8_t contador_UART = 0;
uint8_t error_uart = 0;

void SEOS_Schedule_Tasks() {
	UART_flag=1; //actualizar periferico
	MENU_flag=1; //actualizar menu
}

void SEOS_Dispatch_Tasks() {
	if (UART_flag) {
		UART_flag=0;
		UART_Update(&error_uart);
	}
// 	if (MENU_flag) {
// 		MENU_flag=0;
// 		MENU_Command_Update();
// 	}
}

// Control de la duraci�n del sonido
ISR (TIMER0_COMPA_vect) // ISR para la interrupci�n de comparaci�n del Timer 0
{
	SEOS_Schedule_Tasks();
	if (++contador_UART == 5){
		
	}

}
