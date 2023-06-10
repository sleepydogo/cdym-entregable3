/*
 * main.h
 *
 * Created: 09/06/2023 16:14:22
 *  Author: sleepydogo
 */

#include "main.h"

struct uart_driver uart_driver;

char tx_buffer[100];
char rx_buffer[100];

void tx_interrupt_handler(void)
{
	uart_transmit(&uart_driver, tx_buffer, strlen(tx_buffer));
}

void rx_interrupt_handler(void)
{
	char *data = uart_receive(&uart_driver, 100);

	// Procesamiento de data
	// ...
}

void main()
{
	uint8_t baud_rate = 0x33; 
	uart_init(&uart_driver, tx_buffer, 100, rx_buffer, 100, tx_interrupt_handler, rx_interrupt_handler, baud_rate);

	uart_transmit(&uart_driver, "a", strlen("a"));

	while (1)
	{
		uart_driver.rx_interrupt_handler();

		char data = uart_driver.rx_buffer[0];

		if (data == '\n')
		{
			break;
		}
	}

	return 0;
}