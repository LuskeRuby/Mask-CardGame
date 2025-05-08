//
// Created by rasmu on 24-04-2025.
//
#include "MODEL/deck.h"
#include "playPhaseCommands.h"

#include <string.h>
#include <controller/playPhase.h>

#include "playPhaseValidation.h"
#include "StartPhaseCommands.h"

//Global arrays
Card* columnArr[7];
Card* foundationArr[4];


void InitArray() {
    // save my deck so it can be brought back after
    SD("temp_deck");

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
    // makes sure my Deck is retained and deletes it.
    LD("temp_deck");
    remove("data/temp_deck.txt");
}

//Extract column data from input has different reading based on if HasCardID = 0 or 1.
void ExtractColumnsFromInput(char *input, Card** fromArr, Card** toArr, char HasCardID) {
    //first assign readable variable names to the data (ColNr are typecast to int)
    char fromColType = input[0];            // e.g., 'C' from "C1..."
    int fromColNr = (int)(input[1]-'0');    // e.g., 1 from "C1" → converts char to int
    char toColType; int toColNr;            // destination type and number

    //Different reading if HasCardID = 0 or 1.
    if (HasCardID == 1) {               // Inputtype: XX:YY->ZZ
        toColType = input[7];
        toColNr = (int)(input[8]-'0');
    } else if (HasCardID == 0) {        //Inputtype: XX->ZZ
        toColType = input[4];
        toColNr = (int)(input[5]-'0');
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

void LogNewMove(const char* move, int flipped) {
    char entry[32] = "";

    // Step 1: set '1' or '0' before the string
    entry[0] = flipped ? '1' : '0';

    // Step 2: Copy the rest of the move into entry
    strncpy(entry + 1, move, sizeof(entry) - 2);  // -2: one for 1 or 2, one for '\0'
    entry[sizeof(entry) - 1] = '\0';

    // Step 3: Store the entry in the move log
    if (moveCount < MAX_MOVES) {
        strncpy(moveLog[moveCount], entry, sizeof(moveLog[moveCount]) - 1);
        moveLog[moveCount][sizeof(moveLog[moveCount]) - 1] = '\0';
        moveCount++;
        currentMove = moveCount;  // Clear redo history after this
    }
}

void UndoMove() {
    if (currentMove == 0) {
        printf("Nothing to undo.\n");
        return;
    }

    char* move = moveLog[--currentMove];  // Move the cursor back to the previous move .
    int flipped = move[0] == '1' ;               // Store the first character
    char lastMove[32];

    strcpy(lastMove, move + 1);    // Save without the prepend

    char reversedMove[20];

    // Reverse the move
    if (lastMove[2] == ':') {
        // Format: C3:QH->C7 → invert to C7:QH->C3
        snprintf(reversedMove, sizeof(reversedMove), "%c%c:%c%c->%c%c",
            lastMove[7], lastMove[8],  // dest
            lastMove[3], lastMove[4],  // card ID
            lastMove[0], lastMove[1]   // src (note: shifted by 1 due to flipped char)
        );
    } else if (lastMove[2] == '-') {
        // Format: C1->C7 → invert to C7->C1
        snprintf(reversedMove, sizeof(reversedMove), "%c%c->%c%c",
            lastMove[4], lastMove[5],  // dest
            lastMove[0], lastMove[1]   // src
        );
    } else {
        printf("Cannot undo unknown move format: %s\n", lastMove);
        return;
    }

    Card* from = NULL;
    Card* to = NULL;
    char cardID[3] = { lastMove[3], lastMove[4], '\0' };

    if (flipped) {
        ExtractColumnsFromInput(lastMove, &from, &to, cardID);
        from->prev->faceUp = 0;
    }

    int useCardID = (reversedMove[2] == ':') ? 1 : 0;
    ExtractColumnsFromInput(reversedMove, &from, &to, useCardID);

    int count = 1;
    if (useCardID) {
        IsCardInSourceColumn(from, cardID, &count);
    }

    MoveTopCards(&from, &to, count);


}


void RedoMove() {
    if (currentMove >= moveCount) {
        printf("Nothing to redo.\n");
        return;
    }

    char* move = moveLog[currentMove++];
    int flipped = move[0] == '1' ;
    char lastMove[20];

    strcpy(lastMove, move + 1);

    Card* from = NULL;
    Card* to = NULL;
    int moveType = (lastMove[2] == ':') ? 1 : 0;
    ExtractColumnsFromInput(lastMove, &from, &to, moveType);

    int count = 1;
    if (moveType) {
        char cardID[3] = { lastMove[3], lastMove[4], '\0' };
        IsCardInSourceColumn(from, cardID, &count);
    }

    MoveTopCards(&from, &to, count);

    if (flipped) {
        from->prev->faceUp = 1;
    }
}

