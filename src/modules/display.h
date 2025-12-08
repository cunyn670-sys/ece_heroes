#ifndef DISPLAY_H
#define DISPLAY_H

#include "types.h"

// Efface l'ecran (cls)
void clearScreen();

// Change la couleur du texte
void setColor(int color);

// Restaure la couleur par defaut
void setColorDefault();

// Affiche un item (avec ou sans surbrillance, speciaux compris)
void printItem(Item it, int highlighted);

// Affiche le plateau entier
void displayBoard(Item board[ROWS][COLS], int cx, int cy, int sx, int sy);

// Affiche les infos niveau / vies / coups / temps + progression objectif
void displayInfo(int level, int lives, int moves, int time, char objectiveText[], int collected, int target);

void setCursor(int x, int y);
#endif
