/*
 * main.h
 *
 * Created: 09/06/2023 16:14:22
 *  Author: sleepydogo
 */

#include "main.h"

void main(void){
	// Set baud rate to 9600
	UART_Init(BR9600);
	SEOS_Init_Timer();
	MENU_Show_Menu();
	while (1){
		SEOS_Dispatch_Tasks();
		//SEOS_Go_To_Sleep();
	}
}