#include "checkBoard.h"
#include "globals.h"
#include "turnLogic.h"
#include "networkUtilities.h"
#include <ctype.h>
#include <stdlib.h>

int playCheckers(int redPieces, int whitePieces, int myConcede, int opponentConcede);
void endGame(int redPieces, int whitePieces, int redConcede, int whiteConcede);

char myColor;
char curTurn;

int main(){
  int choice = 0;
  while((choice != 114) && (choice != 119)){
    printf("What color would you like to play as? (r/w) ");
    choice = tolower(getchar());
    // Clear buffer
    while(getchar() != '\n');
    printf("\n");
  }
  myColor = choice;
  createBoard();
  int redPieces = 12;
  int whitePieces = 12;
  int redConcede = 0;
  int whiteConcede = 0;
  playCheckers(redPieces, whitePieces, redConcede, whiteConcede);
  return 0;
}


int playCheckers(int redPieces, int whitePieces, int redConcede, int whiteConcede){
  curTurn = myColor;
  char fromCoord[3] = {'h'};
  char toCoord[3] = {'h'};
  // Drive the game
  while((redPieces > 1) && (whitePieces > 1) && (redConcede != 1) && (whiteConcede != 1)){
    printMyBoard(curTurn);
    printf("It is %c's turn!\n", curTurn);
    int take = canTake(curTurn);
    // Begin turn logic
    int moveMade = 0;
    while(!moveMade){
      // get src and dest coordinates from player
      while((fromCoord[0] == 'h') || (fromCoord[0] == 'p')|| (fromCoord[0] == 'c')){
        printf("Enter coordinate of piece to move, type h for help, type p to print the board: ");
        fgets(fromCoord, 3, stdin);
        printf("\n");
        if(tolower(fromCoord[0]) == 'h'){
          help();
        } else if(tolower(fromCoord[0]) == 'p'){
          printMyBoard(myColor);
          printf("Press ENTER to continue\n");
        } else if(tolower(fromCoord[0]) == 'c'){
          if(concede()){
            if(curTurn == 'r'){
              redConcede = 1;
            } else {
              whiteConcede = 1;
            }
            endGame(redPieces, whitePieces, redConcede, whiteConcede);
          }
        }
        while(getchar() != '\n');
      }
      while((toCoord[0] == 'h') || (toCoord[0] == 'p') || (toCoord[0] == 'c')){
        printf("Enter the destination coordinate of piece to move, type h for help, type p to print the board: ");
        fgets(toCoord, 3, stdin);
        printf("\n");
        if(tolower(toCoord[0]) == 'h'){
          help();
        } else if(tolower(toCoord[0]) == 'p'){
          printMyBoard(myColor);
          printf("Press ENTER to continue\n");
        } else if(tolower(fromCoord[0]) == 'c'){
          if(concede()){
            if(curTurn == 'r'){
              redConcede = 1;
            } else {
              whiteConcede = 1;
            }
            endGame(redPieces, whitePieces, redConcede, whiteConcede);
          }
        }
        while(getchar() != '\n');
      }
      // try to validate input move, otherwise loop and get new coords
      moveMade = validateMove(fromCoord, toCoord, take, curTurn, &redPieces, &whitePieces);
      // Reset input buffers
      fromCoord[0] = 'h';
      toCoord[0] = 'h';
      // If we have already taken a piece need to check to see if we can take again for double
      //TODO this is not quite right, it should only be the piece that moved that can take again
      if(take && moveMade){
        take = canTake(curTurn);
        if(take){
          moveMade = 0;
          printMyBoard(curTurn);
        }
      } 
    }
    // Need to make sure to alternate turns
    if(curTurn == 'r'){
      curTurn = 'w';
    } else {
      curTurn = 'r';
    }
  }
  endGame(redPieces, whitePieces, redConcede, whiteConcede);
  return 0;
}

void endGame(int redPieces, int whitePieces, int redConcede, int whiteConcede){
  if(redPieces < 1){
    printf("Red has no pieces, white wins!\n");
  } else if(whitePieces < 1){
    printf("White has no pieces, red wins!\n");
  } else if(redConcede){
    printf("Red conceded, white wins!\n");
  } else if(whiteConcede){
    printf("White conceded, red wins!\n");
  }
  exit(0);
}
