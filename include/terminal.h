#ifndef TERMINAL_H
#define TERMINAL_H

#include <termios.h>

extern struct termios origTermios;

// converts letter to control key equivalent by zeroing all but lowest 5 bits
#define CTRL_KEY(k) ((k) & 0x1f)

void die(const char* s);
void disableRawMode();
void enableRawMode();
char editorReadKey();
void editorProcessKeypress();
void editorRefreshScreen();
void editorDrawRows();
int getWindowSize();
void initEditor();
int getCursorPosition();

#endif // TERMINAL_H