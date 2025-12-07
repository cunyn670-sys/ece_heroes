#ifndef DISPLAY_H
#define DISPLAY_H

#include "types.h"

// Efface l'écran (cls)
void clearScreen();

// Change la couleur du texte
void setColor(int color);

// Restaure la couleur par défaut
void setColorDefault();

// Affiche un item (avec ou sans surbrillance)
void printItem(Item it, int highlighted);

// Affiche le plateau entier
void displayBoard(Item board[ROWS][COLS], int cx, int cy, int sx, int sy);

// Affiche les infos niveau / vies / coups / temps
void displayInfo(int level, int lives, int moves, int time, char objectiveText[]);

#endif
