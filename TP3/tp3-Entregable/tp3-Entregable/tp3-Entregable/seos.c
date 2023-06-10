/*
 * seos.c
 *
 * Created: 09/06/2023 18:57:26
 *  Author: Usuario
 */ 

uint8_t UART_flag = 0;
uint8_t MENU_flag = 0;

void SEOS_Schedule_Tasks() {
	UART_flag=1; //actualizar periférico
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
ISR (TIMER0_COMPA_vect) // ISR para la interrupción de comparación del Timer 0
{
	if (duration_timer) duration_timer--; // Decremento el timer si > 0
	else                                  // si timer es = 0
	{
		TCCR1A=0;	// Desactivo el timer 1
		sound_playing = 0;                  // Borro el flag para avisar que no hay una nota sonando
	}
}