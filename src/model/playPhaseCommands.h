
#ifndef PLAYPHASECOMMANDS_H
#include "model/deck.h"
#define PLAYPHASECOMMANDS_H
#define MAX_MOVES 500
#include "model/deck.h"


// global values for my moveLog
extern char moveLog[MAX_MOVES][32];  // Array to store the move commands
extern int moveCount;                // Number of moves logged
extern int currentMove;              // Index for undo/redo position


//Functions
void InitArray(); //Populate the array of linkedlists
void ExtractColumnsFromInput(char *input, Card** fromArr, Card** toArr, char HasCardID); //Extract data from input and save as variables

void LogNewMove(const char* move,  int flipped);
void UndoMove();
void RedoMove();

#endif //PLAYPHASECOMMANDS_H
