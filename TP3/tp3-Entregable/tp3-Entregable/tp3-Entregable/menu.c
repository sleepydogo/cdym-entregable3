/*
 * menu.c
 *
 * Created: 09/06/2023 18:58:00
 *  Author: sleepydogo
 */

#include "menu.h"

uint8_t SystemState = 0, cancion_elegida = 0;

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
			char status[100];
			sprintf(status, "\n	Reproduciendo %d \r\n", cancion_elegida);
			UART_Write_String_To_Buffer(status);
			SystemState = -1;
			break;
		}
		case ESTADO_STOP:
		{
			//RTTTL_stop_song();
			UART_Write_String_To_Buffer("Cancion detenida\r\n");
			SystemState = -1;
			break;
		}
		case ESTADO_NUM:
		{
			UART_Write_String_To_Buffer("	1. The Simpsons \r\n");
			UART_Write_String_To_Buffer("	2. Mission Impossible\r\n");
			UART_Write_String_To_Buffer("	3. Batman\r\n");
			UART_Write_String_To_Buffer("	4. La pantera rosa\r\n");
			UART_Write_String_To_Buffer("	5. Adams Family\r\n");
			UART_Write_String_To_Buffer("	6. Argentina\r\n");
			
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