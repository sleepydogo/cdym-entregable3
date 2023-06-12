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
 
void MENU_Command_Update(const char * RX_buffer)
{
	char comando[10];
	memcpy(comando, RX_buffer, 10);
	if (!strcmp(RX_buffer,"PLAY")) SystemState = ESTADO_PLAY;
	else if (!strcmp(RX_buffer,"STOP")) SystemState = ESTADO_STOP;
	else if (!strcmp(RX_buffer,"NUM")) SystemState = ESTADO_NUM;
	else if (!strcmp(RX_buffer,"RESET")) SystemState = ESTADO_RESET;
	else {
		UART_Write_String_To_Buffer("\n	Comando no valido.\n\r");
	}
	UART_Write_String_To_Buffer(comando);
}

void MENU_Perform_Task()
{
    switch (SystemState)
    {
    case ESTADO_PLAY:
    {
        UART_Write_String_To_Buffer("\n	Estado play\n\r");
		SystemState = -1;
		break;
    }
    case ESTADO_STOP:
    {
		UART_Write_String_To_Buffer("\n	estado stop\n\r");
		SystemState = -1;
        break;
    }
	case ESTADO_NUM:
	{
		UART_Write_String_To_Buffer("\n	estado num \n\r");
		SystemState = -1;
		break;
	}
	case ESTADO_RESET:
	{
		UART_Write_String_To_Buffer("\n	estado reset.\n\r");
		SystemState = -1;
		break;
	}
    }
}