#include "checkBoard.h"
#include "globals.h"
#include "turnLogic.h"
#include "networkUtilities.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

int playCheckers(int redPieces, int whitePieces, int myConcede, int opponentConcede);
void endGame(int redPieces, int whitePieces, int redConcede, int whiteConcede);

char myColor;
char curTurn;
char receive_buffer[RECEIVE_BUFFER_SIZE];
int sock_desc;

int main(){
  // Work to host/connect to a game
  printf("Would you like to host or connect? (h/c) ");
  char serv = tolower(getchar());
  while(getchar() != '\n');
  printf("\n");
  // Make sure no garbage in receive buffer
  memset(receive_buffer, 0, RECEIVE_BUFFER_SIZE);
  
  // Whether user running this is hosting or connecting, sock_desc will be the fd they write to and read from
  if(serv == 'h'){
    // Init server
    sock_desc = createServerAndWait();
    receiveAck(sock_desc);
  } else {
    // Connect to server
    sock_desc = connectToServer();
    sendAck(sock_desc);
  }
  if(sock_desc == -1){
    printf("Fatal error during connection, returned sock_desc == -1\n");
    exit(-1);
  }
  char choice;
  // Give client the choice to choose what color they would like
  if(serv == 'c'){
    while((choice != 'r') && (choice != 'w')){
      printf("What color would you like to play as? (r/w) ");
      choice = tolower(getchar());
      // Clear buffer
      while(getchar() != '\n');
      printf("\n");
    }
    myColor = choice;
    if(choice == 'r'){
      sprintf(receive_buffer, "w\n");
      send_message(sock_desc, receive_buffer);
    } else {
      sprintf(receive_buffer, "r\n");
      send_message(sock_desc, receive_buffer);
    }
    receive_board(sock_desc, receive_buffer);
    sendAck(sock_desc);
  } else {
    receive(sock_desc, receive_buffer, RECEIVE_BUFFER_SIZE);
    myColor = receive_buffer[0];
    printf("Client chose their color so I am %c!\n", myColor);
    // Have only the server create the board and then pass it to the client
    createBoard();
    send_board(sock_desc, receive_buffer);
    receiveAck(sock_desc);
  }
  int redPieces = 12;
  int whitePieces = 12;
  int redConcede = 0;
  int whiteConcede = 0;
  playCheckers(redPieces, whitePieces, redConcede, whiteConcede);
  return 0;
}


int playCheckers(int redPieces, int whitePieces, int redConcede, int whiteConcede){
  curTurn = 'r';
  char fromCoord[3] = {'h'};
  char toCoord[3] = {'h'};
  // Drive the game
  while((redPieces > 1) && (whitePieces > 1) && (redConcede != 1) && (whiteConcede != 1)){
    printMyBoard(myColor);
    printf("It is %c's turn!\n", curTurn);
    int take = canTake(myColor);
    // Begin turn logic
    int moveMade = 0;
    while(!moveMade && curTurn == myColor){
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
    // If it is not our turn, wait to receive items
    // Need to make sure to alternate turns
    if(curTurn != myColor){
      printf("Not my turn!\n");
      receiveTurnData(sock_desc, receive_buffer, &redPieces, &whitePieces, &redConcede, &whiteConcede);
    } else {
      sendTurnData(sock_desc, receive_buffer, &redPieces, &whitePieces, &redConcede, &whiteConcede);
    }
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
