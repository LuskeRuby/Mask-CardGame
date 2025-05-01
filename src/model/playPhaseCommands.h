
#ifndef PLAYPHASECOMMANDS_H
#define PLAYPHASECOMMANDS_H

#include "model/deck.h"


//Global arrays
extern Card* columnArr[7];
extern Card* foundationArr[4];

//Functions
void InitArray(); //Populate the array of linkedlists
void ExtractColumnsFromInput(char *input, Card** fromArr, Card** toArr, char HasCardID); //Extract data from input and save as variables
int ConvertRank(Card* card);  // always returns a number, så 1 if ace, 13 if king etc.


#endif //PLAYPHASECOMMANDS_H
