//
// Created by rubym on 24/04/2025.
//

#include "deck.h"
#ifndef STARTPHASECOMMANDS_H
#define STARTPHASECOMMANDS_H

Card* BuildLinkedListFromFile(FILE* stream);
Card* LoadDefaultDeck();
char* LD(); //Filename not provided
char* LD(char* filename);
int LDValidation(Card* Deck);
char* SD(char* filename);
void SI(int split);
void SR();
void SW();


#endif //STARTPHASECOMMANDS_H
