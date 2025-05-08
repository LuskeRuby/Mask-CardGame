#include "startupPhase.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "playPhase.h"
#include "model/playPhaseCommands.h"
#include "model/StartPhaseCommands.h"

Phase currentPhase = STARTUP_PHASE;  // Init

void RunStartupPhase(const char* input) {
    // Make a copy of the input to process
    char msgPrint[100] = ""; //needed msg's that require sprintf
    char command[100];
    strcpy(command, input);

    // Make first two letters uppercase
    for (int i = 0; i < 2 && command[i]; i++) {
        command[i] = toupper(command[i]);
    }

    // Check the command and handle accordingly
    if (strcmp(command, "LD") == 0) {
        char* result = LD(NULL);
        if (strcmp(result, "OK") == 0) {
            PrintStartupPhase("LD", "OK\n");
        } else {
            PrintStartupPhase("LD", "Error loading default deck.\n");
        }

    } else if (strncmp(command, "LD ", 3) == 0) {
        char filename[80];
        strcpy(filename, command + 3);  // Copy everything after "LD "
        filename[79] = '\0';  // Null terminate


        char* result = LD(filename);
        printf(result);
        if (strcmp(result, "OK") == 0) {
            PrintStartupPhase("LD", "OK");
        } else {
            sprintf(msgPrint, "Error loading deck '%s'.\n", filename);
            PrintStartupPhase("LD", msgPrint);
        }



    } else if (strcmp(command, "SW") == 0) {
        SW();
        PrintStartupPhase("SW", "OK.\n");

    } else if (strcmp(command, "SI") == 0) {
        SI(0);
        PrintStartupPhase("SI", "Deck shuffled.\n");

    } else if (strncmp(command, "SI ", 3) == 0) {
        int split = atoi(command + 3); // Convert ASCII to int (Parse after "SI ")
        if (split > 1 && split < 52) {
            SI(split);
            sprintf(msgPrint, "Shuffled using pile with '%d' cards.\n", split);
            PrintStartupPhase("SI", msgPrint);
        } else {
            PrintStartupPhase("SI", "Must provide a number between 1 and 52.\n");
        }

    } else if (strcmp(command, "SR") == 0) {
        SR();
        PrintStartupPhase("SR", "Deck reshuffled.\n");

    } else if (strcmp(command, "SD") == 0) {
        char* result = SD(NULL);
        if (strcmp(result, "OK") == 0) {
            PrintStartupPhase("SD", "Deck 'cards.txt' saved.\n");
        } else {
            PrintStartupPhase("SD", "Error saving deck.\n");
        }

    } else if (strncmp(command, "SD ", 3) == 0) {
        char filename[80];
        strcpy(filename, command + 3);  // Copy everything after "SD "
        filename[79] = '\0';  // Null terminate

        char* result = SD(filename);
        if (strcmp(result, "OK") == 0) {
            sprintf(msgPrint, "Deck '%s' saved.\n", filename);
            PrintStartupPhase("SD", msgPrint);
        } else {
            sprintf(msgPrint, "Error saving deck '%s'.\n", filename);
            PrintStartupPhase("SD", msgPrint);
        }

    } else if (strcmp(command, "QQ") == 0) {
        PrintStartupPhase("QQ", "The program exits.\n");
        exit(0);  // Exit the program

    } else if (strcmp(command, "P") == 0) {
        // Transition to PlayPhase
        currentPhase = PLAY_PHASE;
        InitArray();
        PrintPlayPhase("P", "ok");

    } else {

        PrintStartupPhase(command, "Unknown command.\n");
    }
}
