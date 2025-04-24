#include <stdio.h>
#ifndef CARD_H
#define CARD_H
extern char dummyValue[3];

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
extern Card* dummy; //Dummyvalue globalvariable from deck.h
extern Card *list; //initalize list


//Card Functions
Card* CreateCard(char *ID);
int AddCard(Card *newCard, Card **list);
int DeleteCard(char *cardID, Card **list);
void MoveTopCards(Card** fromPile, Card** toPile, int amount);
int SizeOfDeck(Card** deck);

//----remove card

#endif //CARD_H
