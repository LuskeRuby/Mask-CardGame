//
// Created by rubym on 01/05/2025.
//

#ifndef PLAYPHASEVALIDATION_H
#define PLAYPHASEVALIDATION_H
#include "deck.h"

int ConvertRank(Card* card);
int IsCardInSourceColumn(Card* fromArr, const char* cardID, int* countToCard);
int IsValidMoveBetweenColumns(Card* moving, Card* target);
int IsValidMoveToFoundationFromColumn(Card* moving, Card* target);
int IsTopFaceUpCard(Card* fromArr, Card* card);
int ConvertRank(Card* card);  // always returns a number, så 1 if ace, 13 if king etc.


#endif //PLAYPHASEVALIDATION_H

