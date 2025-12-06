#include "detect.h"
#include "types.h"

int detect_line4(Item board[ROWS][COLS], int mark[ROWS][COLS]) {
    int found = 0;

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS - 3; j++) {
            int t = board[i][j].type;

            if (t >= 0 &&
                t == board[i][j+1].type &&
                t == board[i][j+2].type &&
                t == board[i][j+3].type) {

                mark[i][j] = mark[i][j+1] = mark[i][j+2] = mark[i][j+3] = 1;
                found = 1;
            }
        }
    }

    return found;
}

int detectMatches(Item board[ROWS][COLS], int mark[ROWS][COLS]) {
    int found = 0;
    found |= detect_line4(board, mark);
    return found;
}
