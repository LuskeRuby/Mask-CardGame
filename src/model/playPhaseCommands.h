
#ifndef PLAYPHASECOMMANDS_H
#define PLAYPHASECOMMANDS_H

//Global arrays
extern Card* columnArr[7];
extern Card* foundationArr[4];


//Functions
void InitArray(); //Populate the array of linkedlists
void ExtractColumnsFromInput(char *input, Card** fromArr, Card** toArr, char HasCardID); //Extract data from input and save as variables


#endif //PLAYPHASECOMMANDS_H
