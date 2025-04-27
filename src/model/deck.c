#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "deck.h"

#include <ctype.h>
#include <time.h>

#include "controller/startupPhase.h"
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
//Give the dummy value of frompile and toPile, to move 'x' amount of topcards from
//'fromPile' to 'toPile'
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

