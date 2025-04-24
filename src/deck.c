#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "deck.h"

#include <ctype.h>

#include "startupPhase.h"

//Create singular CARD:
    Card* CreateCard(char *ID) {
        // allocate a new node
        //test
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

// Delete card (Returns 00 if not found in list).
//Deletemethod returns the deleted card, remember to free it wont use the deleted card after.
Card* DeleteCard(char *cardID, Card **list) {
        Card* current = (*list)->next; //list starts at dummy by going next we start from start of list
        int IDCompare;

        while (strcmp(current->ID,dummyValue) != 0) { //If reach dummyID again then iterated entire list
            IDCompare = strcmp(cardID, current->ID);

            if (IDCompare != 0) { //No match found, go to next element
                current = current->next;
                continue;
            } if (IDCompare == 0) { //Match found -> remove
                Card* previousCard = current->prev; //previous from current
                Card* nextCard = current->next; //next from current

                //Change pointers
                previousCard->next = previousCard->next->next;
                nextCard->prev = nextCard->prev->prev;

                //Change pointers of returned (delted) card to itself
                current->prev = current;
                current->next = current;
                return current; //Done
            }
        }
    return dummy; //Card not found in list
    }

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
