/*
 * menu.c
 *
 * Created: 09/06/2023 18:58:00
 *  Author: sleepydogo
 */

#include "menu.h"

uint8_t SystemState = 0;

void MENU_Show_Menu(void)
{
	// El menú se escribe en el buffer de transmisión
 	UART_Write_String_To_Buffer("Menu:\n\r");
    UART_Write_String_To_Buffer("	* PLAY: reproduce la cancion seleccionada\n\r");
    UART_Write_String_To_Buffer("	* STOP: detiene la reproduccion del sonido en curso\n\r");
    UART_Write_String_To_Buffer("	* NUM: numero de cancion a seleccionar de la lista [1 a N]\n\r");
    UART_Write_String_To_Buffer("	* RESET: reinicia el sistema al estado inicial\n\r");
}
 
void MENU_Command_Update(const char* RX_buffer)
{
	if (compareCommand(RX_buffer, "PLAY"))
	SystemState = ESTADO_PLAY;
	else if (compareCommand(RX_buffer, "STOP"))
	SystemState = ESTADO_STOP;
	else if (compareCommand(RX_buffer, "NUM "))
	SystemState = ESTADO_NUM;
	else if (compareCommand(RX_buffer, "RESET"))
	SystemState = ESTADO_RESET;
	else {
		UART_Write_String_To_Buffer("Comando no valido.\r\n");
	}
}

int compareCommand(const char* str1, const char* str2)
{
	int i = 0;
	while (str1[i] != '\0' && str2[i] != '\0') {
		if (str1[i] != str2[i])
		return 0;
		i++;
	}
	return (str1[i] == '\0' && str2[i] == '\0');
}

void MENU_Perform_Task()
{
	switch (SystemState)
	{
		case ESTADO_PLAY:
		{
			UART_Write_String_To_Buffer("Estado PLAY\r\n");
			SystemState = -1;
			break;
		}
		case ESTADO_STOP:
		{
			UART_Write_String_To_Buffer("Estado STOP\r\n");
			SystemState = -1;
			break;
		}
		case ESTADO_NUM:
		{
			UART_Write_String_To_Buffer("Estado NUM\r\n");
			SystemState = -1;
			break;
		}
		case ESTADO_RESET:
		{
			UART_Write_String_To_Buffer("Estado RESET\r\n");
			SystemState = -1;
			break;
		}
	}
}