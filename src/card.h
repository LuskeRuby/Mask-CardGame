
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


//Card Functions
Card* CreateCard(char *ID);

int AddCard(Card *newCard, Card **list);

int DeleteCard(char *cardID, Card **list);

void PrintDeck(Card* list);

int PrintInitialDeck(int SW, char lastCommand[3], char* msg);

int LDValidation(Card* loadedDeck);


//----remove card

#endif //CARD_H
