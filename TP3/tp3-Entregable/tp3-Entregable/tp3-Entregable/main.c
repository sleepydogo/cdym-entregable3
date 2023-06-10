/*
 * main.h
 *
 * Created: 09/06/2023 16:14:22
 *  Author: sleepydogo
 */ 

#include "main.h"

#define MAX_CANCIONES 11

unsigned int duration_timer;
volatile unsigned int sound_playing = 0;
unsigned char duration, octave;
unsigned int tempo;

int main()
{

	baud_rate = 
	uart_init(uart_driver, tx_buffer, tx_size, rx_buffer, rx_size, tx_interrupt_handler, rx_interrupt_handler, baud_rate);

	UCSR0B = (1 << TXEN0) | (1 << RXEN0);

	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);

	uart_transmit(uart_driver, "Hello, world!", strlen("Hello, world!"));

	while (1)
	{
		uart_driver->rx_interrupt_handler();

		char data = uart_receive();

		if (data == '\n')
		{
			break;
		}
	}

	return 0;
}