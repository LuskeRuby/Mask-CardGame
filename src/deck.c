#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "deck.h"

#include <ctype.h>
#include <time.h>

#include "startupPhase.h"
//Global variable
char dummyValue[3] = "00";
Card* dummy = NULL; //Dummyvalue globalvariable from card.h
Card *list = NULL; //initalize list

//Functions
//Create singular CARD:
    Card* CreateCard(char *ID) {
        // allocate a new node
        Card *newCard = (Card *) malloc(sizeof(Card));
        // initialize node data
        strcpy(newCard->ID, ID);
        newCard->faceUp = 0;
        // initialize pointer
        newCard->next = newCard;
        newCard->prev = newCard;
        return newCard;
    }

//Add card to deck (Linkedlist)
int AddCard(Card *newCard, Card **list) {
        //Structure: oldtopcard -> newtopcard -> dummy

        //Change next pointer of the old topcard
        (*list)->prev->next = newCard;
        //Change Newcards pointers
        newCard->next = *list;
        newCard->prev = (*list)->prev;
        //Dummy point to new card
        (*list)->prev = newCard;
        return 0;
    }

// Delete card
int DeleteCard(char *cardID, Card **list) {
        Card* current = (*list)->next; //list starts at dummy by going next we start from start of list
        int IDCompare;

        while (strcmp(current->ID,dummyValue) != 0) { //If reach dummyID again then iterated entire list
            IDCompare = strcmp(cardID, current->ID);

            if (IDCompare != 0) { //No match found, go to next element
                current = current->next;
                continue;
            } if (IDCompare == 0) { //Match found -> remove
                Card* previousCard = current->prev; //previous from element that is removed
                Card* nextCard = current->next; //next from element that is removed

                previousCard->next = nextCard;
                nextCard->prev = previousCard;

                free(current);  // deallocate memory - no longer needed
                return 0; //Done
            }
        }
        return -1; //Card not found in list
    }



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
    FILE* inStream = fopen("data/DEFAULT.txt", "r");
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
        // Convert filename to lowercase to handle case-insensitive input
        char lowerFilename[100];
        strncpy(lowerFilename, filename, sizeof(lowerFilename) - 1);
        lowerFilename[sizeof(lowerFilename) - 1] = '\0';

        for (int i = 0; lowerFilename[i]; i++) {
            lowerFilename[i] = tolower(lowerFilename[i]);
        }

        //all files are located in the /data directory
        char path[100] = "data/";
        strncat(path, lowerFilename, sizeof(path) - strlen(path) - 1);

        inStream = fopen(path, "r");
        if (inStream == NULL) {
            printf("Filename '%s' not found\n", filename);
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

    // Set global list and dummy
    list = deck;
    dummy = deck;
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

    // Traverse the entire circular list (skip dummy at start)
    while (strcmp(current->ID, dummyValue) != 0 && cardCount < 52) {
        // Try to delete this card from the reference deck
        if (DeleteCard(current->ID, &defaultDeck) == -1) {
            printf("Invalid card detected: %s\n", current->ID);
            return 0;
        }
        current = current->next;
        cardCount++;
    }

    // Check if exactly 52 cards were present
    if (cardCount != 52) {
        printf("Error: Deck does not contain exactly 52 cards (found %d).\n", cardCount);
        return 0;
    }

    return 1; // All good!
}

int SD(char* filename) {
    // Fallback to default name if filename is NULL or empty
    const char* defaultName = "cards.txt";
    const char* folder = "data/";

    char fullpath[100];
    if (filename == NULL || strlen(filename) == 0) {
        snprintf(fullpath, sizeof(fullpath), "%s%s", folder, defaultName);
    } else {
        snprintf(fullpath, sizeof(fullpath), "%s%s", folder, filename);
    }

    // Check for valid deck
    if (dummy == NULL || list == NULL || list->next == dummy) {
        printf("Error: No deck loaded to save.\n");
        return 0;
    }

    FILE* outFile = fopen(fullpath, "w");
    if (outFile == NULL) {
        printf("Error: Could not create file '%s'\n", fullpath);
        return 0;
    }

    Card* current = list->next;
    while (strcmp(current->ID, dummyValue) != 0) {
        fprintf(outFile, "%s\n", current->ID);
        current = current->next;
    }

    fclose(outFile);
    printf("Deck saved successfully to '%s'\n", fullpath);
    return 1;
}

void MoveTopCards(Card** fromPile, Card** toPile, int amount) {
        int counter = 0;
        //Topcard and bottomcard of stack that is moved to toPile
        Card* movingTop = (*fromPile)->prev;
        Card* movingBottom = *fromPile; //bottom card of stack that is moved to 'toPile'

        while (amount > counter) { //get the Bottomcard
            movingBottom = movingBottom->prev;
            counter++;
        }

        //Update fromPiles new topcard to point to dummy and other way around
        movingBottom->prev->next = *fromPile;
        (*fromPile)->prev = movingBottom->prev;

        //Update toPiles pointers
        (*toPile)->prev->next = movingBottom;
        movingBottom->prev = (*toPile)->prev;

        (*toPile)->prev = movingTop;
        movingTop->next = *toPile;
    }

int SizeOfDeck(Card** deck) {
        int counter = 0;
        Card* current = (*deck)->next;
        //Go from dummy to dummy = size
        while (strcmp(current->ID, "00") != 0) {
            counter++;
            current = current->next;
        }
        return counter;
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
