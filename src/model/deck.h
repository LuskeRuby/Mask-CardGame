#include <stdio.h>
#ifndef CARD_H
#define CARD_H

//Struct of cards
struct cards {
    char ID[3]; //fx d2
    char faceUp;
    struct cards *prev;
    struct cards *next;
};
typedef struct cards Card;

//global var
extern char dummyValue[3];
extern Card *list; //initalize list


//Card Functions
Card* CreateCard(char *ID);
int AddCard(Card *newCard, Card **list);
int DeleteCard(char *cardID, Card **list);
void MoveTopCards(Card** fromPile, Card** toPile, int amount);
int SizeOfDeck(Card** deck);

#endif //CARD_H
