//
// Created by rubym on 22/04/2025.
//

#include "print.h"
#include "model/deck.h"
#include <stdio.h>
#include <string.h>
#include <controller/playPhase.h>

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

    // temp pointers to not change original columnArr
    Card* tempColumnArr[7];
    for (int i = 0; i < 7; i++) {
        tempColumnArr[i] = columnArr[i];
    }

    // Ensure outputString starts as an empty string before we append to it
    outputString[0] = '\0';

    // Append header to the output string (but do NOT print)
    strcat(outputString, "C1\tC2\tC3\tC4\tC5\tC6\tC7\n\n");

    // Row printer, continues until all 7 columns are done printing
    while (fullyPrintedColumns < 7) {
        fullyPrintedColumns = 0;
        for (int col = 0; col < 7; col++) { // iterate columns

            // If a linked list has reached dummy value then its column is fully printed
            if (strcmp(tempColumnArr[col]->next->ID, "00") != 0) {
                tempColumnArr[col] = tempColumnArr[col]->next; // only go next if not reached dummy
                if (tempColumnArr[col]->faceUp == 1) {
                    // Concatenate the ID to outputBuffer
                    strcat(outputString, tempColumnArr[col]->ID);
                    strcat(outputString, "\t");
                } else {
                    strcat(outputString, "[]\t");
                }
            } else { // reached dummy value (column fully printed)
                fullyPrintedColumns++;
                strcat(outputString, "\t");
            }
        }

        // Handle foundations printing
        if (foundationCounter < 5) {
            if (strcmp(foundationArr[foundationCounter - 1]->prev->ID, "00") == 0) { // if empty deck print []
                strcat(outputString, "\t[]\tF");
                char fCount[3];
                sprintf(fCount, "%d", foundationCounter);
                strcat(outputString, fCount);
            } else {
                // print id of top card
                strcat(outputString, "\t");
                strcat(outputString, foundationArr[foundationCounter - 1]->prev->ID);
                strcat(outputString, "\tF");
                char fCount[3];
                sprintf(fCount, "%d", foundationCounter);
                strcat(outputString, fCount);
            }
            foundationCounter++;
        }

        strcat(outputString, "\n");
    }

    // Capture the last command and message
    strcat(outputString, "\n\nLAST Command: ");
    strcat(outputString, lastCommand);
    strcat(outputString, "\n");

    strcat(outputString, "Message: ");
    strcat(outputString, msg);
    strcat(outputString, "\n");

    //PRINT STRING
    printf("Captured Output:\n%s", outputString); // Print the full captured string after the function call
}