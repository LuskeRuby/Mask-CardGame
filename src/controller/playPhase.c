#include "playPhase.h"
#include <stdio.h>
#include <string.h>
#include <model/deck.h>
#include "view/print.h"

#include "model/playPhaseCommands.h"
#include "model/playPhaseValidation.h"
char outputString[500]; //Initialize String sent to GUI


void RunPlayPhase() {
   outputString[0] = '\0';
    char input[100];
    printf(">>play phase<<. Enter Commands ('Q' to return to the startup Phase):\n");
    InitArray();
    PrintPlayPhase("P", "ok");
    while (1) {
        printf("INPUT > ");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = 0; // Remove trailing newline

        if (strcmp(input, "Q") == 0) {
            printf("Returned to startupPhase.\n");
            RunStartupPhase();
        }

        // Input contains card ID (format: C1:4H->C3)
        else if (input[2] == ':') {
            char moveCardID[3] = {input[3], input[4], '\0'};
            Card* fromArr;
            Card* toArr;
            ExtractColumnsFromInput(input, &fromArr, &toArr, 1);

            // Disallow moving from or to foundation using multi-card format
            if (IsMultiCardMoveToOrFromFoundation(input)) {
                printf("Invalid: can only move top card to/from foundation.\n");
                continue;
            }

            int amountCounter = 0;

            if (!IsCardInSourceColumn(fromArr, moveCardID, &amountCounter)) {
                printf("Card not found in source column.\n");
                continue;
            }

            Card* movingCard = fromArr->prev;
            for (int i = 1; i < amountCounter; ++i) movingCard = movingCard->prev;
            Card* targetCard = toArr->prev;

            if (!IsValidColumnToColumnMove(movingCard, targetCard)) {
                printf("Invalid move: must be one rank lower and not same suit.\n");
                continue;
            }

            MoveTopCards(&fromArr, &toArr, amountCounter);

            if (fromArr->prev->faceUp == 0 && strcmp(fromArr->prev->ID, "00") != 0) {
                fromArr->prev->faceUp = 1;
            }

            PrintPlayPhase(input, "ok");
        }

        // Input is simple move (format: F1->C3 or C1->F2)
         else if (input[2] == '-' && input[3] == '>') {
            Card* fromArr;
            Card* toArr;
            ExtractColumnsFromInput(input, &fromArr, &toArr, 0);

            Card* movingCard = fromArr->prev;
            Card* targetCard = toArr->prev;

            if (!IsTopFaceUpCard(fromArr, movingCard)) {
                printf("Invalid: only top, face-up cards can be moved.\n");
                continue;
            }

            char fromType = input[0];
            char toType = input[4];

            int valid = 0;

            if (fromType == 'F' && toType == 'C') {
                valid = IsValidFoundationToColumnMove(movingCard, targetCard);
                if (!valid) printf("Invalid move from foundation: must be one rank lower and not same suit.\n");
            }
            else if (fromType == 'C' && toType == 'F') {
                valid = IsValidColumnToFoundationMove(movingCard, targetCard);
                if (!valid) printf("Invalid move to foundation: must be same suit and one rank higher.\n");
            }
            else if (fromType == 'C' && toType == 'C') {
                valid = IsValidColumnToColumnMove(movingCard, targetCard);
                if (!valid) printf("Invalid move: must be one rank lower and not same suit.\n");
            }
            else {
                printf("Invalid move type.\n");
                continue;
            }

            if (!valid) continue;

            MoveTopCards(&fromArr, &toArr, 1);

            if (fromArr->prev->faceUp == 0 && strcmp(fromArr->prev->ID, "00") != 0) {
                fromArr->prev->faceUp = 1;
            }

            PrintPlayPhase(input, "ok");
        }

        else if (strncmp(input, "LD", 2) == 0 ||
                 strncmp(input, "SD", 2) == 0 ||
                 strcmp(input, "SW") == 0 ||
                 strncmp(input, "SI", 2) == 0 ||
                 strcmp(input, "SR") == 0 ||
                 strcmp(input, "QQ") == 0) {
            printf("Command not available in the PLAY phase \n");
        }

        else {
            printf("Unknown command \n");
        }
    }
}