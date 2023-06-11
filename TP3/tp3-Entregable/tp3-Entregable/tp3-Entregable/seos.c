/*
 * seos.c
 *
 * Created: 09/06/2023 18:57:26
 *  Author: Usuario
 */
 

#include "seos.h"

uint8_t UART_flag = 0;
uint8_t MENU_flag = 0;

void SEOS_Go_To_Sleep() {
	set_sleep_mode(SLEEP_MODE_PWR_DOWN);  // Configura el modo de bajo consumo
	sleep_enable();  // Habilita el sleep mode
	sleep_cpu();  // Pone el microcontrolador en sleep mode
	sleep_disable();  // Deshabilita el sleep mode cuando se despierte
}

void SEOS_Init_Timer() {
	// ------------------------ Timer 0 ------------------------
	
	// Configuro una interrupción cada 1 mseg
	OCR0A = 248;			//124 para 8MHz y 248 para 16MHz
	TCCR0A = (1<<WGM01);   // Modo CTC, clock interno, prescalador 64
	TCCR0B = (1<<CS01)|(1<<CS00);   // Modo CTC, clock interno, prescalador 64
	TIMSK0 = (1<<OCIE0A);   // Habilito Timer 0 en modo de interrupción de comparación
	
	
	// ------------------------ Timer 1 ------------------------
	
	TCCR1A|=(1<<COM1A0);// Configuro Timer1 para clk con prescaler P=1, modo CTC y salida por pin
	TCCR1B|=(1<<WGM12)|(1<<CS10);
	DDRB|=(1<<PINB1); // El PIN1 del PORTB será el pin de salida


	//Habilito la máscara de interrupciones
	
	sei();
}

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
*/ 