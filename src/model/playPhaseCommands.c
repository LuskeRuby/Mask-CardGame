//
// Created by rasmu on 24-04-2025.
//
#include "deck.h"
#include "playPhaseCommands.h"

#include <string.h>

//Global arrays
Card* columnArr[7];
Card* foundationArr[4];

void InitArray() {
    //Populate colum and foundation arrays with dummyvalues.
    for (int i = 0; i < 7; i++) {
        columnArr[i] = CreateCard(dummyValue);
        if (i < 4) {
            foundationArr[i] = CreateCard(dummyValue);
        }
    }

    //Populate columnarray with current list (deck)
    int k = 0; //We populate arrays between k to 6.
    Card* Bottomcard = list->next->next; //list->next->next is a workaround to move a bottomcard with function MoveTopCards()
    for (int row = 0; row < 11; row++) {
        for (int col = k; col < 7; col++) {
            list->next->faceUp = 1; //turn faceup
            MoveTopCards(&Bottomcard,&columnArr[col],1);
            Bottomcard = list->next->next;
        }
        if (row == 0 || row > 4) { //if condtion met, decrease the column from the left
            k++;
        }
    }
    //Turn certain cards facedown
    k = 5;
    Card* colPointer;
    for (int col = 1; col < 7; col++) { //Iterate the 6 cols having cards facedown
        colPointer = columnArr[col];
        for (int rows = k; rows < 6; rows++) { //Turn the cards between k to 6 facedown
            colPointer = colPointer->next;
            colPointer->faceUp = 0; //turned facedown
        }
        k--; //Make the next iterated col turn an additional card facedown
    }
}














