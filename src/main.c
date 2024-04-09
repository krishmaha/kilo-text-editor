#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include "terminal.h"

// converts letter to control key equivalent by zeroing all but lowest 5 bits
#define CTRL_KEY(k) ((k) & 0x1f)

struct termios origTermios;

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

int main() {
  enableRawMode();
  while (1) {
    editorRefreshScreen();
    editorProcessKeypress();
    // char c = '\0';
    // if (read(STDIN_FILENO, &c, 1) == -1 && errno != EAGAIN) {
    //   die("read");
    // }
    // if (iscntrl(c)) {
    //   printf("%d\r\n", c);
    // } else {
    //   printf("%d ('%c')\r\n", c, c);
    // }
    // if (c == ('q')) {
    //   break;
    // }
  }
  return 0;
}

