/*
 * menu.c
 *
 * Created: 09/06/2023 18:58:00
 *  Author: sleepydogo
 */

#include "menu.h"

const char welcomePicture = "
                     _      \n
                    (_)     \n
 _ __ ___  _   _ ___ _  ___ \n
| '_ ` _ \\| | | / __| |/ __|\n
| | | | | | |_| \\__ \\ | (__ \n
|_| |_| |_|\\__,_|___/_|\\___|\n";

void welcomeMenu(void) {
    
}


int main()
{
    const char *options[] = {"Play", "Stop", "Num", "Reset"};
    int num_options = sizeof(options) / sizeof(options[0]);

    welcomeMenu();
    printf("Bienvenido.. \n")
    for (int i = 0; i < num_options; i++)
    {
        printf("%d. %s\n", i + 1, options[i]);
    }

    int choice;
    scanf("%d", &choice);

    switch (choice)
    {
    case 1:
        // Play
        printf("Playing...\n");
        break;
    case 2:
        // Stop
        printf("Stopping...\n");
        break;
    case 3:
        // Num
        printf("Numbering...\n");
        break;
    case 4:
        // Reset
        printf("Resetting...\n");
        break;
    default:
        printf("Invalid choice.\n");
        break;
    }

    return 0;
}
