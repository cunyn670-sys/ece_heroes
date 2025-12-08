#ifndef GRAVITY_H
#define GRAVITY_H

#include "types.h"

// Descend les items et vide les cases (special remis à NONE sur vide)
void applyGravity(Item board[ROWS][COLS]);
// Remplit les vides par des items aléatoires (special remis à NONE)
void fillEmpty(Item board[ROWS][COLS]);

#endif
