#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include "terminal.h"



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

