#include "startupPhase.h"

#include <ctype.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "model/playPhaseCommands.h"
#include "model/StartPhaseCommands.h"
#include "view/print.h"



void RunStartupPhase() {
    char input[100];

    printf("Enter commands (type 'LD' to load, 'P' to enter StartPhase, 'QQ' to quit):\n");

    while (1) {
        printf("INPUT > ");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = 0; // Remove trailing newline

        // Make first two letters uppercase
        for (int i = 0; i < 2 && input[i]; i++) {
            input[i] = toupper(input[i]);
        }


        // All commands packed in if/else statements

        // LD default deck
        if (strcmp(input, "LD") == 0) {
            char* result = LD(NULL);
            if (strcmp(result, "OK") == 0) {
                printf("Default deck loaded.\n");
                PrintDeck("LD", "OK");
            } else {
                printf("Error loading default deck.\n");
            }

            // LD filename
        } else if (strncmp(input, "LD ", 3) == 0) {
            char filename[80];
            strcpy(filename, input + 3);  // Copy everything after "LD "
            filename[79] = '\0';  // Make sure it is null-terminated (for safety)

            char* result = LD(filename);
            if (strcmp(result, "OK") == 0) {
                printf("Deck '%s' loaded.\n", filename);
                PrintDeck("LD", "OK");
            } else {
                printf("Error loading deck '%s'.\n", filename);
            }


        } else if (strcmp(input, "SW") == 0) {
            SW();
            PrintDeck("SW", "OK");

            //SI pick a random number
        } else if (strcmp(input, "SI") == 0) { //will crash if LIST not initialized
            SI(0);
            PrintDeck("SI", "OK");

        } else if (strncmp(input, "SI ", 3) == 0) {
            int split = atoi(input + 3); // Parse after "SI "

            if (split > 1 && split < 52) {
                SI(split);
                PrintDeck("SI", "OK");
            } else {
                printf("Must provide a number between 1 and 52.\n");
            }


        } else if (strcmp(input, "SR") == 0) {   //will crash if LIST not initialized
            SR();
            PrintDeck("SR", "OK");

        } else if (strcmp(input, "SD") == 0) {
            char* result = SD(NULL);
            if (strcmp(result, "OK") == 0) {
                printf("Deck '%s' Saved.\n", "cards.txt");
                PrintDeck("SD", "OK");
            } else {
                printf("Error saving default deck.\n");
            }

        } else if (strncmp(input, "SD ", 3) == 0) {
            char filename[80];
            strcpy(filename, input + 3);  // Copy everything after "SD "
            filename[79] = '\0';

            char* result = SD(filename);
            if (strcmp(result, "OK") == 0) {
                printf("Deck '%s' Saved.\n", filename);
                PrintDeck("SD", "OK");
            } else {
                printf("Error saving deck '%s'.\n", filename);
            }

        } else if (strcmp(input, "QQ") == 0) {
            printf("The program exits.\n");
            exit(0);

        } else if (strcmp(input, "P") == 0) {
            RunPlayPhase();

        } else {
            printf("Unknown command\n");
        }
    }
}