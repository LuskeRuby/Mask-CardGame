//
// Created by rubym on 01/05/2025.
//

#ifndef PLAYPHASEVALIDATION_H
#define PLAYPHASEVALIDATION_H
#include "deck.h"

int IsCardInSourceColumn(Card* fromArr, const char* cardID, int* countToCard); // ensured card is in the column and count to its position
int IsValidMove(Card* moving, Card* target, char fromType, char toType);
int ConvertRank(Card* card);  // always returns a number, så 1 if ace, 13 if king etc.


#endif //PLAYPHASEVALIDATION_H

