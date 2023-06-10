/*
 * uart.c
 *
 * Created: 09/06/2023 18:52:36
 *  Author: sleepydogo
 */

#include "uart.h"

char RX_buffer[100];
char TX_buffer[100];
uint8_t TXindice_lectura = 0:
uint8_t RXindice_escritura = 0:


char UART_Get_Char_From_Buffer(char *ch)
{
    // Hay nuevo dato en el buffer?
    if (RXIndex_lectura < RXIndex_escritura)
    {
        *ch = RX_buffer[RXIndex_lectura];
        RXIndex_lectura++;
        return 1; // Hay nuevo dato
    }
    else
    {
        RXIndex_lectura = 0;
        RXIndex_escritura = 0;
        return 0; // No Hay
    }
}

void UART_Update(void)
{
    char dato;
    if (TXindice_lectura < TXindice_escritura) // Hay byte en el buffer Tx para transmitir?
    {
        UART_Send_Char(TX_buffer[TXindice_lectura]);
        TXindice_lectura++;
    }
    else
    { // No hay datos disponibles para enviar
        TXindice_lectura = 0;
        TXindice_escritura = 0;
    }
    // se ha recibido algún byte?
    if (UART_Receive_data(&dato) != 0)
    { // Byte recibido. Escribir byte en buffer de entrada
        if (RXIndex_escritura < RX_BUFFER_LENGTH)
        {
            RX_buffer[RXIndex_escritura] = dato; // Guardar dato en buffer
            RXIndex_escritura++;                 // Inc sin desbordar buffer
        }
        else
            Error_code = ERROR_UART_FULL_BUFF;
    }
}

void UART_Send_Char(char dato)
{
    long Timeout = 0;
    while ((++Timeout) && ((UCSR0A & (1 << UDRE0)) == 0))
        ;
    If(Timeout != 0)
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
    else
        return 0;
}