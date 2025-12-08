#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "display.h"
#include "types.h"

void clearScreen() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD count;
    DWORD cellCount;
    COORD homeCoords = {0, 0};

    if (hOut == INVALID_HANDLE_VALUE) return;

    if (!GetConsoleScreenBufferInfo(hOut, &csbi)) return;
    cellCount = csbi.dwSize.X * csbi.dwSize.Y;

    FillConsoleOutputCharacter(hOut, ' ', cellCount, homeCoords, &count);
    FillConsoleOutputAttribute(hOut, csbi.wAttributes, cellCount, homeCoords, &count);
    SetConsoleCursorPosition(hOut, homeCoords);
}

void setColor(int color) {
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(h, color);
}

void setColorDefault() {
    setColor(7); // gris clair
}

void printItem(Item it, int highlighted) {
    if (it.type == ITEM_EMPTY) {
        if (highlighted) {
            setColor(BACKGROUND_BLUE | BACKGROUND_GREEN | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
            printf("   ");
            setColorDefault();
        } else {
            printf("   ");
        }
        return;
    }

    if (highlighted) {
        // sélection
        setColor(BACKGROUND_BLUE | BACKGROUND_GREEN | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    } else {
        switch (it.type) {
            case ITEM_O: setColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY); break;   // O vert
            case ITEM_X: setColor(FOREGROUND_RED | FOREGROUND_INTENSITY); break;     // X rouge
            case ITEM_Q: setColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY); break;    // ? bleu
            case ITEM_P: setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY); break; // % jaune
            default: setColor(FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY); break; // spéciaux en magenta
        }
    }

    char c;
    if (it.special == SPECIAL_BOMB) c = 'B';
    else if (it.special == SPECIAL_LINE) c = '-';
    else c = (it.type == ITEM_O ? 'O' :
             it.type == ITEM_X ? 'X' :
             it.type == ITEM_Q ? '?' :
             it.type == ITEM_P ? '%' : '.');

    printf(" %c ", c);
    setColorDefault();
}

void displayBoard(Item board[ROWS][COLS], int cursorX, int cursorY, int selectX, int selectY) {
    for (int i = 0; i < ROWS; i++) {
        setCursor(0, i);
        for (int j = 0; j < COLS; j++) {
            int highlighted = (i == cursorX && j == cursorY) || (i == selectX && j == selectY);
            printItem(board[i][j], highlighted);
        }
        printf("\n");
    }
}

static void printProgressBar(int collected, int target) {
    int width = 20;
    if (target <= 0) target = 1;
    int filled = collected * width / target;
    if (filled > width) filled = width;

    printf(" [");
    for (int i = 0; i < width; i++) {
        if (i < filled) printf("#");
        else printf("-");
    }
    printf("] %d/%d\n", collected, target);
}

void displayInfo(int level, int lives, int moves, int time, char objectiveText[], int collected, int target) {
    printf("\n");
    printf(" Niveau : %d\n", level);
    printf(" Vies   : %d\n", lives);
    printf(" Coups  : %d\n", moves);
    printf(" Temps  : %d sec\n", time);
    printf(" Objectif : %s\n", objectiveText);
    printProgressBar(collected, target);
}

void setCursor(int x, int y) {
    COORD pos = { x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}
