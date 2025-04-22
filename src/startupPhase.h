#ifndef STARTUPPHASE_H
#define STARTUPPHASE_H
#include <stdio.h>

#include "card.h"

//global var
extern char dummyValue[3];
extern Card* dummy; //Dummyvalue globalvariable from card.h
extern Card *list; //initalize list

//Startupphase Functions
char* LD(); //Filename not provided
Card* BuildLinkedListFromFile(FILE* stream);
Card* LoadDefaultDeck();
void RunStartupPhase();
void RunPlayPhase(void);

//int LD(char Filename); //Provided filename



#endif //STARTUPPHASE_H
