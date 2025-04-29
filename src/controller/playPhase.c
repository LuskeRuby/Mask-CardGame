#include "playPhase.h"
#include <stdio.h>
#include <string.h>
#include <model/deck.h>
#include <view/PRINT.H>

#include "model/playPhaseCommands.h"



void RunPlayPhase() {
    char input[100];
    printf(">>play phase<<. Enter Commands 'Q' to return to the startup Phase):\n");
    InitArray();
    PrintPlayPhase("ok", "ok");

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
            char moveCardID[3];
            moveCardID[0] = input[3];
            moveCardID[1] = input[4];
            moveCardID[2] = '\0';

            Card* fromArr;
            Card* toArr;
            ExtractColumnsFromInput(input, &fromArr, &toArr, 1);

            // Disallow moving from or to foundation using multi-card format
            if (input[0] == 'F' || input[7] == 'F') {
                printf("Invalid: can only move top card to/from foundation.\n");
                continue;
            }

            int amountCounter = 0;
            char found = '0';
            Card* cardIterator = fromArr->prev;
            while (strcmp(cardIterator->ID, "00")) {
                amountCounter++;
                if (strcmp(cardIterator->ID, moveCardID) == 0) {
                    found = '1';
                    break;
                }
                cardIterator = cardIterator->prev;
            }

            if (found == '1') {
                Card* movingCard = cardIterator;
                Card* targetCard = toArr->prev;

                int mRank = ConvertRank(movingCard);
                int tRank = ConvertRank(targetCard);
                char mSuit = movingCard->ID[1];
                char tSuit = targetCard->ID[1];

                // Column to column validation
                if (strcmp(targetCard->ID, "00") == 0) {
                    if (mRank != 13) {
                        printf("Invalid move: only King can be placed on empty column.\n");
                        continue;
                    }
                } else if (mRank + 1 != tRank || mSuit == tSuit) {
                    printf("Invalid move: must be one rank lower and not same suit.\n");
                    continue;
                }

                MoveTopCards(&fromArr, &toArr, amountCounter);
                //If columns topcard is facedown, turn it faceup
                if (fromArr->prev->faceUp == 0 && strcmp(fromArr->prev->ID, "00") != 0) {
                    fromArr->prev->faceUp = 1;
                }
                PrintPlayPhase("ok", "ok");
            } else {
                printf("Card not found in source column.\n");
            }
        }

        // Input is simple move (format: F1->C3 or C1->F2)
        else if (input[2] == '-' && input[3] == '>') {
            Card* fromArr;
            Card* toArr;
            ExtractColumnsFromInput(input, &fromArr, &toArr, 0);

            Card* movingCard = fromArr->prev;
            Card* targetCard = toArr->prev;

            // Only top card can be moved from a column/foundation
            if (movingCard->faceUp == 0 || strcmp(movingCard->ID, "00") == 0) {
                printf("Invalid: only top, face-up cards can be moved.\n");
                continue;
            }

            // If either side is a foundation, only allow moving one card
            if ((input[0] == 'F' || input[4] == 'F')) {
                // Must be moving exactly one card and from the top
                if (movingCard != fromArr->prev) {
                    printf("Invalid: only top card can be moved to/from a foundation.\n");
                    continue;
                }
            }

            int mRank = ConvertRank(movingCard);
            int tRank = ConvertRank(targetCard);
            char mSuit = movingCard->ID[1];
            char tSuit = targetCard->ID[1];

            char fromType = input[0];
            char toType = input[4];

            if (fromType == 'F' && toType == 'C') {
                if (strcmp(targetCard->ID, "00") == 0) {
                    if (mRank != 13) {
                        printf("Invalid move: only King can be placed on empty column.\n");
                        continue;
                    }
                } else if (mRank + 1 != tRank || mSuit == tSuit) {
                    printf("Invalid move: must be one rank lower and not same suit.\n");
                    continue;
                }
            } else if (fromType == 'C' && toType == 'F') {
                if (strcmp(targetCard->ID, "00") == 0) {
                    if (mRank != 1) {
                        printf("Invalid move: only Ace can be placed on empty foundation.\n");
                        continue;
                    }
                } else if (mSuit != tSuit || mRank != tRank + 1) {
                    printf("Invalid move to foundation: must be same suit and one rank higher.\n");
                    continue;
                }
            } else if (fromType == 'C' && toType == 'C') {
                if (strcmp(targetCard->ID, "00") == 0) {
                    if (mRank != 13) {
                        printf("Invalid move: only King can be placed on empty column.\n");
                        continue;
                    }
                } else if (mRank + 1 != tRank || mSuit == tSuit) {
                    printf("Invalid move: must be one rank lower and not same suit.\n");
                    continue;
                }
            }

            MoveTopCards(&fromArr, &toArr, 1);
            //If columns topcard is facedown, turn faceup
            if (fromArr->prev->faceUp == 0 && strcmp(fromArr->prev->ID, "00") != 0) {
                fromArr->prev->faceUp = 1;
            }
            PrintPlayPhase("ok", "ok");
        }

        // Invalid in play phase
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
