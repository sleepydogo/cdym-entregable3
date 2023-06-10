/*
 * timer.c
 *
 * Created: 09/06/2023 18:53:13
 *  Author: Usuario
 */ 

void TIMER_Init(){
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