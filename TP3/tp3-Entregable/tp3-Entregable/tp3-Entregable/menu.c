/*
 * menu.c
 *
 * Created: 09/06/2023 18:58:00
 *  Author: sleepydogo
 */

#include "menu.h"

uint8_t SystemState = 0, cancion_elegida = 0;

void MENU_Show_Canciones(void) {
	UART_Write_String_To_Buffer("Canciones: \r");
	UART_Write_String_To_Buffer("	1. The Simpsons \r");
	UART_Write_String_To_Buffer("	2. Mission Impossible\r");
	UART_Write_String_To_Buffer("	3. Batman\r");
	UART_Write_String_To_Buffer("	4. La pantera rosa\r");
	UART_Write_String_To_Buffer("	5. Adams Family\r");
	UART_Write_String_To_Buffer("	6. Argentina\r");
}

void MENU_Show_Menu(void)
{
	// El menú se escribe en el buffer de transmisión	 
	UART_Write_String_To_Buffer("Menu:\r");
    UART_Write_String_To_Buffer("	* PLAY: reproduce la cancion seleccionada\r");
    UART_Write_String_To_Buffer("	* STOP: detiene la reproduccion del sonido en curso\r");
    UART_Write_String_To_Buffer("	* NUM: numero de cancion a seleccionar de la lista [1 a N]\r");
    UART_Write_String_To_Buffer("	* RESET: reinicia el sistema al estado inicial\r");
}
 
void MENU_Command_Update(const char* RX_buffer)
{
	if (MENU_compareCommand(RX_buffer, "PLAY", 0))
	SystemState = ESTADO_PLAY;
	else if (MENU_compareCommand(RX_buffer, "STOP", 0)) 
	SystemState = ESTADO_STOP;
	else if (MENU_compareCommand(RX_buffer, "RESET", 0))
	SystemState = ESTADO_RESET;
	else if (MENU_compareCommand(RX_buffer, "NUM ", 1) && (1 <= cancion_elegida) && (cancion_elegida <= 6))
	SystemState = ESTADO_NUM;
	else {
		UART_Write_String_To_Buffer("Comando no valido.\r\n");
		cancion_elegida = 0;
	}
}

uint8_t MENU_compareCommand(const char* str1, const char* str2, uint8_t is_num)
{
	uint8_t i = 0, is_integer = 1;
	if (!is_num) {
		while (str1[i] != '\0' && str2[i] != '\0') {
			if (str1[i] != str2[i])
			return 0;
			i++;
		}
		return (str1[i] == '\0' && str2[i] == '\0');
	} else {
		  for (int i = 0; str1[i] != '\0'; i++) {
			  if (str1[i] < '0' && str1[i] > '9') {
				  is_integer = 0;
				  break;
			  }
		  }
		  if (is_integer) cancion_elegida = atoi(str1);
	}
}

void MENU_Perform_Task()
{
	switch (SystemState)
	{
		case ESTADO_PLAY:
		{
			char status[20];
			if ((cancion_elegida >= 1) && (cancion_elegida <= 6)) {
				sprintf(status, "	Reproduciendo	--> %d \r", cancion_elegida);
				UART_Write_String_To_Buffer(status);
				RTTTL_play_song(cancion_elegida);	
			}
			else {
				UART_Write_String_To_Buffer("\n	Error: No se ha seleccionado una cancion... \n\r");
			}
			SystemState = -1;
			break;
		}
		case ESTADO_STOP:
		{
			UART_Write_String_To_Buffer("Cancion detenida\r");
			SystemState = -1;
			break;
		}
		case ESTADO_NUM:
		{
			char status[30];
			sprintf(status, "	Cancion elegida	--> %d \r", cancion_elegida);
			UART_Write_String_To_Buffer(status);
			SystemState = -1;
			break;
		}
		case ESTADO_RESET:
		{
			UART_Write_String_To_Buffer("Reseteando ...\r");
			SystemState = -1;
			System_reset();
			break;
		}
	}
}