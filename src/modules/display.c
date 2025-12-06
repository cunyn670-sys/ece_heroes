#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "display.h"
#include "types.h"

void clearScreen() {
    system("cls");
}

void setColor(int color) {
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(h, color);
}

void setColorDefault() {
    setColor(7);
}

void printItem(Item it, int selected) {
    if (it.type == ITEM_EMPTY) {
        printf("   ");
        return;
    }

    if (selected) setColor(BACKGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    else {
        if (it.type == ITEM_O) setColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        else if (it.type == ITEM_X) setColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
        else if (it.type == ITEM_Q) setColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);
        else if (it.type == ITEM_P) setColor(FOREGROUND_RED | FOREGROUND_GREEN);
        else setColor(FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    }

    char c = (it.type == ITEM_O ? 'O' :
             it.type == ITEM_X ? 'X' :
             it.type == ITEM_Q ? '?' :
             it.type == ITEM_P ? '%' : '*');

    printf(" %c ", c);
    setColorDefault();
}

void displayBoard(Item board[ROWS][COLS], int cx, int cy, int sx, int sy) {
    printf("\n");

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            int selected = (i == sx && j == sy);
            printItem(board[i][j], selected);
        }
        printf("\n");
    }
}

void displayInfo(int level, int lives, int moves, int time, char objectiveText[]) {
    printf("\n");
    printf(" Niveau : %d\n", level);
    printf(" Vies   : %d\n", lives);
    printf(" Coups  : %d\n", moves);
    printf(" Temps  : %d sec\n", time);
    printf(" Objectif : %s\n", objectiveText);
}
