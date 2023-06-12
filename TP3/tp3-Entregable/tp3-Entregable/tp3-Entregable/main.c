/*
 * main.h
 *
 * Created: 09/06/2023 16:14:22
 *  Author: sleepydogo
 */

#include "main.h"
uint8_t aux = 0;
void main(void){
	// Set baud rate to 9600
	UART_Init(BR9600);
	
	//UART_TX_Interrupt_Enable();
	
	//UART_RX_Interrupt_Enable();
	
	MENU_Show_Menu();
	
	SEOS_Init_Timer();	
	
	while (1){
		SEOS_Dispatch_Tasks();
	}
}