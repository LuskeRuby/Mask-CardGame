#include "playPhase.h"

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <model/deck.h>
#include <view/print.h>

#include "startupPhase.h"
#include "model/playPhaseCommands.h"
#include "model/playPhaseValidation.h"
#include "view/print.h"



char outputString[500]; // Initialize String sent to GUI


void RunPlayPhase(const char* input) {
    outputString[0] = '\0';
    //char input[100];
    char command[100];
    strcpy(command, input);

    // Make first two letters uppercase
    for (int i = 0; i < 2 && command[i]; i++) {
        command[i] = toupper(command[i]);
    }

    for (int i = 0; i < 9; i++) {
        command[i] = toupper(command[i]);
    }

    if (strcmp(command, "Q") == 0) {
        currentPhase = STARTUP_PHASE;
        PrintStartupPhase("Q", "Returned to startupPhase.\n");
        return;
    }

    // Undo previous move
    if (strcmp(command, "U") == 0) {
        UndoMove();
    }

    // Redo previous move
    if (strcmp(command, "R") == 0) {
        RedoMove();
    }

    // Handle multi-card column-to-column move: e.g., "C1:4H->C3"
    if (command[2] == ':') {

        // Illegal to move multiple cards between foundations
        if (command[0] == 'F' || command[7] == 'F') {
            PrintPlayPhase(command, "Invalid: can only move top card to/from foundation.\n");
            return;
        }

        // Extract the card to move
        char moveCardID[3] = { command[3], command[4], '\0' };
        Card* from; Card* to; // Declarations for later use
        int count = 0;

        ExtractColumnsFromInput(command, &from, &to, 1);

        // Check if the source card is facedown before allowing the move
        if (from->prev->faceUp == 0) {
            PrintPlayPhase(command, "Invalid move: card is facedown and cannot be moved.\n");
            return;
        }

        // ensures card is in the column and return the position
        if (!IsCardInSourceColumn(from, moveCardID, &count)) {
            PrintPlayPhase(command, "Card not found in source column.\n");
            return;
        }

        Card *moving = from->prev;
        for (int i = 1; i < count; ++i) {    // Go to the count provided
            moving = moving->prev;
        }
        Card *target = to->prev;

        if (!IsValidMove(moving, target, 'C', 'C')) {
            PrintPlayPhase(command, "Invalid move: must be one rank lower and not same suit.\n");
            return;
        }

        // Move it
        MoveTopCards(&from, &to, count);

        // If the top card is now face-down, flip it and save it in my LogMove
        if (from->prev->faceUp == 0 && strcmp(from->prev->ID, "00") != 0) {
            from->prev->faceUp = 1;
            LogNewMove(command, "1");
        } else {
            LogNewMove(command,0);
        }

        PrintPlayPhase(command, "ok");
    }

    // Handle single-card move: e.g., "F1->C3", "C1->F2", "C2->C3"
    else if (command[2] == '-' && command[3] == '>') {
        Card *from, *to; // Declarations for later use
        ExtractColumnsFromInput(command, &from, &to, 0);

        Card *moving = from->prev;
        Card *target = to->prev;

        char fromType = command[0];
        char toType = command[4];
        int valid = 0;


        // Check validity of the move using the helper function
        valid = IsValidMove(moving, target, fromType, toType);

        if (!valid) {
            // Detailed error messages based on move type
            if (fromType == 'C' && toType == 'F') {
                PrintPlayPhase(command, "Invalid move to foundation: must be same suit and one rank higher.\n");
                return;
            } else if (fromType == 'F' && toType == 'C') {
                PrintPlayPhase(command, "Invalid move from foundation: must be one rank lower and different suit.\n");
                return;
            } else if (fromType == 'C' && toType == 'C') {
                PrintPlayPhase(command, "Invalid move between columns: must be one rank lower and different suit.\n");
                return;
            } else {
                PrintPlayPhase(command, "Invalid move type.\n");
                return;
            }
        }

        // Move the cards
        MoveTopCards(&from, &to, 1);


        // If the top card is now face-down, flip it and save it in my LogMove
        if (from->prev->faceUp == 0 && strcmp(from->prev->ID, "00") != 0) {
            from->prev->faceUp = 1;
            LogNewMove(command, "1");
        } else {
            LogNewMove(command,"0");
        }

        PrintPlayPhase(command, "ok");
    }

    // Disallowed commands in play phase
    else if (strncmp(command, "LD", 2) == 0 || strcmp(command, "QQ") == 0 ||
             strncmp(command, "SD", 2) == 0 || strcmp(command, "SW") == 0 ||
             strncmp(command, "SI", 2) == 0 || strcmp(command, "SR") == 0)

    { PrintPlayPhase(command, "Command not available in the PLAY phase\n"); }

    // Unknown command
    else { PrintPlayPhase(command, "Unknown command\n"); }
}

