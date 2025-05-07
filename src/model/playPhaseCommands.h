
#ifndef PLAYPHASECOMMANDS_H
#include "model/deck.h"
#define PLAYPHASECOMMANDS_H
#define MAX_MOVES 500


// global variables for moveLog
extern char moveLog[MAX_MOVES][15];  // Array to store the move commands
extern int moveCount;                // Number of moves logged
extern int currentMove;              // Index for undo/redo position


//Functions
void InitArray(); //Populate the array of linkedlists with the global variable 'list'
void ExtractColumnsFromInput(char *input, Card** fromArr, Card** toArr, char HasCardID); //Extract data from inputString. For example make the input "C1" point to columnArray[0]. HasCardID is given to 1 if input of type "XX:YY->ZZ" and 0 for "XX->ZZ"
void LogNewMove(const char* move,  const char* flipped); //Log move
void UndoMove(); //Undo
void RedoMove(); //Redo

#endif //PLAYPHASECOMMANDS_H
