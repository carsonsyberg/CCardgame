#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "cardgame.h"

struct _card{
  int suit; //integer from 0 to 3
  int number; //integer from 0 to 12
};

/*************************************************************
   Deal : Creates deck, sends it to be shuffled, and deals out 
          4 hands. 
   Returns : 0 for success
 *************************************************************/
int deal(card * handOne, card * handTwo, card * handThree, card * handFour) {
  
  card theDeck[52];
  
  int s = 0;
  int n = 0;
  //fills the deck in orderly fashion
  for(int i = 0; i < 52; i++) {
    theDeck[i].suit = s;
    theDeck[i].number = n;
    n++;
    //changes to next suit after filling first 12 card slots
    if(n > 12) {
      n = 0;
      s++;
    }
  }

  shuffle(theDeck);

  int p = 0;
  //deals the now shuffled deck ensuring random hands
  for(int i = 0; i < 52; i += 4) {
    handOne[p] = theDeck[i];
    handTwo[p] = theDeck[i+1];
    handThree[p] = theDeck[i+2];
    handFour[p] = theDeck[i+3];
    p++;
  }
    
  return 0;
}

/*************************************************************
   Shuffle : randomly places cards from original deck into new 
             deck, then copies new deck over old deck.
   Returns : 0 for success, -1 for NULL pointers
 *************************************************************/
int shuffle(card * theDeck) {

  if(!theDeck) {
    printf("\n NULL POINTER \n");
    return -1;
  }
  
  card shuffledDeck[52]; //empty array to store the shuffle

  //sets all suits of shuffle cards 5 to begin with so we can tell which have been filled
  for(int s = 0; s < 52; s++) { 
    shuffledDeck[s].suit = 5;
  }

  //goes through all cards, using random number generator, to find one with a 5 for a suit value
  //and then replace that cards values with the next value from the original deck
  //if it does not find a 5 suit it makes a new random number and tries again til no more 5 suits
  //exist.
  int z = 0;
  for(int s = 0; s < 52; s++) {
    int randNum;
    while(1){ //random num loop that stays with one original card its whole run
      randNum = rand() % 52;
      //if random number finds empty card -> fill it
      if(shuffledDeck[randNum].suit == 5) {
	shuffledDeck[randNum].suit = theDeck[z].suit;
	shuffledDeck[randNum].number = theDeck[z].number;
	z = z + 1;
	break; //goes to next card with successful assignment
      }
    }
  }

  //copies the shuffled deck back over the original deck to be used in deal function
  for(int i = 0; i < 52; i++) {
    theDeck[i].number = shuffledDeck[i].number;
    theDeck[i].suit = shuffledDeck[i].suit;
  }

  return 0;

}

/*************************************************************
   PrintDeck : prints the cards of the deck'
   Returns : 0 for success, -1 for NULL pointers
 *************************************************************/
int printdeck(card * theDeck) {

  if(!theDeck) {
    printf("\n NULL POINTER \n");
    return -1;
  }

  //takes the 0 - 13 ints and translates them to 3 - 10 / J - 2 for printing
  for(int i = 0; i < 13; i++) {
    if(theDeck[i].number < 7)
      printf(" %d", theDeck[i].number + 3);
    else if(theDeck[i].number == 7)
      printf(" %c", 'T');
    else if(theDeck[i].number == 8)
      printf(" %c", 'J');
    else if(theDeck[i].number == 9)
      printf(" %c", 'Q');
    else if(theDeck[i].number == 10)
      printf(" %c", 'K');
    else if(theDeck[i].number == 11)
      printf(" %c", 'A');
    else if(theDeck[i].number == 12)
      printf(" %d", 2);
    else
      printf(" %c", '_');

    //takes the 0 - 3 ints and translates them to S C D H for printing
    if(theDeck[i].suit == 0)
      printf("%c ", 'S');
    else if(theDeck[i].suit == 1)
      printf("%c ", 'C');
    else if(theDeck[i].suit == 2)
      printf("%c ", 'D');
    else if(theDeck[i].suit == 3)
      printf("%c ", 'H');
    else
      printf("%c ", '_');
  }

  return 0;
}

