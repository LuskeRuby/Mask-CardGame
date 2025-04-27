#include "startupPhase.h"

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "model/playPhaseCommands.h"
#include "model/StartPhaseCommands.h"

void RunStartupPhase() {
    char input[100];

    printf("Enter commands (LD, SW, SD, SI, SR, QQ, P):\n");

    while (1) {
        printf("INPUT > ");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = 0; // Remove newline

        // Always make the first two characters uppercase
        for (int i = 0; i < 2 && input[i]; i++) {
            input[i] = toupper(input[i]);
        }

        if (strcmp(input, "LD") == 0 || strncmp(input, "LD ", 3) == 0) {
            char* arg = input[2] == ' ' ? input + 3 : NULL;
            char* result = LD(arg);
            if (strcmp(result, "OK") == 0) {
                printf("Deck '%s' loaded.\n", arg ? arg : "default");
                PrintDeck("LD", "OK");
            } else {
                printf("Error loading deck '%s'.\n", arg ? arg : "default");
            }

        } else if (strcmp(input, "SW") == 0) {  //OVERFLOW IF DECK NOT LOADED ALREADY
            SW();
            PrintDeck("SW", "OK");


        } else if (strcmp(input, "SD") == 0 || strncmp(input, "SD ", 3) == 0) {
            char* arg = input[2] == ' ' ? input + 3 : NULL;
            char* result = SD(arg);
            if (strcmp(result, "OK") == 0) {
                printf("Deck '%s' saved.\n", arg ? arg : "default");
                PrintDeck("SD", "OK");
            } else {
                printf("Error saving deck '%s'.\n", arg ? arg : "default");
            }

        } else if (strcmp(input, "SI") == 0 || strncmp(input, "SI ", 3) == 0) {
            int split = 0; // default random
            if (strncmp(input, "SI ", 3) == 0) {
                split = atoi(input + 3); // try to parse split if given
            }
            SI(split);
            PrintDeck("SI", "OK");

        } else if (strcmp(input, "SR") == 0) {
            SR();
            PrintDeck("SR", "OK");

        } else if (strcmp(input, "QQ") == 0) {
            printf("The program exits.\n");
            exit(0);

        } else if (strcmp(input, "P") == 0) {
            RunPlayPhase();

        } else {
            printf("Unknown command or input too many characters.\n");
        }
    }
}