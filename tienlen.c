#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "cardgame.h"

int main(int argc, char * argv[]) {

  srand(time(0));

  int winner = 0;
  
  //command line arguments and parsing, only blank or -r for rules works
  if(argc == 2) {
    if(strcmp(argv[1], "-r") == 0) {
      
      // File opening code resources found at www.cs.utah.edu/~germain/
      // PPS/Topics/C_language/file_IO.html
      
      char character;
      
      FILE * rules_file = fopen("rules.txt", "r");
      
      if(!rules_file) {
      	printf("Could not find rules file.\n");
      	return -1;
        }
      
        while(fscanf(rules_file, "%c", &character) == 1) {
      	printf("%c", character);
         }
      
        fclose(rules_file);
      
      return 0; //Ends the rule book path
    }
    else {
      printf("Please input only ./tienlen -r for the rules or only ./tienlen to play\n");
      return -1;
    }
  }
  
  if(argc != 1) {
    printf("Please input only ./tienlen to start or the ./tienlen -r for the rules.\n");
    return -1;
  }

  //start of gameplay - these if's and else if's ensure the hand with the 3 of spades goes first
  winner = gameplay();
  
  printWinner(winner);
  
  return 0;
}
