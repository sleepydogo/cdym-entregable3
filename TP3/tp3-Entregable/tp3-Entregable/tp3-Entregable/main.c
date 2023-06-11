/*
 * main.h
 *
 * Created: 09/06/2023 16:14:22
 *  Author: sleepydogo
 */

#include "main.h"

void main(void){
	int aux = 0;
	// Set baud rate to 9600
	UART_Init(BR9600);
	TIMER_Init();
	//UART_Write_String_To_Buffer("                     _      \n                    (_)     \n _ __ ___  _   _ ___ _  ___ \n| '_ ` _ \\| | | / __| |/ __|\n| | | | | | |_| \\__ \\ | (__ \n|_| |_| |_|\\__,_|___/_|\\___|\n\r");
	//UART_Write_String_To_Buffer("---------created by sleepydogo, merianro--------\n\r");
	
	MENU_Show_Menu();
	while (1){
		SEOS_Dispatch_Tasks();
	}
}