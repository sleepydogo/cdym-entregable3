/*
 * main.h
 *
 * Created: 09/06/2023 16:14:22
 *  Author: sleepydogo
 */

#include "main.h"
uint8_t aux = 0;

void System_reset() {
	// Enable the watchdog timer
	wdt_enable(WDTO_15MS); // Reset the microcontroller after 15 milliseconds
}


void main(void){
	// Set baud rate to 9600
	UART_Init(BR9600);
	MENU_Show_Canciones();
	SEOS_Init_Timer();
	_delay_ms(100);
	MENU_Show_Menu();
	while (1){
		SEOS_Dispatch_Tasks();
	}
}