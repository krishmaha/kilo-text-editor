#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include <sys/ioctl.h>

#include "terminal.h"

struct editorConfig {
  int screenrows;
  int screencols;
  struct termios origTermios;
};

struct editorConfig E;


void die(const char* s) {
  write(STDOUT_FILENO, "\x1b[2J", 4);
  write(STDOUT_FILENO, "\x1b[H", 3);
  perror(s);
  exit(1);
}

void disableRawMode() {
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &E.origTermios) == -1) {
    die("tcsetattr");
  }
}

// turn off echoing in the terminal
void enableRawMode() {

  // read original (no echo) terminal attributes into a struct
  if (tcgetattr(STDIN_FILENO, &E.origTermios) == -1) {
    die("tcgetattr");
  };
  atexit(disableRawMode);

  struct termios raw = E.origTermios;

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
      // Using atexit() to clear screen when program exits would erase the message 
      // printed by die straight away after printing
      write(STDOUT_FILENO, "\x1b[2J", 4);
      write(STDOUT_FILENO, "\x1b[H", 3);
      exit(0);
      break;
  }
}

void editorRefreshScreen() {
  // Clear entire screen
  // Escape sequence is 4 bytes long - write 4 bytes out to terminal
  write(STDOUT_FILENO, "\x1b[2J", 4);
  // Positions the cursor (using H command) to top left of screen
  // No arguments specified for row and col e.g. (<esc>[12;40H) defaults to top left
  write(STDOUT_FILENO, "\x1b[H", 3);
  editorDrawRows();
  write(STDOUT_FILENO, "\x1b[H", 3);
}

void editorDrawRows() {
  int y;
  for (y = 0; y < E.screenrows; y++) {
    // Print tildes (~) in each row like Vim 
    write(STDOUT_FILENO, "~\r\n", 3);
  }
}

int getWindowSize(int *rows, int *cols) {
  struct winsize ws;
  // TIOCGWINSZ: terminal IOCtl get window size
  // Get size of the terminal window
  if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0) {
    return -1;
  } else {
    *cols = ws.ws_col;
    *rows = ws.ws_row;
    return 0;
  }
}

void initEditor() {
  if (getWindowSize(&E.screenrows, &E.screencols) == -1) {
    die("getWindowSize");
  }
}