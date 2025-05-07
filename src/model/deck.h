#ifndef DECK_H
#define DECK_H

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

extern Card* columnArr[7];  // Declaring external reference for columnArr
extern Card* foundationArr[4];  // Declaring external reference for foundationArr


//Card Functions
Card* CreateCard(char *ID);
int AddCard(Card *newCard, Card **list); //Add card parametr to given list parameter
int DeleteCard(char *cardID, Card **list); //Delete card matching cardID parameter from list, and free it from memory
void MoveTopCards(Card** fromPile, Card** toPile, int amount); //Give the dummy value of frompile and toPile, to move 'amount' of topcards from 'fromPile' to 'toPile'
int SizeOfDeck(Card** deck); //Return amount of cards in list/deck

#endif //CARD_H
