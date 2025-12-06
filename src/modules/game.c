#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>

#include "game.h"
#include "types.h"
#include "display.h"
#include "input.h"
#include "detect.h"
#include "gravity.h"
#include "level.h"
#include "save.h"

void playLoop(Item board[ROWS][COLS], Level level, int *lives, char pseudo[]) {
    int cx = 0, cy = 0;      // Curseur
    int sx = -1, sy = -1;    // Sélection
    int running = 1;

    int moves = level.moves;
    int timeLeft = level.time;

    time_t lastTime = time(NULL);

    while (running) {

        // TIMER
        time_t now = time(NULL);
        if (now - lastTime >= 1) {
            timeLeft--;
            lastTime = now;
            if (timeLeft <= 0) {
                printf("\nTemps écoulé !\n");
                (*lives)--;
                Sleep(1000);
                return;
            }
        }

        clearScreen();
        displayBoard(board, cx, cy, sx, sy);
        displayInfo(level.level, *lives, moves, timeLeft, level.text);

        int k = readKey();

        if (k == 27) return; // ESC = abandon

        // Déplacements
        if (k == 'z' || k == 'Z') cx = (cx + ROWS - 1) % ROWS;
        if (k == 's' || k == 'S') cx = (cx + 1) % ROWS;
        if (k == 'q' || k == 'Q') cy = (cy + COLS - 1) % COLS;
        if (k == 'd' || k == 'D') cy = (cy + 1) % COLS;

        // Sélection / permutation
        if (k == ' ') {
            if (sx == -1) {
                sx = cx;
                sy = cy;
            } else {
                // Échange
                Item tmp = board[cx][cy];
                board[cx][cy] = board[sx][sy];
                board[sx][sy] = tmp;

                moves--;

                sx = sy = -1;

                // Vérification de combos
                int mark[ROWS][COLS] = {0};

                if (detectMatches(board, mark)) {

                    // Suppression
                    for (int i = 0; i < ROWS; i++)
                        for (int j = 0; j < COLS; j++)
                            if (mark[i][j] == 1)
                                board[i][j].type = ITEM_EMPTY;

                    // Gravité
                    applyGravity(board);
                    fillEmpty(board);
                }

                if (moves <= 0) {
                    printf("\nPlus de coups !\n");
                    (*lives)--;
                    Sleep(1000);
                    return;
                }
            }
        }

        // Objectif atteint ?
        int countO = 0;
        for (int i = 0; i < ROWS; i++)
            for (int j = 0; j < COLS; j++)
                if (board[i][j].type == level.targetType)
                    countO++;

        if (countO >= level.targetCount) {
            printf("\nObjectif atteint !\nSauvegarde...\n");
            saveGame(pseudo, level.level + 1, *lives);
            Sleep(1000);
            return;
        }

        Sleep(20);
    }
}

void generateBoard(Item board[ROWS][COLS]) {
    for (int i = 0; i < ROWS; i++)
        for (int j = 0; j < COLS; j++)
            board[i][j].type = rand() % 4;
}

void startGame() {
    char pseudo[64];
    printf("Entrez un pseudo pour sauvegarder : ");
    scanf("%s", pseudo);

    Item board[ROWS][COLS];
    generateBoard(board);

    Level L = getLevel(1);
    int lives = 3;

    saveGame(pseudo, 1, lives);

    playLoop(board, L, &lives, pseudo);
}

void startLoadedGame(char pseudo[]) {
    int level, lives;

    if (!loadGame(pseudo, &level, &lives)) {
        printf("Aucune sauvegarde trouvee pour ce pseudo.\n");
        Sleep(1500);
        return;
    }

    printf("Partie chargee : niveau %d, vies %d\n", level, lives);
    Sleep(1000);

    Item board[ROWS][COLS];
    generateBoard(board);

    Level L = getLevel(level);
    playLoop(board, L, &lives, pseudo);
}
