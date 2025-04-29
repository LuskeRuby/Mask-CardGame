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
        fgets(input, sizeof(input),stdin);
        input[strcspn(input, "\n")] = 0; // Remove trailing newline

        //Check if command is exacly "Q"
        if (strcmp(input, "Q") == 0) {
            printf("Returned to startupPhase.\n");
            RunStartupPhase();
        }

        //If input has cardID: xx:yy->zz
        else if (input[2] == ':') {

            //Extract CardID from input
            char moveCardID[3];
            moveCardID[0] = input[3];
            moveCardID[1] = input[4];
            moveCardID[2] = '\0';

            //Extract columndata from input
            Card* fromArr;
            Card* toArr;
            ExtractColumnsFromInput(&input,  &fromArr, &toArr,1);

            //Check if cardID is in column and find amount of cards to move with the Card
            int amountCounter = 0;
            char found = '0';
            Card* cardIterator = fromArr->prev;
            while (strcmp(cardIterator->ID,"00")) {
                amountCounter++;
                if (strcmp(cardIterator->ID, moveCardID) == 0) {
                    found = '1';
                    break;
                }
                cardIterator = cardIterator->prev;
            }

            //Move cards
            if (found == '1') { //If we found the card in the column then run validation, otherwise print error
                Card* movingCard = cardIterator;
                Card* targetCard = toArr->prev;

                if (strncmp(&input[strlen(input) - 2], "C", 1) == 0) {
                    // Column-to-column
                    if (strcmp(targetCard->ID, "00") == 0) {
                        // Only King can be placed in empty column
                        if (ConvertRank(movingCard->ID[0]) != 13) {
                            printf("Invalid move: only King can be placed on empty column.\n");
                        }
                    } else {
                        // Extract and convert the rank so it's all numbers for comparisons
                        int mRank = ConvertRank(movingCard->ID[0]);
                        int tRank = ConvertRank(targetCard->ID[0]);
                        char mSuit = movingCard->ID[1];
                        char tSuit = targetCard->ID[1];

                        if (mRank + 1 != tRank || mSuit == tSuit) {
                            printf("Invalid move: must be one rank lower and not same suit.\n");
                        }
                    }

                } else if (strncmp(&input[strlen(input) - 2], "F", 1) == 0) {
                    // Column-to-foundation
                    if (strcmp(targetCard->ID, "00") == 0) {
                        // Only Ace can be placed on empty foundation
                        if (ConvertRank(movingCard->ID[0]) != 1) {
                            printf("Invalid move: only Ace can be placed on empty foundation.\n");
                        }
                    } else {
                        int mRank = ConvertRank(movingCard->ID[0]);
                        int tRank = ConvertRank(targetCard->ID[0]);
                        char mSuit = movingCard->ID[1];
                        char tSuit = targetCard->ID[1];

                        if (mSuit != tSuit || mRank != tRank + 1) {
                            printf("Invalid move to foundation: must be same suit and one rank higher.\n");
                        }
                    }
                }


                //if input doesnt have cardID: xx->zz
                else if (input[2] == '-' && input[3] == '>') {
                    Card* fromArr = CreateCard("00");
                    Card* toArr = CreateCard("00");
                    ExtractColumnsFromInput(&input,  &fromArr, &toArr, 0);

                    Card* movingCard = fromArr->prev;
                    Card* targetCard = toArr->prev;

                    // Foundation to column
                    if (strncmp(&input[0], "F", 1) == 0 && strncmp(&input[strlen(input) - 2], "C", 1) == 0) {
                        int mRank = ConvertRank(movingCard->ID[0]);
                        int tRank = ConvertRank(targetCard->ID[0]);
                        char mSuit = movingCard->ID[1];
                        char tSuit = targetCard->ID[1];

                        if (mRank + 1 != tRank || mSuit == tSuit) {
                            printf("Invalid move: must be one rank lower and not same suit.\n");
                            continue;
                        }
                    }

                    //If passed the check
                    MoveTopCards(&fromArr, &toArr, 1);
                    PrintPlayPhase("ok", "ok");
                }

                //Check if command is a startupPhase Command
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
    }
}








