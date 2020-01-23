#ifndef CARDGAME_H_
#define CARDGAME_H_

//cards have int suit to describe suit, and int number to describe number
//0-3 represents spade/club/diamond/heart
//0-12 represents cards 3/4/5/6/7/8/9/T/J/Q/K/A/2
typedef struct _card card;

/*************************************************************
   Deal : Creates deck, sends it to be shuffled, and deals out 
          4 hands. 
   Returns : 0 for success, -1 if null pointers.
 *************************************************************/
int deal(card * handOne, card * handTwo, card * handThree, card * handFour);

/*************************************************************
   Shuffle : randomly places cards from original deck into new 
             deck, then copies new deck over old deck.
   Returns : 0 for success, -1 for NULL pointers
 *************************************************************/
int shuffle(card * theDeck);

/*************************************************************
   PrintDeck : prints the cards of the deck'
   Returns : 0 for success, -1 for NULL pointers
 *************************************************************/
int printdeck(card * theDeck);

/*************************************************************
   PrintWinner : prints number of winner w/ congratulations
   Returns : 0 for success, -1 for NULL pointers
 *************************************************************/
void printWinner(int winner);

/*************************************************************
   Gameplay : holds loop of the game
   Returns : 1/2/3/4 at end of game, depending on winner. 
             -1 for NULL pointers
 *************************************************************/
int gameplay();

/*************************************************************
   Turn : holds the player's turn / getting user input
   Returns : -1 for NULL pointers, 0 for success
 *************************************************************/
int turn(card * theHand, int * hasPassed, card * play1, card * play2, int reset, int handNum);

/*************************************************************
   GetUserInput : gets userinput and checks that it is valid
   Returns : 0 for success, 1 for invalid input, -1 for NULL pointers
 *************************************************************/
int getUserInput(card * play2, int * hasPassed);

/*************************************************************
   Sorthand : sorts each hand in order of number/suit
   Returns : 0 for success, -1 for NULL pointers
 *************************************************************/
int sorthand(card * theHand);

/*************************************************************
   InHand : checks if given cards (theMove) are in a given hand (theHand)
   Returns : 1 if cards in hand, 0 if cards !inhand, -1 for NULL pointers
 *************************************************************/
int inHand(card * theMove, card * theHand);

/*************************************************************
   RemoveOldCards : Takes successfully played cards out of the hand
   Returns : 0 for success, -1 for NULL pointers
 *************************************************************/
int removeOldCards(card * theHand, card * thePlay);

/*************************************************************
   Playcmp : Checks if played cards can beat the previous play / are valid
   Returns : 1 if valid play, 0 if invalid play, -1 for NULL pointers
 *************************************************************/
int playcmp(card * const prevPlay, card * const currPlay, int counter, int * hasPassed, int reset);

/*************************************************************
   ChanePrevPlay : updates the previous play for printing to the next player
   Returns : 0 for success, -1 for NULL pointers
 *************************************************************/
int changePrevPlay(card * play1, card * play2);

/*************************************************************
   TypeOfPlay : finds out what type of play the cards are. (Ex: straight, single, double, etc..)
   Returns : 0 for invalid cards, 1 for single, 2 for double, 
             3 for triple, 4 for 3 card run, 5 for breaker, 
             6 for 4 card run, 7 for 5 card run, 8 for 6 card run,
             9 for 7 card run, 10 for 8 card run, 11 for 9 card run
             12 for 10 card run, 13 for 11 card run, 14 for 12 card run
             15 for 13 card run, and -1 for NULL pointers
 *************************************************************/
int typeOfPlay(card * theCards);


/*************************************************************
   ValidStraight : checks if play is in numbered order 
   Returns : 1 if true, 0 if false, -1 for NULL pointers
 *************************************************************/
int validStraight(card * theCards, int numOfCards);

/*************************************************************
   CardsInHand : finds the number of cards in a hand
   Returns : number of cards in hand or -1 for NULL pointers
 *************************************************************/
int cardsInHand(card * theHand);

#endif