/*************************************************************
   PrintWinner : prints number of winner w/ congratulations
   Returns : 0 for success, -1 for NULL pointers
 *************************************************************/
void printWinner(int winner) {
  //prints a bunch of new lines for formatting
  for(int i = 0; i < 20; i++)
    printf("\n");
  printf("PLAYER %d WINS!!!", winner);
  //prints a bunch of new lines for formatting
  for(int i = 0; i < 10; i++)
    printf("\n");
}

/*************************************************************
   Gameplay : holds loop of the game
   Returns : 1/2/3/4 at end of game, depending on winner. 
             -1 for NULL pointers
 *************************************************************/
int gameplay() {

  //the four player hands to hold their cards
  card handOne[13];
  card handTwo[13];
  card handThree[13];
  card handFour[13];

  //deals
  int err = deal(handOne, handTwo, handThree, handFour);
  if(err != 0) {
    printf("Error dealing!\n");
    return -1;
  }
  
  //sorts hands
  sorthand(handOne);
  sorthand(handTwo);
  sorthand(handThree);
  sorthand(handFour);

  int hasPassed1 = 0;
  int hasPassed2 = 0;
  int hasPassed3 = 0;
  int hasPassed4 = 0;
  int reset = 0;
  int counter = 0;
  int validPlay;

  //variables for deciding who starts / has the 3 of spades
  int start1 = 0;
  int start2 = 0;
  int start3 = 0;
  int start4 = 0;
  
  //these card arrays hold the previous play and the current play
  card thePlay1[13];
  card thePlay2[13];

  //sets the first previous play to empty cards
  for(int i = 0; i < 13; i++) {
    thePlay1[i].number = 15;
    thePlay1[i].suit = 5;
  }

  //decides who has the 3 of spades, and assigns them to start the game
  if(handOne[0].number == 0 && handOne[0].suit == 0) {
    start1 = 1;
  }
  if(handTwo[0].number == 0 && handTwo[0].suit == 0) {
    start2 = 1;
  }
  if(handThree[0].number == 0 && handThree[0].suit == 0) {
    start3 = 1;
  }
  if(handFour[0].number == 0 && handFour[0].suit == 0) {
    start4 = 1;
  }
  
  //start of the loop of the game
  while(1) {

    //checks if other players have passed
    if(!hasPassed1 && hasPassed2 && hasPassed3 && hasPassed4) {
      reset = 1;
      hasPassed2 = 0;
      hasPassed3 = 0;
      hasPassed4 = 0;
    }
    if(!hasPassed1 && start1) {
      //starts player's turn
      turn(handOne, &hasPassed1, thePlay1, thePlay2,  reset, 1);
      
      //Checks if cards beat previous play / are valid -> if not they have to do their turn again
      validPlay = playcmp(thePlay1, thePlay2, counter, &hasPassed1, reset);
      while(!validPlay) {
	printf("Sorry, play invalid. Try again.\n");
	turn(handOne, &hasPassed1, thePlay1, thePlay2,  reset, 1);
	validPlay = playcmp(thePlay1, thePlay2, counter, &hasPassed1, reset);
      }
    }
    //after play, reset goes back to false
    reset = 0;
    //checks if cards played are valid and that player hasn't passed
    //wont need this if statement once above portion is fixed, since user input will be guarunteed to be correct
    if(validPlay && !hasPassed1 && start1) {
      //updates the previous play to what has been validly played
      changePrevPlay(thePlay1, thePlay2);
      //takes cards played out of the player's deck
      removeOldCards(handOne, thePlay1);
      start1 = 1;
      start2 = 1;
      start3 = 1;
      start4 = 1;
      counter = 1;
    }
    //checks if player 1 has no more cards -> if true, they win
    if(!cardsInHand(handOne))
      return 1;
    
    //all other turns (players 2 - 4) are identical to player 1's
    if(!hasPassed2 && hasPassed1 && hasPassed3 && hasPassed4) {
      reset = 1;
      hasPassed1 = 0;
      hasPassed3 = 0;
      hasPassed4 = 0;
    }
    if(!hasPassed2 && start2) {
      turn(handTwo, &hasPassed2, thePlay1, thePlay2,  reset, 2);
      //Under Construction
      validPlay = playcmp(thePlay1, thePlay2, counter, &hasPassed2, reset);
      while(!validPlay) {
        printf("Sorry, play invalid. Try again.\n");
	turn(handTwo, &hasPassed2, thePlay1, thePlay2,  reset, 2);
	validPlay = playcmp(thePlay1, thePlay2, counter, &hasPassed2, reset);
      }
    }
    reset = 0;
    if(validPlay && !hasPassed2 && start2) {
      changePrevPlay(thePlay1, thePlay2);
      removeOldCards(handTwo, thePlay1);
      start1 = 1;
      start2 = 1;
      start3 = 1;
      start4 = 1;
      counter = 1;
    }
    //checks if player 2 has no more cards -> if true, they win
    if(!cardsInHand(handTwo))
      return 2;
    
    if(!hasPassed3 && hasPassed2 && hasPassed1 && hasPassed4) {
      reset = 1;
      hasPassed1 = 0;
      hasPassed2 = 0;
      hasPassed4 = 0;
    }
    if(!hasPassed3 && start3) {
      turn(handThree, &hasPassed3, thePlay1, thePlay2,  reset, 3);
      //UNDER CONSTRUCTION
      validPlay = playcmp(thePlay1, thePlay2, counter, &hasPassed3, reset);
      while(!validPlay) {
        printf("Sorry, play invalid. Try again.\n");
	turn(handThree, &hasPassed3, thePlay1, thePlay2,  reset, 3);
	validPlay = playcmp(thePlay1, thePlay2, counter, &hasPassed3, reset);
      }
    }
    reset = 0;
    if(validPlay && !hasPassed3 && start3) {
      changePrevPlay(thePlay1, thePlay2);
      removeOldCards(handThree, thePlay1);
      start1 = 1;
      start2 = 1;
      start3 = 1;
      start4 = 1;
      counter = 1;
    }
    //checks if player 3 has no more cards -> if true, they win
    if(!cardsInHand(handThree))
      return 3;
    
    if(!hasPassed4 && hasPassed2 && hasPassed3 && hasPassed1) {
      reset = 1;
      hasPassed1 = 0;
      hasPassed2 = 0;
      hasPassed3 = 0;
    }
    if(!hasPassed4 && start4) {
      turn(handFour, &hasPassed4, thePlay1, thePlay2,  reset, 4);
      //Under construction
      validPlay = playcmp(thePlay1, thePlay2, counter, &hasPassed4, reset);
      while(!validPlay) {
        printf("Sorry, play invalid. Try again.\n");
	turn(handFour, &hasPassed4, thePlay1, thePlay2,  reset, 4);
	validPlay = playcmp(thePlay1, thePlay2, counter, &hasPassed4, reset);
      }
    }
    reset = 0;
    if(validPlay && !hasPassed4 && start4) {
      changePrevPlay(thePlay1, thePlay2);
      removeOldCards(handFour, thePlay1);
      start1 = 1;
      start2 = 1;
      start3 = 1;
      start4 = 1;
      counter = 1;
    }
    //checks if player 4 has no more cards -> if true, they win
    if(!cardsInHand(handFour))
      return 4;
  }
}

