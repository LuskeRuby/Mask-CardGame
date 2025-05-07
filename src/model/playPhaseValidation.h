#ifndef PLAYPHASEVALIDATION_H
#define PLAYPHASEVALIDATION_H
#include "model/deck.h"

int IsCardInSourceColumn(Card* fromArr, const char* cardID, int* countToCard); // ensured card is in the column and count to its position
int IsValidMove(Card* moving, Card* target, char fromType, char toType); // Returns 1 if the move is valid according to rules, 0 otherwise.
int ConvertRank(Card* card);  // always returns a number, så 1 if ace, 13 if king etc.


#endif //PLAYPHASEVALIDATION_H

