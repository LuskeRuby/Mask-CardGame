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

    entry[0] = flipped ? '1' : '0';
    strncpy(entry + 1, move, sizeof(entry) - 2);
    entry[sizeof(entry) - 1] = '\0';

    if (moveCount < MAX_MOVES) {
        strncpy(moveLog[moveCount], entry, sizeof(moveLog[moveCount]) - 1);
        moveLog[moveCount][sizeof(moveLog[moveCount]) - 1] = '\0';  //log it

        moveCount++;
        currentMove = moveCount;
    }
}


void UndoMove() {
    if (currentMove <= 0) {
        printf("Nothing to undo.\n");
        return;
    }

    // Step 1: Get the move entry and save the flip status
    char* fullEntry = moveLog[--currentMove];
    int flipped = fullEntry[0] == '1';

    // Step 2: Remove the first character (flip flag)
    char cleanedMove[31];
    strncpy(cleanedMove, fullEntry + 1, sizeof(cleanedMove) - 1);
    cleanedMove[sizeof(cleanedMove) - 1] = '\0';

    // Step 3: Extract from the original move and reverse on that if i need to flip.
    Card* from = NULL;
    Card* to = NULL;

    if (flipped) {
        ExtractColumnsFromInput(cleanedMove, &from, &to, (cleanedMove[2] == ':') ? 1 : 0);
        from->prev->faceUp = 0;
    }

    // Step 4: Parse and reverse the move
    char reversedMove[20] = "";
    if (cleanedMove[2] == ':') {
        // Format: C3:QH->C7 → invert to C7:QH->C3
        snprintf(reversedMove, sizeof(reversedMove), "%c%c:%c%c->%c%c",
            cleanedMove[7], cleanedMove[8],  // dest
            cleanedMove[3], cleanedMove[4],  // card ID
            cleanedMove[0], cleanedMove[1]   // src
        );
    } else if (cleanedMove[2] == '-') {
        // Format: C1->C7 → invert to C7->C1
        snprintf(reversedMove, sizeof(reversedMove), "%c%c->%c%c",
            cleanedMove[4], cleanedMove[5],  // dest
            cleanedMove[0], cleanedMove[1]   // src
        );
    } else {
        printf("Cannot undo unknown move format: %s\n", cleanedMove);
        return;
    }

    // Step 5: Extract source/destination columns from reversed move
    ExtractColumnsFromInput(reversedMove, &from, &to, (reversedMove[2] == ':') ? 1 : 0);

    // Step 6: Determine how many cards to move
    char moveCardID[3] = "";
    int count = 0;
    if (reversedMove[2] == ':') {
        strncpy(moveCardID, &reversedMove[3], 2);
        moveCardID[2] = '\0';
        IsCardInSourceColumn(from, moveCardID, &count);
    } else {
        count = 1;
    }

    // Step 7: Execute the move
    MoveTopCards(&from, &to, count);
}


// Function to redo the last undone move
void RedoMove() {
    if (currentMove == moveCount) {
        printf("Nothing to redo.\n");
        return;
    }

    char* fullEntry = moveLog[currentMove++];
    int flipped = fullEntry[0] == '1';

    //remove the 1 or 0 at the start
    char move[31];
    strncpy(move, fullEntry + 1, sizeof(move) - 1);
    move[sizeof(move) - 1] = '\0';


    Card* from = NULL;
    Card* to = NULL;
    ExtractColumnsFromInput(move, &from, &to, (move[2] == ':') ? 1 : 0);

    int count = 0;
    if (move[2] == ':') {
        char cardID[3] = { move[3], move[4], '\0' };
        IsCardInSourceColumn(from, cardID, &count);
    } else {
        count = 1;
    }

    MoveTopCards(&from, &to, count);

    if (flipped != 0) {
        from->prev->faceUp = 1;
    }
}