/*************************************************************
   Turn : holds the player's turn / getting user input
   Returns : -1 for NULL pointers, 0 for success
 *************************************************************/
int turn(card * theHand, int * hasPassed, card * play1, card * play2, int reset, int handNum) {

  if(!theHand || !hasPassed || !play1 || !play2) {
    printf("\n NULL POINTER \n");
    return -1;
  }
  
  //checks if player gets to reset, if not it prints the previous play
  if(!reset) {
    //prints a bunch of new lines for formatting
    for(int i = 0; i < 20; i++)
      printf("\n");
    printf("\nPrevious Play :\n");
    printdeck(play1);
  }
  else {
    //prints a bunch of new lines for formatting
    for(int i = 0; i < 20; i++)
      printf("\n");
    printf("\nReset! Play any valid combination of cards you want.\n");
  }

  printf("\n\nPlayer %d's Hand :\n", handNum);
  printdeck(theHand);
  printf("\n");
  //prints new lines for formatting
  for(int i = 0; i < 10; i++)
    printf("\n");

  //gets user input and if player doesn't have cards gets it again til they type correctly
  getUserInput(play2, hasPassed);
  while(inHand(play2, theHand)) {
    for(int i = 0; i < 10; i++)
      printf("\n");
    printf("\nYou don't have those cards, try again.\n");
    printf("\nPrevious Play :\n");
    printdeck(play1);
    printf("\n\nPlayer %d's Hand :\n", handNum);
    printdeck(theHand);
    for(int i = 0; i < 10; i++)
      printf("\n");
    getUserInput(play2, hasPassed);
  }

  return 0;
}

