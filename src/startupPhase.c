#include "startupPhase.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "card.h"

char dummyValue[3] = "00";
Card* dummy = NULL; //Dummyvalue globalvariable from card.h
Card *list = NULL; //initalize list

Card* BuildLinkedListFromFile(FILE* stream) {
    if (stream == NULL) return NULL;

    char data[3];
    int cardCount = 0;

    Card* dummy = CreateCard(dummyValue);
    dummy->next = dummy;
    dummy->prev = dummy;
    Card* list = dummy;

    while (fscanf(stream, "%2s", data) != EOF && cardCount < 52) {
        Card* c = CreateCard(data);
        AddCard(c, &list);
        cardCount++;
    }

    return list; // returns the dummy node as the head
}


Card* LoadDefaultDeck() {
    FILE* inStream = fopen("DEFAULT.txt", "r");
    if (inStream == NULL) {
        printf("Error: Could not open DEFAULT.txt\n");
        return NULL;
    }

    Card* deck = BuildLinkedListFromFile(inStream);
    fclose(inStream);
    return deck;
}



char* LD(char* filename) {
    FILE* inStream;
    Card* deck;

    if (filename == NULL) {
        deck = LoadDefaultDeck();
        if (deck == NULL) {
            return "0";
        }
    } else {
        inStream = fopen(filename, "r");
        if (inStream == NULL) {
            printf("Filename not found\n");
            return "0";
        }
        deck = BuildLinkedListFromFile(inStream);
        fclose(inStream);
        if (deck == NULL) {
            return "0";
        }
        if (LDValidation(deck) == 0) {
            printf("Error: Illigal deck of cards\n");
            return "0";
        }
    }

    // Set global list and dummy
    list = deck;
    dummy = deck;
    PrintInitialDeck(1, "LD", "OK");
    return "OK";
}

void RunStartupPhase() {
    char input[100];

    printf("Enter commands (type 'LD' to load, 'P' to Enter StartPhase, 'QQ' to quit):\n");

    while(1) {
        printf("INPUT > ");
        fgets(input, sizeof(input),stdin);
        input[strcspn(input, "\n")] = 0; // Remove trailing newline

        //Check if command is exacly "LD"
        if (strcmp(input, "LD") == 0) {
            char* result = LD(NULL);
            if (strcmp(result, "OK") == 0)
                printf("Default deck loaded.\n");
            else
                printf("Error loading default deck.\n");
        }

        else if (strncmp(input, "LD(", 3) == 0 && input[strlen(input) - 1] == ')') {
            // Extract filename inside the parentheses
            char filename[80];
            strncpy(filename, input + 3, strlen(input) - 4);
            filename[strlen(input) - 4] = '\0';  // Null-terminate properly

            char* result = LD(filename);
            if (strcmp(result, "OK") == 0)
                printf("Deck '%s' loaded.\n", filename);
            else
                printf("Error loading deck '%s'.\n", filename);
        }

        else if (strcmp(input, "QQ") == 0) {
            printf("The program exits");
            exit(1);
        }

        else if (strcmp(input, "P") == 0) {
            RunPlayPhase();
        }

        else {
            printf("Unknown command or input too many characters\n");
        }
    }
}