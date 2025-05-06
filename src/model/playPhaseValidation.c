#include "playPhaseValidation.h"

#include <string.h>

#include "deck.h"

int ConvertRank(Card* card) {
    char rankChar = card->ID[0];
    if (rankChar >= '2' && rankChar <= '9') {
        return rankChar - '0'; }
    if (rankChar == 'A') { return 1;  }
    if (rankChar == 'T') { return 10; }
    if (rankChar == 'J') { return 11; }
    if (rankChar == 'Q') { return 12; }
    if (rankChar == 'K') { return 13; }

    return -1; // Error
}

// ensured card is in the column and count to its position
int IsCardInSourceColumn(Card* fromArr, const char* cardID, int* countToCard) {
    *countToCard = 0;
    for (Card* curr = fromArr->prev; strcmp(curr->ID, "00") != 0; curr = curr->prev) {
        (*countToCard)++;
        if (strcmp(curr->ID, cardID) == 0) {
            return 1;
        }
    }
    return 0;
}



// Returns 1 if the move is valid according to rules, 0 otherwise.
int IsValidMove(Card* moving, Card* target, char fromType, char toType) {
    int mRank = ConvertRank(moving);
    int tRank = ConvertRank(target);
    char mSuit = moving->ID[1];
    char tSuit = target->ID[1];

    // moving to -> column
    if ((fromType == 'C' || fromType == 'F') && toType == 'C') {
        if (strcmp(target->ID, "00") == 0) {
            return mRank == 13; // Only King can go to empty column
        }
        return (mRank + 1 == tRank) && (mSuit != tSuit); // One rank lower, different suit
    }

    // moving to → Foundation
    if (fromType == 'C' && toType == 'F') {
        if (strcmp(target->ID, "00") == 0) {
            return mRank == 1; // Only Ace can go to empty foundation
        }
        return (mSuit == tSuit) && (mRank == tRank + 1); // Same suit, one rank higher
    }

    // Other moves are invalid (e.g., F → F not allowed)
    return 0;
}

