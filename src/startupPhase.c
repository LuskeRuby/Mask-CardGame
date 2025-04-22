#include "startupPhase.h"

#include <ctype.h>

#include "deck.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


void RunStartupPhase() {
    char input[100];

    printf("Enter commands (type 'LD' to load, 'P' to enter StartPhase, 'QQ' to quit):\n");

    while (1) {
        printf("INPUT > ");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = 0; // Remove trailing newline

        // Convert only the command part to uppercase
        for (int i = 0; input[i] && input[i] != '('; i++) {
            input[i] = toupper(input[i]);
        }


        if (strcmp(input, "LD") == 0) {
            // Load default deck
            char* result = LD(NULL);
            if (strcmp(result, "OK") == 0) {
                printf("Default deck loaded.\n");
                PrintDeck(1, "LD", "OK");
            } else {
                printf("Error loading default deck.\n");
            }

        } else if (strncmp(input, "LD(", 3) == 0 && input[strlen(input) - 1] == ')') {
            // Extract filename inside the parentheses
            char filename[80];
            strncpy(filename, input + 3, strlen(input) - 4);
            filename[strlen(input) - 4] = '\0';  // Null-terminate

            char* result = LD(filename);
            if (strcmp(result, "OK") == 0) {
                printf("Deck '%s' loaded.\n", filename);
                PrintDeck(1, "LD", "OK");
            } else {
                printf("Error loading deck '%s'.\n", filename);
            }

        } else if (strncmp(input, "SD(", 3) == 0 && input[strlen(input) - 1] == ')') {
            char filename[100];
            strncpy(filename, input + 3, strlen(input) - 4);
            filename[strlen(input) - 4] = '\0';

            SD(filename);
        } else if (strcmp(input, "SD") == 0) { // <-- CHANGE HERE
            SD(NULL);

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
