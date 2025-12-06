#ifndef DISPLAY_H
#define DISPLAY_H

#include "types.h"

void clearScreen();
void printItem(Item it, int selected);
void displayBoard(Item board[ROWS][COLS], int cx, int cy, int sx, int sy);
void displayInfo(int level, int lives, int moves, int time, char objectiveText[]);
void setColorDefault();

#endif
