//
// Created by rubym on 14/04/2025.
//

#include "playPhase.h"
#include <stdio.h>
#include <string.h>
#include <model/deck.h>
#include <view/print.h>

#include "model/playPhaseCommands.h"



void RunPlayPhase() {
    char input[100];
    printf(">>play phase<<. Enter Commands 'Q' to return to the startup Phase):\n");
    InitArray();
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
            moveCardID[0] = input[3]; moveCardID[1] = input[4]; moveCardID[2] = '\0';

            //Extract columndata from input
            Card* fromArr;   Card* toArr;
            ExtractColumnsFromInput(&input,  &fromArr, &toArr,1);

            //Check if cardID is in column and find amount of cards to move with the Card
            int amountCounter = 0; char found = '0'; Card* cardIterator = fromArr->prev;
            while (strcmp(cardIterator->ID,"00")) {
                amountCounter++;
                if (strcmp(cardIterator->ID, moveCardID) == 0) {
                    found = '1';
                    break;
                }
                cardIterator = cardIterator->prev;
            }

            //Move cards
            if (found == '1') { //If we found the card in the column then move, otherwise print error
                MoveTopCards(&fromArr,&toArr,amountCounter);
            } else {
                printf("The card moved does not exist at the specified location. \n");
            }
            //Print the cards in console
            PrintPlayPhase("ok", "ok");
        }
        //if input doesnt have cardID: xx->zz
        else if (input[2] == '-' && input[3] == '>') {
            Card* fromArr;   Card* toArr;
            ExtractColumnsFromInput(&input,  &fromArr, &toArr, 0);

            MoveTopCards(&fromArr,&toArr,1);
            PrintPlayPhase("ok", "ok");
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
