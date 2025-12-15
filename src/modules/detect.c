// detect.c
#include "detect.h"
#include "types.h"
#include <string.h>

// Mark every occurrence of a type on a given row and column
static void markTypeOnRowAndCol(Item board[ROWS][COLS], int mark[ROWS][COLS], int row, int col, int type) {
    for (int c = 0; c < COLS; c++) {
        if (board[row][c].type == type) mark[row][c] = 1;
    }
    for (int r = 0; r < ROWS; r++) {
        if (board[r][col].type == type) mark[r][col] = 1;
    }
}

int detectMatches(Item board[ROWS][COLS], int mark[ROWS][COLS]) {
    int found = 0;
    // s'assurer que mark est clean si quelqu'un oublie
    memset(mark, 0, sizeof(int) * ROWS * COLS);

    // Horizontal
    for (int i = 0; i < ROWS; i++) {
        int j = 0;
        while (j < COLS) {
            int t = board[i][j].type;
            if (t == ITEM_EMPTY) { j++; continue; }
            int len = 1;
            while (j + len < COLS && board[i][j + len].type == t) len++;
            if (len >= 3) {
                found = 1;
                for (int k = 0; k < len; k++) mark[i][j + k] = 1;
            }
            j += len;
        }
    }

    // Vertical
    for (int j = 0; j < COLS; j++) {
        int i = 0;
        while (i < ROWS) {
            int t = board[i][j].type;
            if (t == ITEM_EMPTY) { i++; continue; }
            int len = 1;
            while (i + len < ROWS && board[i + len][j].type == t) len++;
            if (len >= 3) {
                found = 1;
                for (int k = 0; k < len; k++) mark[i + k][j] = 1;
            }
            i += len;
        }
    }

    return found;
}

// Etend les suppressions selon les regles de patterns
void applyPatternClears(Item board[ROWS][COLS], int mark[ROWS][COLS]) {
    int clearType[16] = {0};

    // Suites horizontales / verticales : marquer >=4 et declencher clear global pour >=6
    for (int i = 0; i < ROWS; i++) {
        int j = 0;
        while (j < COLS) {
            int t = board[i][j].type;
            if (t == ITEM_EMPTY) { j++; continue; }
            int len = 1;
            while (j + len < COLS && board[i][j + len].type == t) len++;
            if (len >= 4) {
                for (int k = 0; k < len; k++) mark[i][j + k] = 1;
                if (len >= 6) clearType[t] = 1; // suite de 6 => clear global du type
            }
            j += len;
        }
    }
    for (int j = 0; j < COLS; j++) {
        int i = 0;
        while (i < ROWS) {
            int t = board[i][j].type;
            if (t == ITEM_EMPTY) { i++; continue; }
            int len = 1;
            while (i + len < ROWS && board[i + len][j].type == t) len++;
            if (len >= 4) {
                for (int k = 0; k < len; k++) mark[i + k][j] = 1;
                if (len >= 6) clearType[t] = 1; // suite de 6 => clear global du type
            }
            i += len;
        }
    }

    // Clear global pour les types marques
    for (int t = 0; t < 16; t++) {
        if (!clearType[t]) continue;
        for (int i = 0; i < ROWS; i++)
            for (int j = 0; j < COLS; j++)
                if (board[i][j].type == t) mark[i][j] = 1;
    }

    // Croix de 9 (bras de longueur 2) : supprime tous les items du type sur la meme ligne/colonne
    for (int i = 2; i < ROWS - 2; i++) {
        for (int j = 2; j < COLS - 2; j++) {
            int t = board[i][j].type;
            if (t == ITEM_EMPTY) continue;
            int ok = 1;
            for (int k = -2; k <= 2 && ok; k++) {
                if (board[i + k][j].type != t) ok = 0;
                if (board[i][j + k].type != t) ok = 0;
            }
            if (ok) markTypeOnRowAndCol(board, mark, i, j, t);
        }
    }

    // Carre 4x4 homogene : supprime tout le carre (meme type partout)
    for (int i = 0; i <= ROWS - 4; i++) {
        for (int j = 0; j <= COLS - 4; j++) {
            int t = board[i][j].type;
            if (t == ITEM_EMPTY) continue;
            int ok = 1;
            for (int di = 0; di < 4 && ok; di++)
                for (int dj = 0; dj < 4; dj++)
                    if (board[i + di][j + dj].type != t) { ok = 0; break; }
            if (ok) {
                for (int di = 0; di < 4; di++)
                    for (int dj = 0; dj < 4; dj++)
                        mark[i + di][j + dj] = 1;
            }
        }
    }
}
