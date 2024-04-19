#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include "terminal.h"

struct termios origTermios;

void die(const char* s) {
  perror(s);
  exit(1);
}

void disableRawMode() {
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &origTermios) == -1) {
    die("tcsetattr");
  }
}

// turn off echoing in the terminal
void enableRawMode() {

  // read original (no echo) terminal attributes into a struct
  if (tcgetattr(STDIN_FILENO, &origTermios) == -1) {
    die("tcgetattr");
  };
  atexit(disableRawMode);
  struct termios raw = origTermios;

  // turns off echo and canonical so we read input byte by byte rather 
  // than line by line. Also allows keypresses that would otherwise terminate program
  raw.c_iflag &= ~(IXON | ICRNL | BRKINT | INPCK | ISTRIP);
  raw.c_oflag &= ~(OPOST);
  raw.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN);
  // set character size to 8 bits ber byte
  raw.c_cflag |= (CS8);
  // minimum number of input bytes needed before read() can return 
  raw.c_cc[VMIN] = 0;
  // maximum time to wait (in tenths of a second) before read() can return
  raw.c_cc[VTIME] = 1;

  // applies new flags to the terminal, after all pending output has been 
  // written and discarding unread input
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
  
}


char editorReadKey() {
  int nread;
  char c;
  while ((nread = read(STDIN_FILENO, &c, 1)) != 1) {
    if (nread == -1 && errno != EAGAIN) {
      die("read");
    } 
  }
  return c;
}

void editorProcessKeypress() {
  char c = editorReadKey();
  switch (c) {
    case CTRL_KEY('q'):
      exit(0);
      break;
  }
}

void editorRefreshScreen() {
  write(STDOUT_FILENO, "\x1b[2J", 4);
}