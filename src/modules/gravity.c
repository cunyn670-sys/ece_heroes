#include "gravity.h"
#include <stdlib.h>
#include <time.h>
#include "types.h"
void applyGravity(Item board[ROWS][COLS]) {
    for (int j = 0; j < COLS; j++) {
        int write = ROWS - 1;

        for (int i = ROWS - 1; i >= 0; i--) {
            if (board[i][j].type != ITEM_EMPTY) {
                board[write][j] = board[i][j];
                write--;
            }
        }

        for (int i = write; i >= 0; i--) {
            board[i][j].type = ITEM_EMPTY;
            board[i][j].special = SPECIAL_NONE;
        }
    }
}

void fillEmpty(Item board[ROWS][COLS]) {
    for (int i = 0; i < ROWS; i++)
        for (int j = 0; j < COLS; j++)
            if (board[i][j].type == ITEM_EMPTY) {
                board[i][j].type = rand() % 4;
                board[i][j].special = SPECIAL_NONE;
            }
}
