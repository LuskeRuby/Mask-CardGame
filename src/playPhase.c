//
// Created by rubym on 14/04/2025.
//

#include "playPhase.h"
#include <stdio.h>
#include <string.h>

void RunPlayPhase() {
    char input[100];

    printf(">>play phase<<. Enter Commands 'Q' to return to the startup Phase):\n");

    while (1) {
        printf("INPUT > ");
        fgets(input, sizeof(input),stdin);
        input[strcspn(input, "\n")] = 0; // Remove trailing newline

        //Check if command is exacly "Q"
        if (strcmp(input, "Q") == 0) {
            printf("Returned to startupPhase.\n");
            RunStartupPhase();
        }

        //Check if command is a startupPhase Command
        else if(strncmp(input, "LD", 2) == 0 ||
                strncmp(input, "SD", 2) == 0 ||
                strcmp(input, "SW") == 0 ||
                strcmp(input, "SI") == 0 ||
                strcmp(input, "SR") == 0||
                strcmp(input, "QQ") == 0 ) {
            printf("Command not available in the PLAY phase \n");
        }
        else {
            printf("Unknown command \n");
        }
    }
}
