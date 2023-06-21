/*
 * seos.c
 *
 * Created: 09/06/2023 18:57:26
 *  Author: Tomas E. Schattmann, Mariano A. Rodriguez Mesa
 */
 

#include "seos.h"

// Flag interno que representa si se debe ejecutar la funcion UART_update
uint8_t UART_flag = 0;

/*
	SEOS_Init_Timer:
		Inicializa los registros internos del timer 0 y el timer 1 para generar interrupciones.
*/
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


/*
	SEOS_Schedule_Tasks:
		Esta funcion se ejecuta en cada interrupcion del timer 0, pone el flag de la UART en 1.
*/
void SEOS_Schedule_Tasks() {
	UART_flag=1;
}

/*
	SEOS_Dispatch_Tasks:
		Esta funcion se ejecuta constantemente, si el flag de la UART es uno, ejecuta la funcion UART_update, ademas se invoca a la funcion 
		MENU_Perform_task la cual ejecuta las acciones correspondientes dependiendo de la opcion de menu que se haya seleccionado.
*/
void SEOS_Dispatch_Tasks() {
	if (UART_flag) {
		UART_flag=0;
		UART_Update();
	}
	MENU_Perform_Task();
}

// Control de la duraci�n del sonido
ISR (TIMER0_COMPA_vect) // ISR para la interrupci�n de comparaci�n del Timer 0
{
	SEOS_Schedule_Tasks();
	RTTTL_interruption_handler();
}