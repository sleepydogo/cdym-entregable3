/*
 * menu.c
 *
 * Created: 09/06/2023 18:58:00
 *  Author: sleepydogo
 */

#include "menu.h"

void MENU_Show_Menu(void)
{
    // El menú se escribe en el buffer de transmisión
	UART_Write_String_To_Buffer("                     _      \n                    (_)     \n _ __ ___  _   _ ___ _  ___ \n| '_ ` _ \\| | | / __| |/ __|\n| | | | | | |_| \\__ \\ | (__ \n|_| |_| |_|\\__,_|___/_|\\___|\n\r");
    UART_Write_String_To_Buffer("---------created by sleepydogo, merianro--------\n\r");
	UART_Write_String_To_Buffer("Menu:\n\r");
    UART_Write_String_To_Buffer("	* PLAY: reproduce la cancion seleccionada\n\r");
    UART_Write_String_To_Buffer("	* STOP: detiene la reproduccion del sonido en curso\n\r");
    UART_Write_String_To_Buffer("	* NUM: numero de cancion a seleccionar de la lista [1 a N]\n\r");
    UART_Write_String_To_Buffer("	* RESET: reinicia el sistema al estado inicial\n\r");
}
 
void MENU_Command_Update(void)
{
    char ch;
    // Check for user inputs
    if (UART_Get_Char_From_Buffer(&ch) != 0)
    {
        MENU_Perform_Task(ch);
        MENU_Show_Menu();
    }
}

/*void MENU_Perform_Task(char c)
{
    // Echo the menu option
    UART_Write_Char_To_Buffer(c);
    UART_Write_Char_To_Buffer( "\n\r");
    switch (c)
    {
    case 'a':
    case 'A':
    {
        //Function_1(); // Perform the task PORT1
        break;
    }
    case 'b':
    case 'B':
    {
        //Function_2(); // Perform the task PORT2
        break;
    }
    }
}*/