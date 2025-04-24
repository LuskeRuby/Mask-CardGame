//
// Created by rubym on 22/04/2025.
//

#include "print.h"
#include "model/deck.h"
#include <stdio.h>
#include <string.h>

#include "model/playPhaseCommands.h"

int PrintDeck(char lastCommand[3], char* msg) {
    Card* current = list->next;  // skip dummy node
    int columnCounter = 0;
    int foundationCounter = 1;

    printf("C1\tC2\tC3\tC4\tC5\tC6\tC7\n\n");

    while (strcmp(current->ID, dummyValue) != 0) {
        if (current->faceUp == 1) {
            printf("%2s\t", current->ID);  // face up
        } else {
            printf("%2s\t", "[]");        // face down
        }

        columnCounter++;
        if (columnCounter == 7) {
            if (foundationCounter < 5) {
                printf("\t[]\tF%d", foundationCounter);
                foundationCounter++;
            }
            printf("\n");
            columnCounter = 0;
        }

        current = current->next;
    }

    printf("\n\nLAST Command: %s\n", lastCommand);
    printf("Message: %s\n", msg);

    return 0;
}

void PrintPlayPhase(char lastCommand[3], char* msg) {
    int fullyPrintedColumns = 0;
    int foundationCounter = 1;
    printf("C1\tC2\tC3\tC4\tC5\tC6\tC7\n\n");

    while (fullyPrintedColumns < 7) { //Rowprinter, continues until all 7 columns are done printing
        fullyPrintedColumns = 0;
        for (int col = 0; col < 7; col++) { //iterate columns

            //If a linkedlist has reached dummyvalue then its column is fully printed
            if (strcmp(columnArr[col]->next->ID, "00") != 0) {
                columnArr[col] = columnArr[col]->next; //only go next if not reached dummy
                if (columnArr[col]->faceUp == 1) {
                    printf("%2s\t", columnArr[col]->ID);
                } else {
                    printf("[]\t");
                }
            } else { //reached dummyvalue (col fully printed)
                fullyPrintedColumns++;
                printf("\t"); //print tab
            }
        }
        if (foundationCounter < 5) {
            if (strcmp(foundationArr[foundationCounter-1]->prev->ID, "00") == 0) { //if empty deck print []
                printf("\t[]\tF%d",foundationCounter);
            } else {
                //print id of topcard
                printf("\t%2s\tF%d",foundationArr[foundationCounter-1]->prev->ID, foundationCounter);
            }
            foundationCounter++;
        }
        printf("\n");
    }
    printf("\n\nLAST Command: %s\n", lastCommand);
    printf("Message: %s\n", msg);
}