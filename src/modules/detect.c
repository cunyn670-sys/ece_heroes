// detect.c
#include "detect.h"
#include "types.h"
#include <string.h>

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
