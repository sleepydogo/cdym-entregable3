/*
 * EjemploTimerRingtone.c
 *
 * Created: 23/10/2020 08:26:59 p. m.
 * Autor:								Pereira Fabio (01/09/2008) - Para un microcontrolador Freescale HCS08
 * Autor migracion / modificacion:		Perri Victor
 * Autor migracion / modificacion: 		Tomas E. Schattmann,  Mariano A. Rodriguez Mesa
 * Asignatura:							Circuitos digitales y microcontroladores
-----------------------------------------------------------*/

#include "main.h"

// Colecci?n de m?sica RTTL

const char PROGMEM song0 [] = "TheSimpsons:d=4,o=5,b=160:c.6,e6,f#6,8a6,g.6,e6,c6,8a,8f#,8f#,8f#,2g,8p,8p,8f#,8f#,8f#,8g,a#.,8c6,8c6,8c6,c6";
const char PROGMEM song1 [] = "Indiana:d=4,o=5,b=250:e,8p,8f,8g,8p,1c6,8p.,d,8p,8e,1f,p.,g,8p,8a,8b,8p,1f6,p,a,8p,8b,2c6,2d6,2e6,e,8p,8f,8g,8p,1c6,p,d6,8p,8e6,1f.6,g,8p,8g,e.6,8p,d6,8p,8g,e.6,8p,d6,8p,8g,f.6,8p,e6,8p,8d6,2c6";
const char PROGMEM song2 [] = "killing me softly:d=4,o=5,b=90:p,8e,f,g,8a,a,8g,d,g.,p,8p,8a,g,8f,8e,8e,8f,2c,p,8e,f,g,8a,a,8g,a,b,8b,8c6,8b,16a,8g,16a,2a,2a.";
const char PROGMEM song3 [] = "Xfiles:d=4,o=5,b=125:e,b,a,b,d6,2b.,1p,e,b,a,b,e6,2b.,1p,g6,f#6,e6,d6,e6,2b.,1p,g6,f#6,e6,d6,f#6,2b.,1p,e,b,a,b,d6,2b.,1p,e,b,a,b,e6,2b.,1p,e6,2b.";
const char PROGMEM song4 [] = "Zorba2:d=4,o=5,b=125:16c#6,2d6,2p,16c#6,2d6,2p,32e6,32d6,32c#6,2d6,2p,16c#6,2d6,2p,16b,2c6,2p,32d6,32c6,32b,2c6,2p,16a#,2b,p,8p,32c6,32b,32a,32g,32b,2a,2p,32a,32g,32f#,32a,1g,1p,8c#6,8d6,8d6,8d6,8d6,8d6,8d6,8d6,8c#6,8d6,8d6,8d6,8d6,8d6,16e6,16d6,16c#6,16e6,8c#6,8d6,8d6,8d6,8d6,8d6,8d6,8d6,8c#6,8d6,8d6,8d6,8d6,8d6";

const uint16_t duracion_cancion[5] = {108, 193, 126, 146, 314}; 
const char* const song_table[] PROGMEM = {song0, song1, song2, song3, song4};

// La siguiente matriz almacena las frecuencias de las notas musicales
const unsigned int note[4][12] =
{   // C    C#    D     D#    E     F     F#    G     G#    A     A#    B
	{262,  277,  294,  311,  330,  349,  370,  392,  415,  440,  466,  494}, // 4ta octava
	{523,  554,  587,  622,  659,  698,  740,  784,  830,  880,  932,  988}, // 5ta octava
	{1047, 1109, 1175, 1244, 1319, 1397, 1480, 1568, 1660, 1760, 1865, 1976}, // 6ta octava
	{2093, 2218, 2349, 2489, 2637, 2794, 2960, 3136, 3320, 3520, 3728, 3951}  // 7ma octava
};

unsigned int duration_timer;
volatile unsigned int sound_playing=0;
unsigned char duration, octave;
unsigned int tempo;

// Para que una cancion sea reproducida tiene que tener valor 0
uint8_t FLAG_stop = 0;

/*
	RTTTL_set_flag_stop:
		Recibe como parametro el valor booleano val y lo setea en la variable interna FLAG_STOP
*/
void RTTTL_set_flag_stop(uint8_t val) {
	FLAG_stop = val;
}

