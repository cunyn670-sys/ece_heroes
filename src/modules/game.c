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

// Fonction pour compter le nombre de cases d'un type donné sur le plateau
int countType(Item board[ROWS][COLS], int type) {
    int cnt = 0;
    for (int i = 0; i < ROWS; i++)
        for (int j = 0; j < COLS; j++)
            if (board[i][j].type == type) cnt++;
    return cnt;
}
// Génération du plateau
void generateBoard(Item board[ROWS][COLS], Level level) {
    int mark[ROWS][COLS];
    int tries = 0;
    int countTarget = 0;

    do {
        tries++;
        for (int i = 0; i < ROWS; i++)
            for (int j = 0; j < COLS; j++)
                board[i][j].type = rand() % 4; // adapte le nombre de types si besoin

        memset(mark, 0, sizeof(mark));

        countTarget = countType(board, level.targetType);
    } while ((detectMatches(board, mark) || countTarget >= level.targetCount) && tries < 1000);
}
void playLoop(Item board[ROWS][COLS], Level level, int *lives, char pseudo[]) {
    int cx = 0, cy = 0;      // Curseur
    int sx = -1, sy = -1;    // Sélection
    int running = 1;
    int needsRefresh = 1;

    int moves = level.moves;
    int timeLeft = level.time;
    int collectedTarget = 0; // compteur d'éléments collectés

    time_t lastTime = time(NULL);

    while (running) {
        // TIMER
        time_t now = time(NULL);
        if (now - lastTime >= 1) {
            timeLeft--;
            lastTime = now;
            needsRefresh = 1;
            if (timeLeft <= 0) {
                printf("\nTemps écoulé !\n");
                (*lives)--;
                Sleep(1000);
                return;
            }
        }

        int k = readKey();
        if (k == -1) {
            if (needsRefresh) {
                clearScreen();
                displayBoard(board, cx, cy, sx, sy);
                displayInfo(level.level, *lives, moves, timeLeft, level.text);
                needsRefresh = 0;
            }
            Sleep(30);
            continue;
        }
        needsRefresh = 1;
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

                // Vérification de combos
                int mark[ROWS][COLS] = {0};
                if (detectMatches(board, mark)) {
                    // Match valide → on compte le coup
                    moves--;

                    // Suppression et comptage des éléments collectés
                    for (int i = 0; i < ROWS; i++)
                        for (int j = 0; j < COLS; j++)
                            if (mark[i][j] == 1) {
                                if (board[i][j].type == level.targetType)
                                    collectedTarget++;
                                board[i][j].type = ITEM_EMPTY;
                            }

                    applyGravity(board);
                    fillEmpty(board);
                } else {
                    // Pas de match → rollback de l'échange
                    Item tmp2 = board[cx][cy];
                    board[cx][cy] = board[sx][sy];
                    board[sx][sy] = tmp2;
                    // le coup n'est pas compté
                }

                sx = sy = -1;

                if (moves <= 0) {
                    printf("\nPlus de coups !\n");
                    (*lives)--;
                    Sleep(1000);
                    return;
                }
            }
        }

            // Objectif atteint ?
        if (collectedTarget >= level.targetCount) {

            // --- SI NIVEAU FINAL ---
            if (level.level >= 4) {
                clearScreen();
                printf("\n FELICITATIONS ! \n");
                printf("Vous avez termine le niveau 4 et gagne la partie !\n");
                printf("Bravo %s !\n", pseudo);
                Sleep(3000);
                exit(0);  // Fin totale du programme
            }

            // --- SINON PASSAGE AU NIVEAU SUIVANT ---
            printf("\nObjectif atteint ! Passage au niveau %d...\n", level.level + 1);
            saveGame(pseudo, level.level + 1, *lives);
            Sleep(1000);

            Level nextLevel = getLevel(level.level + 1);
            Item newBoard[ROWS][COLS];
            generateBoard(newBoard, nextLevel);

            playLoop(newBoard, nextLevel, lives, pseudo);
            return;
        }


        if (needsRefresh) {
            clearScreen();
            displayBoard(board, cx, cy, sx, sy);
            displayInfo(level.level, *lives, moves, timeLeft, level.text);
            needsRefresh = 0;
        }

        Sleep(2);
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

    Level L = getLevel(level);

    Item board[ROWS][COLS];
    generateBoard(board, L); // si tu veux vraiment sauvegarder le plateau, il faudra le stocker

    playLoop(board, L, &lives, pseudo);
}