/*************************************************************
   GetUserInput : gets userinput and checks that it is valid
   Returns : 0 for success, 1 for invalid input, -1 for NULL pointers
 *************************************************************/
int getUserInput(card * play2, int * hasPassed) {

  if(!play2 || !hasPassed) {
    printf("\n NULL POINTER \n");
    return -1;
  }

  //empties array to hold user input
  for(int i = 0; i < 13; i++) {
    play2[i].number = 15;
    play2[i].suit = 5;
  }

  int r = 0;
  
  //50 is max amount of characters possible in a move
  char userInput[50];
  while(scanf("%c", &userInput[r]) != EOF) {
    r++;
  }

  //checks if player wants to pass
  if(userInput[0] == 'p' && userInput[1] == 'a' && userInput[2] == 's' && userInput[3] == 's') {
    *hasPassed = 1;
    return 0;
  }

  //translates userinput of 3-10 / J-2 to 0-13
  int z = 0;
  int counter = 0;
  for(int i = 0; i < r; i = i+3) {
    counter++;
    if(userInput[i] == '3') {
      play2[z].number = 0;
      z = z + 1;
    }
    else if(userInput[i] == '4') {
      play2[z].number = 1;
      z = z + 1;
    }
    else if(userInput[i] == '5') {
      play2[z].number = 2;
      z = z + 1;
    }
    else if(userInput[i] == '6') {
      play2[z].number = 3;
      z = z + 1;
    }
    else if(userInput[i] == '7') {
      play2[z].number = 4;
      z = z + 1;
    }
    else if(userInput[i] == '8') {
      play2[z].number = 5;
      z = z + 1;
    }
    else if(userInput[i] == '9') {
      play2[z].number = 6;
      z = z + 1;
    }
    else if(userInput[i] == 'T') {
      play2[z].number = 7;
      z = z + 1;
    }
    else if(userInput[i] == 'J') {
      play2[z].number = 8;
      z = z + 1;
    }
    else if(userInput[i] == 'Q') {
      play2[z].number = 9;
      z = z + 1;
    }
    else if(userInput[i] == 'K') {
      play2[z].number = 10;
      z = z + 1;
    }
    else if(userInput[i] == 'A') {
      play2[z].number = 11;
      z = z + 1;
    }
    else if(userInput[i] == '2') {
	play2[z].number = 12;
	z = z + 1;
    }
    else {
      printf("\nInvalid input, try again.\n");
      getUserInput(play2, hasPassed);
      return 1;
    }

  }

  //if they did not type anything this will be true and they pass their turn
  if(counter == 0) {
    *hasPassed = 1;
  }
  
  //translates user input of S C D H to 0-3
  z = 0;
  for(int i = 1; i < r; i = i+3) {
    if(userInput[i] == 'S') {
      play2[z].suit = 0;
      z = z + 1;
    }
    else if(userInput[i] == 'C') {
      play2[z].suit = 1;
      z = z + 1;
    }
    else if(userInput[i] == 'D') {
      play2[z].suit = 2;
      z = z + 1;
    }
    else if(userInput[i] == 'H') {
      play2[z].suit = 3;
      z = z + 1;
    }
    else {
      printf("\nInvalid Input. Try again.\n");
      getUserInput(play2, hasPassed);
      return 1;
    }
  }
  
  return 0;
}

