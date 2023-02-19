#include "checkBoard.h"
#include "turnLogic.h"

#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

// Consider making a gameDriver.h file to gain access to the gameDriver global vars

// Record as much info about the state of the game as possible before exiting
void recordErrorAndClose(int ix, int iy, int jx, int jy, int *redPieces, int *whitePieces){
  int fd;
  // Note on western nfs O_APPEND might lead to file corruption. Hopefuly we don't need this too much
  // 404 should be anyone can read
  fd = open("errorLog", O_WRONLY | O_CREAT | O_APPEND, 404);
  if(fd == -1){
    printf("Error on opening file\n");
    exit(-1);
  }
  // Point stdout to the open file
  if(dup2(fd, 1) == -1){
    printf("Dup2 failed\n");
    exit(-1);
  }
  printMyBoard('w');
  printMyBoard('r');
  printf("redPieces is %d, whitePieces is %d\n", *redPieces, *whitePieces);
  printf("ix is %d, iy is %d, jx is %d, jy is %d\n", ix, iy, jx, jy);
  exit(-1);
}
