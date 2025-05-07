
#include "StartPhaseCommands.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include "deck.h"


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
    FILE* inStream = fopen("data/default.txt", "r");
    if (inStream == NULL) {
        printf("Error: Could not open default.txt\n");
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
        char finalFilename[100];
        strncpy(finalFilename, filename, sizeof(finalFilename) - 1);
        finalFilename[sizeof(finalFilename) - 1] = '\0';

        // Append .txt if missing
        if (strlen(finalFilename) < 4 || strcmp(finalFilename + strlen(finalFilename) - 4, ".txt") != 0) {
            strncat(finalFilename, ".txt", sizeof(finalFilename) - strlen(finalFilename) - 1);
        }

        // Build full path
        char path[150] = "data/";
        strncat(path, finalFilename, sizeof(path) - strlen(path) - 1);

        // Open file
        inStream = fopen(path, "r");
        if (inStream == NULL) {
            printf("Error: Could not open file '%s'\n", path);
            return "0";
        }

        deck = BuildLinkedListFromFile(inStream);
        fclose(inStream);

        if (deck == NULL) {
            return "0";
        }

        if (LDValidation(deck) == 0) {
            printf("Error: Illegal deck of cards\n");
            return "0";
        }
    }

    list = deck;
    return "OK";
}


int LDValidation(Card* deckToValidate) {
    // Load a fresh default deck to compare against
    Card* defaultDeck = LoadDefaultDeck();
    if (!defaultDeck) {
        printf("Error: Failed to load default deck for validation.\n");
        return 0;
    }

    Card* current = deckToValidate->next;
    int cardCount = 0;

    // Traverse the entire list (skip dummy at start)
    while (strcmp(current->ID, dummyValue) != 0 && cardCount < 52) {
        // Try to delete this card from the reference deck
        if (DeleteCard(current->ID, &defaultDeck) == -1) {
            printf("Invalid card detected: %s\n", current->ID);
            return 0;
        }
        current = current->next;
        cardCount++;
    }

    // Check if exactly 52 cards
    if (cardCount != 52) {
        printf("Error: Deck does not contain exactly 52 cards (found %d).\n", cardCount);
        return 0;
    }

    return 1; // All good
}


char* SD(char* filename) {
    const char* folder = "data/";
    const char* defaultName = "cards.txt";
    static char fullpath[150];

    if (filename == NULL) {
        // Use default filename
        strcpy(fullpath, folder);
        strcat(fullpath, defaultName);
    } else if (strlen(filename) == 0) {
        printf("Error: filename must not be an empty string.\n");
        return "0";

    } else {
        static char finalFilename[100];
        strncpy(finalFilename, filename, sizeof(finalFilename) - 1);
        finalFilename[sizeof(finalFilename) - 1] = '\0';

        // Append .txt if missing
        int len = (int)strlen(finalFilename);
        if (len < 4 || strcmp(finalFilename + len - 4, ".txt") != 0) {
            strncat(finalFilename, ".txt", sizeof(finalFilename) - strlen(finalFilename) - 1);
        }

        strcpy(fullpath, folder);
        strcat(fullpath, finalFilename);
    }

    FILE* outFile = fopen(fullpath, "w");
    if (outFile == NULL) {
        printf("Error: Could not create file '%s'\n", fullpath);
        return "0";
    }

    // Write deck
    Card* current = list->next;
    while (strcmp(current->ID, dummyValue) != 0) {
        fputs(current->ID, outFile);
        fputc('\n', outFile);  // add newline
        current = current->next;
    }

    fclose(outFile);
    printf("Deck saved successfully to '%s'\n", fullpath);
    return "OK";
}




void SI(int split) {

    if (split > 0 && split < 52) { // check split parameter
        } else { //Invalid parameter (Get random number between 1 and 52)
            srand(time(NULL));
            split = (rand() % 52) + 1;
        }

    Card* pile1 = list; //pile1 = Main deck
    //Make pile2
    Card* pile2Dummy = CreateCard(dummyValue);
    Card* pile2 = pile2Dummy;
    int counter = 0;

    //Moves 'split' amount of topcards from pile1 to pile2
    MoveTopCards(&pile1,&pile2,split);

    //Create another empty deck used for shuffling.
    Card* shuffledDummy = CreateCard(dummyValue);
    Card* shuffledDeck = shuffledDummy;

    Card* pile2Top = pile2->prev;
    Card* pile1Top = pile1->prev;
//interleave cards from pile1&2 to shuffledDeck, until either deck hits the dummy card (dummy = deck is exhausted)
    while (strcmp(pile2Top->ID, "00") != 0 && strcmp(pile1Top->ID, "00") != 0) {
        //move card to shuffledDeck

        MoveTopCards(&pile2,&shuffledDeck,1);
        MoveTopCards(&pile1,&shuffledDeck,1);
        pile2Top = pile2->prev;
        pile1Top = pile1->prev;
    }
    //Add shuffleddeck ontop of the remaining of pile1 or 2
    if (strcmp(pile1Top->ID, dummyValue) != 0) { //If pile 2 exhausted
        MoveTopCards(&shuffledDeck,&pile1,SizeOfDeck(&shuffledDeck));

        //Make pile1 our main deck (global variable)
        list = pile1;
    }
    if (strcmp(pile2Top->ID, dummyValue) != 0) { //if pile1 exhausted
        MoveTopCards(&shuffledDeck,&pile2,SizeOfDeck(&shuffledDeck));

        //Make pile1 our main deck (global variable)
        list = pile2;
    }
}

void SR() {
    //Create pile and shuffled pile
    Card* pile1 = list;
    Card* dummy1 = CreateCard(dummyValue);
    Card* shuffledPile = dummy1;

    //Local variables
    int shuffledPileSize = 1; //Size of shuffledPile (starts at 1 cuz we cant modulo with 0)
    Card* randomPilePosition = shuffledPile; //Random position in shuffledPile

    //Loop all 52 cards
    for (int i = 0; i < 52; i++) {
        //get Get random number between 1 and size of pile
        srand(time(NULL));
        int randomNr = (rand() % shuffledPileSize) + 1;

        //Get the Pile position from the given random nr
        for (int j = 0; j < randomNr; j++) {
            randomPilePosition = randomPilePosition->next;
        }
        //Move 1 (amount = 1) topcard from pile1 to randompileposition in shuffleddeck
        MoveTopCards(&pile1,&randomPilePosition,1);
        shuffledPileSize++;
    }
    list = shuffledPile; //Update the global list variable
}

void SW() {
    Card* listptr = list->next;
    while (strcmp(listptr->ID, dummyValue) != 0) {
        listptr->faceUp = 1;
        listptr = listptr->next;
    }

}