/*************************************************************
   Sorthand : sorts each hand in order of number/suit
   Returns : 0 for success, -1 for NULL pointers
 *************************************************************/
int sorthand(card * theHand) {

  if(!theHand) {
    printf("\n NULL POINTER \n");
    return -1;
  }

  //acts as a holder to store the sorted order
  card sortedHand[13];
  
  card smallest;
  int smallestI = 0;
  smallest.number = theHand[0].number;
  smallest.suit = theHand[0].suit;
  //similar to other sort functions, sets 1st smallest card to the first in the deck
  
  //this loops through every card in the deck to find its place in the sorted order
  for(int j = 0; j < 13; j++) {
    
    //this loops through every card trying to find a card smaller than smallest
    for(int i = 0; i < 13; i++) {
      //checks if smallest card number == another cards number
      if(theHand[i].number == smallest.number) {
	//checks if smallest card suit > another cards suit
	if(theHand[i].suit < smallest.suit) {
	  //if the card we check against is smaller than the smallest -> it becomes new smallest card
	  smallest.number = theHand[i].number;
	  smallest.suit = theHand[i].suit;
	  smallestI = i;
	}
      }
      //checks if smallest card number > another cards number
      if(theHand[i].number < smallest.number) {
	//if the card we check against is smaller than the smallest -> it becomes new smallest card
	smallest.number = theHand[i].number;
	smallest.suit = theHand[i].suit;
	smallestI = i;
      }
    }
    
    //after finding card smaller than any others
    
    theHand[smallestI].number = 15; //takes this card out of the search for smallest
    theHand[smallestI].suit = 5;    //5 for suit and 15 for num are the default empty cards

    //places smallest card into next open slot of sortedhand
    sortedHand[j] = smallest;

    //resets smallest num/suit to 15/5 so that it is guarunteed to find a card lower than it
    //and allows the previous smallest card to not be chosen
    smallest.number = 15;
    smallest.suit = 5;

  }

  //rewrites the sortedhand back into the original hand
  for(int i = 0; i < 13; i++) {
    theHand[i].number = sortedHand[i].number;
    theHand[i].suit = sortedHand[i].suit;
  }

  return 0;	
}

/*************************************************************
   InHand : checks if given cards (theMove) are in a given hand (theHand)
   Returns : 1 if cards in hand, 0 if cards !inhand, -1 for NULL pointers
 *************************************************************/
int inHand(card * theMove, card * theHand) {

  if(!theHand || !theMove) {
    printf("\n NULL POINTER \n");
    return -1;
  }

  int count = 0;
  int count2 = 0;

  card handHolder[13];
  for(int i = 0; i < 13; i++) {
    handHolder[i].number = theHand[i].number;
    handHolder[i].suit = theHand[i].suit;
  }

  //counts how many cards the user has played
  while(theMove[count].suit != 5) {
    count++;
  }

  //checks the user's hand for the cards they played
  //this loop goes through each card the user typed in
  for(int z = 0; z < count; z++) {
    //this loop goes through each card looking for a match
    for(int i = 0; i < 13; i++) {
      if(theMove[z].number  == handHolder[i].number && theMove[z].suit == handHolder[i].suit) {
	handHolder[i].number = 15;
	handHolder[i].suit = 5;
	count2++;
      }
    }
  }
  
  
  //if these are equal, then there were matches for each card the user played
  if(count2 + 1 == count)
    return 1;
  
  //fails to find move card in deck
  return 0;
    
  
}

/*************************************************************
   RemoveOldCards : Takes successfully played cards out of the hand
   Returns : 0 for success, -1 for NULL pointers
 *************************************************************/
