/*
 * menu.c
 *
 * Created: 09/06/2023 18:58:00
 *  Author: Tomas E. Schattmann, Mariano A. Rodriguez Mesa
 */

#include "menu.h"

/* 
	SystemState representa la opcion elegida del menu, se podria interpretar como una MEF.
	cancion_elegida representa numericamente la cancion seleccionada
*/
uint8_t SystemState = 0, cancion_elegida = 0;

// Se almacenan las opciones del menu en la memoria ROM
const char PROGMEM option0 []  = "Menu:\r";
const char PROGMEM option1 []  = "* PLAY: reproduce la cancion seleccionada\r";
const char PROGMEM option2 []  = "* STOP: detiene la reproduccion del sonido en curso\r";
const char PROGMEM option3 []  = "* NUM: numero de cancion a seleccionar de la lista [1 a N]\r";
const char PROGMEM option4 []  = "* RESET: reinicia el sistema al estado inicial\r";
const char PROGMEM songMenu0 [] = "Canciones: \r";

const uint8_t option_length [5] = {8,44,54,61,49,13};

const char * const option_table[] PROGMEM = {option0, option1,option2,option3,option4, songMenu0};

void MENU_Show_Canciones(void) {
	
	for (uint8_t i = 0; i <= 5; i ++) {
		char tempBuffer[30];
		
		// Copiar el string hasta el primer ':'
		strncpy_P(tempBuffer, option_table[i], strchr_P(option_table[i], ':') - option_table[i]);
 
		// Añadir el carácter nulo al final del string copiado
		tempBuffer[strchr_P(option_table[i], ':') - option_table[i]] = '\0';
		UART_Write_String_To_Buffer(tempBuffer);
  
	}


// 	UART_Write_String_To_Buffer("	1. The Simpsons \r");
// 	UART_Write_String_To_Buffer("	2. Mission Impossible\r");
// 	UART_Write_String_To_Buffer("	3. Batman\r");
// 	UART_Write_String_To_Buffer("	4. La pantera rosa\r");
// 	UART_Write_String_To_Buffer("	5. Adams Family\r");
// 	UART_Write_String_To_Buffer("	6. Argentina\r");
}


/*
	MENU_Show_Menu:
		Esta funcion muestra cada una de las opciones del menu leyendolas desde la memoria ROM.
*/
void MENU_Show_Menu(void)
{
	for (uint8_t i = 0; i <= 5; i ++) {
		char tempBuffer[option_length[i]];
		strcpy_P(tempBuffer, (char *)pgm_read_word(&(option_table[i])));
		UART_Write_String_To_Buffer(tempBuffer);
	}
}
 
 /*
	MENU_Command_Update:
		Recibe por parametro el buffer RX y compara la informacion recibida con las opciones correspondientes del menu.
 */
void MENU_Command_Update(const char* RX_buffer)
{
	if (MENU_Compare_Command(RX_buffer, "PLAY", 0))
	SystemState = ESTADO_PLAY;
	else if (MENU_Compare_Command(RX_buffer, "STOP", 0)) 
	SystemState = ESTADO_STOP;
	else if (MENU_Compare_Command(RX_buffer, "RESET", 0))
	SystemState = ESTADO_RESET;
	else if (MENU_Compare_Command(RX_buffer, "NUM ", 1) && (1 <= cancion_elegida) && (cancion_elegida <= 6)) // Aqui para entrar al ESTADO_PLAY se necesita que la cancion_elegida este comprendida entre [1,6]
	SystemState = ESTADO_NUM;																											
	else {
		UART_Write_String_To_Buffer("Comando no valido.\r\n");
		cancion_elegida = 0;
	}
}

/*
	MENU_Compare_Command:
		Recibe dos strings por parametro y realiza una comparacion entre ellas, retorna 1 si son iguales.
		Adicionalmente recibe un parametro booleano is_num que indica si las strings a comparar corresponden a la opcion NUM de las opciones de menu,
		en cuyo caso se realiza una comparacion del str1 con valores numericos, retorna 1 si son todos numeros y asigna el valor de RX_Buffer a cancion_elegida.
*/
uint8_t MENU_Compare_Command(const char* str1, const char* str2, uint8_t is_num)
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

/*
	MENU_Perfom_Task:
		Esta funcion trabaja como una maquina de estados, dependiendo de la variable SystemState ejecuta las acciones correspondientes a cada caso.
*/
void MENU_Perform_Task()
{
	switch (SystemState)
	{
		/*
			ESTADO_PLAY: Comprueba si se ha seleccionado una cancion, si es asi la reproduce.
		*/
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
		/*
			ESTADO_STOP: La cancion originalmente se detiene desde la interrupcion RX, debido a lo explicado en la seccion COMPLETAR del informe. Este estado
			envia un string de cancion detenida y setea el valor de flag_stop del modulo RTTTL en cero. 
		*/
		case ESTADO_STOP:
		{
			UART_Write_String_To_Buffer("Cancion detenida\r");
			RTTTL_set_flag_stop(0);
			SystemState = -1;
			break;
		}
		/*
			ESTADO_PLAY: Transmite un string de estatus donde se muestra la cancion elegida.
		*/
		case ESTADO_NUM:
		{
			char status[30];
			sprintf(status, "	Cancion elegida	--> %d \r", cancion_elegida);
			UART_Write_String_To_Buffer(status);
			SystemState = -1;
			break;
		}
		/*
			ESTADO_RESET: Transmite un string de estatus y luego llama a la funcion System_reset para reiniciar el sistema.
		*/
		case ESTADO_RESET:
		{
			UART_Write_String_To_Buffer("Reseteando ...\r");
			SystemState = -1;
			System_reset();
			break;
		}
	}
}