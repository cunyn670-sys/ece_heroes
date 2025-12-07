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

                    // Vérification de combos
                    int mark[ROWS][COLS] = {0};

                    if (detectMatches(board, mark)) {
                        // Match valide → le coup est compté
                        moves--;

                        // Suppression
                        for (int i = 0; i < ROWS; i++)
                            for (int j = 0; j < COLS; j++)
                                if (mark[i][j] == 1)
                                    board[i][j].type = ITEM_EMPTY;

                        applyGravity(board);
                        fillEmpty(board);
                    } else {
                        // Pas de match → rollback de l'échange
                        Item tmp2 = board[cx][cy];
                        board[cx][cy] = board[sx][sy];
                        board[sx][sy] = tmp2;

                        // On NE compte PAS le coup
                    }

                    sx = sy = -1;

                    // Fin de l'action


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



void generateBoard(Item board[ROWS][COLS], Level level) {
    int mark[ROWS][COLS];
    int tries = 0;

    do {
        tries++;
        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLS; j++) {
                board[i][j].type = rand() % 4; // adapte le nombre de types si besoin
            }
        }

        memset(mark, 0, sizeof(mark));
        // tant qu'il y a des matches initiaux, régénérer
    } while (detectMatches(board, mark) && tries < 1000);

    // Optionnel : si tu veux éviter que l'objectif soit déjà atteint au départ,
    // on vérifie et regénère si count >= targetCount
    int countTarget = 0;
    for (int i = 0; i < ROWS; i++)
        for (int j = 0; j < COLS; j++)
            if (board[i][j].type == level.targetType) countTarget++;

    if (countTarget >= level.targetCount) {
        // régénérer (simple stratégie : recommencer la génération)
        tries = 0;
        do {
            tries++;
            for (int i = 0; i < ROWS; i++)
                for (int j = 0; j < COLS; j++)
                    board[i][j].type = rand() % 4;

            memset(mark, 0, sizeof(mark));
        } while ((detectMatches(board, mark) || 
                 // recompute countTarget
                 ({ int ct=0; for (int ii=0; ii<ROWS; ii++) for (int jj=0; jj<COLS; jj++) if (board[ii][jj].type == level.targetType) ct++; ct >= level.targetCount; })) 
                 && tries < 1000);
    }
}

void startGame() {
    srand(time(NULL));
    char pseudo[64];
    printf("Entrez un pseudo pour sauvegarder : ");
    scanf("%s", pseudo);

    Level L = getLevel(1);

    Item board[ROWS][COLS];
    generateBoard(board, L);
    // après generateBoard(...)
    int lives = 3;
    int countO = 0;
    for (int i = 0; i < ROWS; i++)
        for (int j = 0; j < COLS; j++)
            if (board[i][j].type == L.targetType) countO++;

    printf("DEBUG: countTarget = %d, level.targetCount = %d\n", countO, L.targetCount);

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

    Level L = getLevel(level);

    Item board[ROWS][COLS];
    generateBoard(board, L);

    playLoop(board, L, &lives, pseudo);
}
