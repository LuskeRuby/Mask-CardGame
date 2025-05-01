#include "playPhaseValidation.h"

#include <string.h>

#include "playPhaseCommands.h"

//validations

// Validates if a move uses multi-card syntax and if it is legal for columns only
int IsMultiCardMoveToOrFromFoundation(const char* input) {
    return (input[0] == 'F' || input[7] == 'F');
}

// Validates if a card is a valid multi-card source
int IsCardInSourceColumn(Card* fromArr, const char* cardID, int* countToCard) {
    Card* iter = fromArr->prev;
    *countToCard = 0;

    while (strcmp(iter->ID, "00") != 0) {
        (*countToCard)++;
        if (strcmp(iter->ID, cardID) == 0) {
            return 1;
        }
        iter = iter->prev;
    }
    return 0;
}

// Validates column-to-column rules
int IsValidColumnToColumnMove(Card* moving, Card* target) {
    int mRank = ConvertRank(moving);
    int tRank = ConvertRank(target);
    char mSuit = moving->ID[1];
    char tSuit = target->ID[1];

    if (strcmp(target->ID, "00") == 0) {
        return mRank == 13; // Only King
    }
    return (mRank + 1 == tRank) && (mSuit != tSuit);
}

// Validates column-to-foundation move
int IsValidColumnToFoundationMove(Card* moving, Card* target) {
    int mRank = ConvertRank(moving);
    int tRank = ConvertRank(target);
    char mSuit = moving->ID[1];
    char tSuit = target->ID[1];

    if (strcmp(target->ID, "00") == 0) {
        return mRank == 1; // Ace
    }
    return (mSuit == tSuit) && (mRank == tRank + 1);
}

// Validates foundation-to-column move
int IsValidFoundationToColumnMove(Card* moving, Card* target) {
    return IsValidColumnToColumnMove(moving, target);
}

// Ensures the card being moved is top and face-up
int IsTopFaceUpCard(Card* fromArr, Card* card) {
    return card == fromArr->prev && card->faceUp != 0 && strcmp(card->ID, "00") != 0;
}