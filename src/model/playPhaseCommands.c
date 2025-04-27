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
//Extract column data from input has different reading based on if HasCardID = 0 or 1.
void ExtractColumnsFromInput(char *input, Card** fromArr, Card** toArr, char HasCardID) {
    //first assign readable variable names to the data (ColNr are typecast to int)
    char fromColType = input[0]; int fromColNr = (int)(input[1]-'0');
    char toColType; int toColNr;

    //Different reading based on if HasCardID = 0 or 1.
    if (HasCardID == 1) { //Inputtype: XX:YY->ZZ
         toColType = input[7]; toColNr = (int)(input[8]-'0');
    } else if (HasCardID == 0) { //Inputtype: XX->ZZ
        toColType = input[4]; toColNr = (int)(input[5]-'0');
    }
    //Find correct columnArr
    if (fromColType == 'C') { //Normal column
        *fromArr = columnArr[fromColNr -1];
    } else { //foundation
        *fromArr = foundationArr[fromColNr-1];
    }

    if (toColType == 'C') {
        *toArr = columnArr[toColNr-1];
    } else {
        *toArr = foundationArr[toColNr-1];
    }
}















