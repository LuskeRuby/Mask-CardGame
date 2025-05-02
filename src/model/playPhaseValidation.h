//
// Created by rubym on 01/05/2025.
//

#ifndef PLAYPHASEVALIDATION_H
#define PLAYPHASEVALIDATION_H
#include "deck.h"

int IsMultiCardMoveToOrFromFoundation(const char* input);
int IsCardInSourceColumn(Card* fromArr, const char* cardID, int* countToCard);
int IsValidColumnToFoundationMove(Card* moving, Card* target);
int IsValidColumnToColumnMove(Card* moving, Card* target);
int IsValidFoundationToColumnMove(Card* moving, Card* target);
int IsTopFaceUpCard(Card* fromArr, Card* card);
int ConvertRank(Card* card);  // always returns a number, så 1 if ace, 13 if king etc.


#endif //PLAYPHASEVALIDATION_H

