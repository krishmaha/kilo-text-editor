#ifndef TERMINAL_H
#define TERMINAL_H

#include <termios.h>

extern struct termios origTermios;

void die(const char* s);
void disableRawMode();
void enableRawMode();

#endif // TERMINAL_H