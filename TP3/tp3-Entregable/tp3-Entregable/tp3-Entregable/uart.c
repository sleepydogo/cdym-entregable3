/*
 * uart.c
 *
 * Created: 09/06/2023 18:52:36
 *  Author: sleepydogo
 */

#include "uart.h"

char RX_buffer[100];
char TX_buffer[100];
uint8_t RXindice_lectura;
uint8_t RXindice_escritura;
uint8_t TXindice_lectura;
uint8_t TXindice_escritura;
char Error_code[25];

void UART_Write_Char_To_Buffer(const char data) {
    if (TXindice_escritura < TX_BUFFER_LENGTH)
    {
        TX_buffer[TXindice_escritura] = data;
        TXindice_escritura++;
    }
    else
    { // Write buffer is full
        *Error_code = "ERROR_UART_FULL_BUFF";
    }
}

void UART_Write_String_To_Buffer(const char *STR_PTR)
{
    char i = 0;
    while (STR_PTR[i] != "\0")
    {
        UART_Write_Char_To_Buffer(STR_PTR[i]);
        i++;
    }
}

char UART_Get_Char_From_Buffer(char *ch)
{
    // Hay nuevo dato en el buffer?
    if (RXindice_lectura < RXindice_lectura)
    {
        *ch = RX_buffer[RXindice_lectura];
        RXindice_lectura++;
        return 1; // Hay nuevo dato
    }
    else
    {
        RXindice_lectura = 0;
        RXindice_lectura = 0;
        return 0; // No Hay
    }
}

void UART_Send_Char(char dato)
{
	long Timeout = 0;
	while ((++Timeout) && ((UCSR0A & (1 << UDRE0)) == 0))
	;
	if (Timeout != 0)
	UDR0 = dato;
	else
	{
		// TX_UART did not respond – error
	}
}

char UART_Receive_data(char *dato)
{
	if (UCSR0A & (1 << RXC0))
	{
		*dato = UDR0;
		return 1;
	}
	else {
		return 0;
	}
}

void UART_Update(void)
{
    char dato;
    if (TXindice_lectura < TXindice_escritura) // Hay byte en el buffer Tx para transmitir?
    {
        UART_Send_Char(TX_buffer[TXindice_lectura]);
        TXindice_escritura++;
    }
    else
    { // No hay datos disponibles para enviar
        TXindice_lectura = 0;
        TXindice_escritura = 0;
    }
    // se ha recibido algún byte?
    if (UART_Receive_data(&dato) != 0)
    { // Byte recibido. Escribir byte en buffer de entrada
        if (RXindice_lectura < RX_BUFFER_LENGTH)
        {
            RX_buffer[RXindice_lectura] = dato; // Guardar dato en buffer
            RXindice_lectura++;                 // Inc sin desbordar buffer
        }
        else {
			*Error_code = "ERROR_UART_FULL_BUFF";
		}    
    }
}