// Saco el sonido por el PIN5 del PORTD: freq en Hz, dur en ms
void RTTTL_sound(unsigned int freq, unsigned int dur)
{
	while (sound_playing);      // Si hay alg?n sonido presente, espero a que termine
	
	duration_timer = dur;       // Seteo el tiempo de duraci?n
	
	// Activo la salida y configuro el timer para que genere la se?al de la frecuencia apropiada
	TCCR1A|=(1<<COM1A0);
	
	// Actualizo el valor de OCR1A para que produzca la nota adecuada
	OCR1A=(8000000/(freq))-1;
	
	sound_playing = 1;          // Activo el flag para avisar que hay una nota sonando
}

/*
	RTTTL_interruption_handler:
		Se encarga de realizar el decremento del timer y de borrar el flag de una nota sonando.
		Se ejecuta cada vez que hay una interrupcion del TIMER 0
*/
void RTTTL_interruption_handler(){
	if (duration_timer) duration_timer--; // Decremento el timer si > 0
	else                                  // si timer es = 0
	{
		TCCR1A=0;	// Desactivo el timer 1
		sound_playing = 0;                  // Borro el flag para avisar que no hay una nota sonando
	}
}

/*
	RTTTL_get_song_name:
		Recibe por parametro un valor numerico cancion_elegida y lee el nombre de dicha cancion.
*/
void RTTTL_get_song_name(uint8_t cancion_elegida) {
	char buffer_song[duracion_cancion[cancion_elegida]];

	strcpy_P(buffer_song, (char *)pgm_read_word(&(song_table[cancion_elegida])));
	char * song = buffer_song;
	
	while (* song != ':') {
		UART_Write_Char_To_Buffer(song);	
		song++;
	}
}	


