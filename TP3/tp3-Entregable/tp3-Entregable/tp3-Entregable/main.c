/*
 * main.h
 *
 * Created: 09/06/2023 16:14:22
 *  Author: sleepydogo
 */

#include "main.h"

void main(void)
{
	// Set baud rate to 9600
	SerialPort_Init(103);
	SerialPort_TX_Enable();

	// Set up RTI 5ms tick
	//sEOS_Init_Timer(5);
	// Menu first time
	MENU_Show_Menu();
	while(1)
	{
		SEOS_Dispatch_Tasks();
		//SEOS_Go_To_Sleep();
	}
}