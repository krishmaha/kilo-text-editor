#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <stdlib.h>

struct termios origTermios;

void disableRawMode() {
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &origTermios);
}

// turn off echoing in the terminal
void enableRawMode() {

  // read original (no echo) terminal attributes into a struct
  tcgetattr(STDIN_FILENO, &origTermios);
  atexit(disableRawMode);
  struct termios raw = origTermios;

  // turns off echo and canonical so we read input byte by byte rather 
  // than line by line
  raw.c_lflag &= ~(ECHO | ICANON);

  // applies new flags to the terminal, after all pending output has been 
  // written and discarding unread input
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
  
}

int main() {
  enableRawMode();
  char c;
  // chars after q will be left unread on the input queue
  // fed into shell after program exits
  while (read(STDIN_FILENO, &c, 1) == 1 && c != 'q');
  return 0;
}