// Esta funcion reproduce una cancion dependiendo del numero que se pase por parametro
void RTTTL_play_song(uint8_t cancion_elegida)
{
	char buffer_song[duracion_cancion[cancion_elegida]];
	strcpy_P(buffer_song, (char *)pgm_read_word(&(song_table[cancion_elegida])));
	char * song = buffer_song;
	unsigned char temp_duration, temp_octave, current_note, dot_flag;
	unsigned int calc_duration;
	duration = 4;                 // Duraci?n est?ndar = 4/4 = 1 beat
	tempo = 63;                   // Tempo est?ndar = 63 bpm
	octave = 6;                   // Octava est?ndar = 6th
	while (*song != ':') song++;  // Busca el primer ':'
	song++;                       // Saltea el primer ':'
	while (*song!=':')            // Repite hasta encontrar ':'
	{
		if (*song == 'd')           // Entra si es el seteo de la duraci?n
		{
			duration = 0;             // Seteo la duraci?n en cero (temporalmente)
			song++;                   // Avanzo al pr?ximo caracter
			while (*song == '=') song++;  // Salteo '='
			while (*song == ' ') song++;  // Salteo los espacios
			// Si el caracter es un n?mero, seteo la duraci?n
			if (*song>='0' && *song<='9') duration = *song - '0';
			song++;                   // Avanzo al pr?ximo caracter
			// Me fijo si el caracter es un n?mero, ya que la diraci?n puede ser de dos d?gitos de largo
			if (*song>='0' && *song<='9')
			{ // Multiplico duraci?n por 10 y le agrego el valor del caracter
				duration = duration*10 + (*song - '0');
				song++;                 // Avanzo al pr?ximo caracter
			}
			while (*song == ',') song++;  // Salteo ','
		}
		
		if (*song == 'o')           // Entra si es el seteo de la octava
		{
			octave = 0;               // Seteo la octava en cero (temporalmente)
			song++;                   // Avanzo al pr?ximo caracter
			while (*song == '=') song++;  // Salteo '='
			while (*song == ' ') song++;  // Salteo los espacios
			// Si el caracter es un n?mero, seteo la octava
			if (*song>='0' && *song<='9') octave = *song - '0';
			song++;                   // Avanzo al pr?ximo caracter
			while (*song == ',') song++;  // Salteo ','
		}
		if (*song == 'b')           // Entra si es el seteo del tempo (beats por minuto)
		{
			tempo = 0;                // Seteo el tempo en cero (temporalmente)
			song++;                   // Avanzo al pr?ximo caracter
			while (*song == '=') song++;  // Salteo '='
			while (*song == ' ') song++;  // Salteo los espacios
			// Ahora leo el seteo del tempo (puede tener 3 d?gitos de largo)
			if (*song>='0' && *song<='9') tempo = *song - '0';
			song++;                   // Avanzo al pr?ximo caracter
			if (*song>='0' && *song<='9')
			{
				tempo = tempo*10 + (*song - '0'); // El tempo tiene dos d?gitos
				song++;                 // Avanzo al pr?ximo caracter
				if (*song>='0' && *song<='9')
				{
					tempo = tempo*10 + (*song - '0'); // El tempo tiene tres d?gitos
					song++;               // Avanzo al pr?ximo caracter
				}
			}
			while (*song == ',') song++;  // Salteo ','
		}
		while (*song == ',') song++;    // Salteo ','
	}
	song++;                       // Avanzo al pr?ximo caracter
	// read the musical notes
	while ((*song) && !FLAG_stop)                 // Repito hasta que el caracter sea null
	{
		current_note = 255;         // Nota por defecto = pausa
		temp_octave = octave;       // Seteo la octava a la por defecto de la canci?n
		temp_duration = duration;   // Seteo la duraci?n a la por defecto de la canci?n
		dot_flag = 0;               // Borro el flag de detecci?n de punto
		// Busco un prefijo de duraci?n
		if (*song>='0' && *song<='9')
		{
			temp_duration = *song - '0';
			song++;
			if (*song>='0' && *song<='9')
			{
				temp_duration = temp_duration*10 + (*song - '0');
				song++;
			}
		}
		// Busco una nota
		switch (*song)
		{
			case 'c': current_note = 0; break;    // C (do)
			case 'd': current_note = 2; break;    // D (re)
			case 'e': current_note = 4; break;    // E (mi)
			case 'f': current_note = 5; break;    // F (fa)
			case 'g': current_note = 7; break;    // G (sol)
			case 'a': current_note = 9; break;    // A (la)
			case 'b': current_note = 11; break;   // B (si)
			case 'p': current_note = 255; break;  // pausa
		}
		song++;                     // Avanzo al pr?ximo caracter
		// Busco un '#' siguiendo la nota
		if (*song=='#')
		{
			current_note++;   // Incremento la nota (A->A#, C->C#, D->D#, F->F#, G->G#)
			song++;                   // Avanzo al pr?ximo caracter
		}
		// Busco '.' (extiende la duraci?n de la nota un 50%)
		if (*song=='.')
		{
			dot_flag = 1;             // Si se encuentra '.', seteo el flag
			song++;                   // Avanzo al pr?ximo caracter
		}
		// Busco un sufijo de una octava
		if (*song>='0' && *song<='9')
		{
			temp_octave = *song - '0';// Seteo la octava en consecuencia
			song++;                   // Avanzo al pr?ximo caracter
		}
		if (*song=='.') // Un punto puede ser encontrado incluso despu?s de una octava
		{
			dot_flag = 1;             // Si se encuentra '.', seteo el flag
			song++;                   // Avanzo al pr?ximo caracter
		}
		while (*song == ',') song++;    // Salteo ','
		// Calculo la duraci?n de la nota
		calc_duration = (60000/tempo)/(temp_duration);
		calc_duration *= 4;         // La nota completa tiene cuatro beats
		// Chequeo si el flag de punto est? activado, de ser as?, extiendo la duraci?n en un 50%
		if (dot_flag) calc_duration = (calc_duration*3)/2;
		// Si la nota actual NO es una pausa, reproduzco la nota usando la funci?n sound
		if (current_note<255) RTTTL_sound(note[temp_octave-4][current_note],calc_duration);
		else
		{ // Si la nota actual es una pausa (255), espero dicha cantidad de tiempo
			duration_timer = calc_duration;
			sound_playing = 1;
		}
		while (sound_playing);      // Espero a la que nota/pausa en curso finalice
	}
}




