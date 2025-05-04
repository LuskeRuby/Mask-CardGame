#include "playPhase.h"

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <model/deck.h>
#include "view/print.h"

#include "model/playPhaseCommands.h"
#include "model/playPhaseValidation.h"


char outputString[500]; // Initialize String sent to GUI


void RunPlayPhase() {
    outputString[0] = '\0';
    char input[100];

    printf(">>play phase<<. Enter Commands ('Q' to return to the startup Phase):\n");

    InitArray();
    PrintPlayPhase("P", "ok");


    while (1) {
        printf("INPUT > ");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0'; // Strip newline

        // Make first two letters uppercase
        for (int i = 0; i < 2 && input[i]; i++) {
            input[i] = toupper(input[i]);
        }


        if (strcmp(input, "Q") == 0) {
            printf("Returned to startupPhase.\n");
            RunStartupPhase();
            return;
        }

        // Handle multi-card column-to-column move: e.g., "C1:4H->C3"
        if (input[2] == ':') {

            // Illegal to move multiple cards between foundations
            if (input[0] == 'F' || input[7] == 'F') {
                printf("Invalid: can only move top card to/from foundation.\n");
                continue;
            }

            // Extract the card to move
            char moveCardID[3] = { input[3], input[4], '\0' };
            Card* from; Card* to; // Declarations for later use
            int count = 0;

            ExtractColumnsFromInput(input, &from, &to, 1);

            // Check if the source card is facedown before allowing the move
            if (from->prev->faceUp == 0) {
                printf("Invalid move: card is facedown and cannot be moved.\n");
                continue;
            }

            // ensures card is in the column and return the position
            if (!IsCardInSourceColumn(from, moveCardID, &count)) {
                printf("Card not found in source column.\n");
                continue;
            }

            Card *moving = from->prev;
            for (int i = 1; i < count; ++i) {    // Go to the count provided
                moving = moving->prev;
            }
            Card *target = to->prev;

            if (!IsValidMove(moving, target, 'C', 'C')) {
                printf("Invalid move: must be one rank lower and not same suit.\n");
                continue;
            }

            // Move it
            MoveTopCards(&from, &to, count);

            // If top card is now facedown, flip it
            if (from->prev->faceUp == 0 && strcmp(from->prev->ID, "00") != 0) {
                from->prev->faceUp = 1;
            }

            PrintPlayPhase(input, "ok");
        }

        // Handle single-card move: e.g., "F1->C3", "C1->F2", "C2->C3"
        else if (input[2] == '-' && input[3] == '>') {

            Card *from, *to; // Declarations for later use
            ExtractColumnsFromInput(input, &from, &to, 0);

            Card *moving = from->prev;
            Card *target = to->prev;

            char fromType = input[0];
            char toType = input[4];
            int valid = 0;


            // Check validity of the move using the helper function
            valid = IsValidMove(moving, target, fromType, toType);

            if (!valid) {
                // Detailed error messages based on move type
                if (fromType == 'C' && toType == 'F') {
                    printf("Invalid move to foundation: must be same suit and one rank higher.\n");
                } else if (fromType == 'F' && toType == 'C') {
                    printf("Invalid move from foundation: must be one rank lower and different suit.\n");
                } else if (fromType == 'C' && toType == 'C') {
                    printf("Invalid move between columns: must be one rank lower and different suit.\n");
                } else {
                    printf("Invalid move type.\n");
                }
                continue;
            }

            // Move the cards
            MoveTopCards(&from, &to, 1);

            // If the top card is now face-down, flip it
            if (from->prev->faceUp == 0 && strcmp(from->prev->ID, "00") != 0) {
                from->prev->faceUp = 1;
            }

            PrintPlayPhase(input, "ok");
        }

        // Disallowed commands in play phase
        else if (strncmp(input, "LD", 2) == 0 ||
                 strncmp(input, "SD", 2) == 0 ||
                 strcmp(input, "SW") == 0 ||
                 strncmp(input, "SI", 2) == 0 ||
                 strcmp(input, "SR") == 0 ||
                 strcmp(input, "QQ") == 0) {
            printf("Command not available in the PLAY phase\n");
        }

        // Unknown command
        else {
            printf("Unknown command\n");
        }
    }
}
