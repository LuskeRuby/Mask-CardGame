#include "card.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "startupPhase.h"

//Create singular CARD:
    Card* CreateCard(char *ID) {
        Card* newCard = malloc(sizeof(Card));
        // initialize node data
        strcpy(newCard->ID, ID);
        newCard->faceUp = 0;
        // initialize pointer
        newCard->next = newCard;
        newCard->prev = newCard;
        return newCard;
    }
/*
*Card* CreateCard(char *cardRank, char *cardSuit) {
        // allocate a new node
        Card *newCard = (Card *) malloc(sizeof(Card));
        // initialize node data
        newCard->ID[0] = *cardRank;
        newCard->ID[1] = *cardSuit;
        newCard->ID[2] = '\0';
        newCard->faceUp = 0;
        // initialize pointer
        newCard->next = newCard;
        newCard->prev = newCard;
        return newCard;
    }
    */



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

        //Old shit
        /*
        (*list)->prev = newCard->prev;
        (*list)->prev = newCard;
        newCard->next = *list;
        *list = newCard;
        (*list)->prev->next=*list; ;
        */
        return 0;
    }

int PrintInitialDeck(int SW, char lastCommand[3], char* msg) {
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

int LDValidation(Card* Deck) {
        Card* defaultDeck = LoadDefaultDeck();  // So it does not overwrite my global variable.

        if (!defaultDeck) {
            printf("Error loading default deck for validation\n");
            return 0;
        }

        Card* current = Deck;
        char cardID[10];

        while (current != NULL) {

            snprintf(cardID, sizeof(cardID), "%s", current->ID);

            // Try to delete from defaultDeck
            if (!DeleteCard(cardID, &defaultDeck)) {
                return 0; // Card not found in default deck
            }

            current = current->next;
        }

        return 1; // All cards matched
    }
