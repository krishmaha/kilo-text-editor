#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
#include <ctype.h>

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

int main() {
  enableRawMode();
  while (1) {
    char c = '\0';
    read(STDIN_FILENO, &c, 1);
    if (iscntrl(c)) {
      printf("%d\r\n", c);
    } else {
      printf("%d ('%c')\r\n", c, c);
    }
    if (c == 'q') {
      break;
    }
  }
  return 0;
}