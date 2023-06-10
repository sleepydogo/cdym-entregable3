/*
 * seos.c
 *
 * Created: 09/06/2023 18:57:26
 *  Author: Usuario
 */ 

uint8_t UART_flag = 0;
uint8_t MENU_flag = 0;

void SEOS_Schedule_Tasks() {
	UART_flag=1; //actualizar perif�rico
	MENU_flag=1; //actualizar menu
}

void SEOS_Dispatch_Tasks() {
	if (UART_flag) {
		UART_flag=0;
		UART_Update();
	}
	if (MENU_flag) {
		MENU_flag=0;
		MENU_Command_Update();
	}
}

// Control de la duración del sonido
ISR (Timer_Comp_vect) { 
	SEOS_Schedule_Tasks();
}