int removeOldCards(card * theHand, card * thePlay) {

  if(!theHand || !thePlay) {
    printf("\n NULL POINTER \n");
    return -1;
  }
  
  int count = 0;
  //counts how many cards the user has played
  while(thePlay[count].suit != 5) {
    count++;
  }
  
  //gets rid of cards that the player has used
  //this loop runs through each card the user played
  for(int z = 0; z < count; z++) {
    //this loop runs through each card the player has and removes matches
    for(int i = 0; i < 13; i++) {
      if(thePlay[z].number == theHand[i].number && thePlay[z].suit == theHand[i].suit) {
	theHand[i].number = 15;
	theHand[i].suit = 5;
      }
    }
  }

  return 0;
}

/*************************************************************
   Playcmp : Checks if played cards can beat the previous play / are valid
   Returns : 1 if valid play, 0 if invalid play, -1 for NULL pointers
 *************************************************************/
int playcmp(card * const prevPlay, card * const currPlay, int counter, int * hasPassed, int reset) {

  if(!prevPlay || !currPlay || !hasPassed) {
    printf("\n NULL POINTER \n");
    return -1;
  }

  int prevType, currType;
  int playSize = 0;
  
  for(int i = 0; i < 13; i++) {
    if(prevPlay[i].suit != 5) {
      playSize++;
    }
  }
  
  if(*hasPassed)
    return 1;
  
  //ensures on first play that the only card played is the 3 of spades
  if(counter == 0) {
    if(!(currPlay[0].number == 0 && currPlay[0].suit == 0)) {
      printf("\nYou can only play the 3S or things added to it to start. Try again.\n");
      return 0;
    }
    if(typeOfPlay(currPlay) == 0)
      return 0; //checks if their cards are invalid, if so returns 0
    else
      return 1; //allows them to play any valid cards on first go
  }
  else if(reset) {
    if(typeOfPlay(currPlay) == 0)
      return 0; //ensures their play is a valid combo of cards
    else
      return 1; //lets them play any valid cards since they have the reset
  }

  prevType = typeOfPlay(prevPlay);
  currType = typeOfPlay(currPlay);
  if(currType == 0)
    return 0; //invalid play

  //special case of a single 2 that can be beaten by a higher 2 or a breaker
  if(prevType == 1 && prevPlay[0].number == 12) {
    printf("\nA 2 HAS BEEN PLAYED\n");
    if(currType == 5)
      return 1; //breaker beats a two
    else if(currType == prevType) {
      if(prevPlay[0].suit < currPlay[0].suit && prevPlay[0].number <= currPlay[0].number)
	return 1; //player had higher 2
      else
	return 0; //player did not have higher 2
    }
    else
      return 0; //player played a card type that was not a single
  }

  if(prevType != currType)
    return 0; //player played a card type that was not the same as the card type before it

  if(currPlay[playSize-1].number > prevPlay[playSize-1].number)
    return 1; //had higher top card number
  else if(currPlay[playSize-1].number == prevPlay[playSize-1].number) {
    if(currPlay[playSize-1].suit > prevPlay[playSize-1].suit)
      return 1; //if top card num was equal and top suit higher, valid play
    else
      return 0; //had same top num, but a lower suit so invalid play
  }
  
  return 0; //if top card was not higher, invalid play
}

/*************************************************************
   ChanePrevPlay : updates the previous play for printing to the next player
   Returns : 0 for success, -1 for NULL pointers
 *************************************************************/
int changePrevPlay(card * play1, card * play2) {
  
  if(!play1 || !play2) {
    printf("\n NULL POINTER \n");
    return -1;
  }
  
  for(int i = 0; i < 13; i++) {
    play1[i].number = play2[i].number;
    play1[i].suit = play2[i].suit;
  }

  return 0;
}

/*************************************************************
   TypeOfPlay : finds out what type of play the cards are. (Ex: straight, single, double, etc..)
   Returns : 0 for invalid cards, 1 for single, 2 for double, 
             3 for triple, 4 for 3 card run, 5 for breaker, 
             6 for 4 card run, 7 for 5 card run, 8 for 6 card run,
             9 for 7 card run, 10 for 8 card run, 11 for 9 card run
             12 for 10 card run, 13 for 11 card run, 14 for 12 card run
             15 for 13 card run, and -1 for NULL pointers
 *************************************************************/
