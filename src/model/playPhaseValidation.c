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


// Validates move between two columns
int IsValidMoveBetweenColumns(Card* moving, Card* target) {
    int mRank = ConvertRank(moving);
    int tRank = ConvertRank(target);
    char mSuit = moving->ID[1];
    char tSuit = target->ID[1];

    if (strcmp(target->ID, "00") == 0) {
        return mRank == 13; // Only King can go to an empty column
    }
    return (mRank + 1 == tRank) && (mSuit != tSuit); // Must be one rank lower, and different suit
}

// Validates move from column to foundation
int IsValidMoveToFoundationFromColumn(Card* moving, Card* target) {
    int mRank = ConvertRank(moving);
    int tRank = ConvertRank(target);
    char mSuit = moving->ID[1];
    char tSuit = target->ID[1];

    if (strcmp(target->ID, "00") == 0) {
        return mRank == 1; // Ace must be placed on an empty foundation
    }
    return (mSuit == tSuit) && (mRank == tRank + 1); // Same suit, and one rank higher
}

// Ensures the card being moved is top and face-up
int IsTopFaceUpCard(Card* fromArr, Card* card) {
    return card == fromArr->prev && card->faceUp != 0 && strcmp(card->ID, "00") != 0;
}
