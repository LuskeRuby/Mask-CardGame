//
// Created by rubym on 22/04/2025.
//

#include "print.h"
#include "deck.h"
#include <stdio.h>
#include <string.h>

int PrintDeck(int SW, char lastCommand[3], char* msg) {
    Card* current = list->next;  // skip dummy node
    int columnCounter = 0;
    int foundationCounter = 1;

    printf("C1\tC2\tC3\tC4\tC5\tC6\tC7\n\n");

    while (strcmp(current->ID, dummyValue) != 0) {
        if (SW == 1) {
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