int typeOfPlay(card * theCards) {

  if(!theCards) {
    printf("\n NULL POINTER \n");
    return -1;
  }
  
  int sizeOfPlay = 0;
  //gets sizes of each play
  for(int i = 0; i < 13; i++) {
    if(theCards[i].suit != 5)
      sizeOfPlay++;
  }
  
  if(sizeOfPlay == 1) {
    return 1; //valid single = 1
  }
  else if(sizeOfPlay == 2 && theCards[0].number == theCards[1].number) {
    return 2; //valid double = 2
  }
  else if(sizeOfPlay == 3) {
    if(theCards[0].number == theCards[1].number && theCards[1].number == theCards[2].number) {
      //valid triple = 3
      return 3;
    }
    else if(validStraight(theCards, sizeOfPlay)) {
      //valid 3 card run = 4
      return 4;
    }
    else
      return 0; //invalid cards = 0
  }
  else if(sizeOfPlay == 4) {
    if(theCards[0].number == theCards[1].number && theCards[1].number == theCards[2].number && theCards[2].number == theCards[3].number) {
      printf("BREAKER!!!!");
      return 5; //valid breaker / 4 of a kind = 5
    }
    else if(validStraight(theCards, sizeOfPlay))
      return 6; //valid 4 card run = 6
    else
      return 0; //invalid play if 4 cards and not 4 of a kind / straight
  }
  else if(sizeOfPlay == 5) {
    if(validStraight(theCards, sizeOfPlay))
      return 7; //valid 5 card run = 7
    else
      return 0; //if not valid straight, it's invalid cards
  }
  else if(sizeOfPlay == 6) {
    if(theCards[0].number == theCards[1].number && theCards[2].number == theCards[3].number && theCards[4].number == theCards[5].number) {
      if((theCards[0].number == theCards[2].number - 1) && (theCards[2].number == theCards[4].number - 1)) {
	printf("BREAKER!!!");
	return 5; //valid breaker / triple pair = 5
      }
    }
    else if(validStraight(theCards, sizeOfPlay))
      return 8; //valid 6 card run = 8
    else
      return 0; //if not valid straight, then invalid cards
  }
  else if(validStraight(theCards, sizeOfPlay)) {
    return sizeOfPlay + 2; //if played more than 7 cards, can only be a straight
    //returns 9 for run of 7 cards
    //returns 10 for run of 8 cards
    //returns 11 for run of 9 cards
    //returns 12 for run of 10 cards
    //returns 13 for run of 11 cards
    //returns 14 for run of 12 cards
    //returns 15 for run of 12 cards
  }
  else
    return 0; //if not valid straight it's invalid cards
  
  return 0; //if it wasn't anything above, it's invalid cards
  
}

/*************************************************************
   ValidStraight : checks if play is in numbered order 
   Returns : 1 if true, 0 if false, -1 for NULL pointers
 *************************************************************/
int validStraight(card * theCards, int numOfCards) {

  if(!theCards) {
    printf("\n NULL POINTER \n");
    return -1;
  }
  
  int counter = 0;
  printf("\nNUM OF CARDS %d\n", numOfCards);
  //checks to ensure cards are in numerical order
  for(int i = 1; i < numOfCards; i++) {
    if(theCards[i-1].number == theCards[i].number - 1)
      counter++;
  }
  printf("\nCOUNTER %d\n", counter);
  if(counter == numOfCards-1)
    return 1;
  
  return 0;
}

/*************************************************************
   CardsInHand : finds the number of cards in a hand
   Returns : number of cards in hand or -1 for NULL pointers
 *************************************************************/
int cardsInHand(card * theHand) {

  if(!theHand) {
    printf("\n NULL POINTER \n");
    return -1;
  }
  
  int cardNum = 0;
  //gets amount of non 15/5 cards in hand
  for(int i = 0; i < 13; i++) {
    if(theHand[i].suit != 5)
      cardNum++;
  }

  //if cardNum == 0 -> player has no more cards and they win
  return cardNum;
}


  
      
