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

static void addSpecialBlast(Item board[ROWS][COLS], int mark[ROWS][COLS]) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (mark[i][j] == 1) {
                if (board[i][j].special == SPECIAL_LINE) {
                    for (int c = 0; c < COLS; c++) mark[i][c] = 1;
                } else if (board[i][j].special == SPECIAL_BOMB) {
                    for (int di = -1; di <= 1; di++)
                        for (int dj = -1; dj <= 1; dj++) {
                            int ni = i + di, nj = j + dj;
                            if (ni >= 0 && ni < ROWS && nj >= 0 && nj < COLS) mark[ni][nj] = 1;
                        }
                }
            }
        }
    }
}

static int countMarked(int mark[ROWS][COLS]) {
    int c = 0;
    for (int i = 0; i < ROWS; i++)
        for (int j = 0; j < COLS; j++)
            if (mark[i][j]) c++;
    return c;
}

// Étend les suppressions selon les nouvelles règles
static void applyPatternClears(Item board[ROWS][COLS], int mark[ROWS][COLS]) {
    int clearType[16] = {0};

    // Suites horizontales / verticales : marquer >=4 et déclencher clear global pour >=6
    for (int i = 0; i < ROWS; i++) {
        int j = 0;
        while (j < COLS) {
            int t = board[i][j].type;
            if (t == ITEM_EMPTY) { j++; continue; }
            int len = 1;
            while (j + len < COLS && board[i][j + len].type == t) len++;
            if (len >= 4) {
                for (int k = 0; k < len; k++) mark[i][j + k] = 1;
                if (len >= 6) clearType[t] = 1;
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
                if (len >= 6) clearType[t] = 1;
            }
            i += len;
        }
    }

    // Clear global pour les types marqués
    for (int t = 0; t < 16; t++) {
        if (!clearType[t]) continue;
        for (int i = 0; i < ROWS; i++)
            for (int j = 0; j < COLS; j++)
                if (board[i][j].type == t) mark[i][j] = 1;
    }

    // Croix de 9 (bras de longueur 2) : supprime tous les X sur la même ligne/colonne
    for (int i = 2; i < ROWS - 2; i++) {
        for (int j = 2; j < COLS - 2; j++) {
            int t = board[i][j].type;
            if (t == ITEM_EMPTY) continue;
            int ok = 1;
            for (int k = -2; k <= 2 && ok; k++) {
                if (board[i + k][j].type != t) ok = 0;
                if (board[i][j + k].type != t) ok = 0;
            }
            if (ok) {
                for (int c = 0; c < COLS; c++)
                    if (board[i][c].type == t) mark[i][c] = 1;
                for (int r = 0; r < ROWS; r++)
                    if (board[r][j].type == t) mark[r][j] = 1;
            }
        }
    }

    // Carré 4x4 homogène : supprime tout le carré
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

// nombre de cases d'un type
int countType(Item board[ROWS][COLS], int type) {
    int cnt = 0;
    for (int i = 0; i < ROWS; i++)
        for (int j = 0; j < COLS; j++)
            if (board[i][j].type == type) cnt++;
    return cnt;
}

// Generation du plateau initial sans combo ni objectif atteint d'emblee
void generateBoard(Item board[ROWS][COLS], Level level) {
    int tries = 0;

    while (tries < 1000) {
        int countTarget = 0;

        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLS; j++) {
                int t;
                int reroll = 0;
                do {
                    t = rand() % 4; // 4 types de base
                    reroll++;
                } while (reroll < 20 &&
                        ((j >= 2 && board[i][j - 1].type == t && board[i][j - 2].type == t) ||
                         (i >= 2 && board[i - 1][j].type == t && board[i - 2][j].type == t)));

                board[i][j].type = t;
                board[i][j].special = SPECIAL_NONE;
                if (t == level.targetType) countTarget++;
            }
        }

        int mark[ROWS][COLS] = {0};
        if (!detectMatches(board, mark) && countTarget < level.targetCount) {
            return;
        }

        tries++;
    }
}

void playLoop(Item board[ROWS][COLS], Level level, int *lives, char pseudo[]) {
    int cx = 0, cy = 0;      // Curseur
    int sx = -1, sy = -1;    // Selection
    int running = 1;
    int needsRefresh = 1;

    int moves = level.moves;
    int timeLeft = level.time;
    int collectedTarget = 0; // elements collectes

    time_t lastTime = time(NULL);

    while (running) {
        // TIMER
        time_t now = time(NULL);
        if (now - lastTime >= 1) {
            timeLeft--;
            lastTime = now;
            needsRefresh = 1;
            if (timeLeft <= 0) {
                printf("\nTemps ecoule !\n");
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
                displayInfo(level.level, *lives, moves, timeLeft, level.text, collectedTarget, level.targetCount);
                needsRefresh = 0;
            }
            Sleep(30);
            continue;
        }
        needsRefresh = 1;
        if (k == 27) return; // ESC = abandon

        // Deplacements
        if (k == 'z' || k == 'Z') cx = (cx + ROWS - 1) % ROWS;
        if (k == 's' || k == 'S') cx = (cx + 1) % ROWS;
        if (k == 'q' || k == 'Q') cy = (cy + COLS - 1) % COLS;
        if (k == 'd' || k == 'D') cy = (cy + 1) % COLS;

        // Selection / permutation
        if (k == ' ') {
            if (sx == -1) {
                sx = cx;
                sy = cy;
            } else {
                // deplacement limite a une case adjacente
                if (abs(cx - sx) + abs(cy - sy) != 1) {
                    sx = sy = -1;
                    continue;
                }

                // Echange
                Item tmp = board[cx][cy];
                board[cx][cy] = board[sx][sy];
                board[sx][sy] = tmp;

                // Verification de combos
                int mark[ROWS][COLS] = {0};
                int baseType = board[cx][cy].type;
                int createdSpecial = 0;
                int specialKind = SPECIAL_NONE;

                if (detectMatches(board, mark)) {
                    applyPatternClears(board, mark);
                    addSpecialBlast(board, mark);
                    // Match valide => on compte le coup
                    moves--;

                    int cleared = countMarked(mark);
                    if (cleared >= 5) {
                        createdSpecial = 1;
                        specialKind = SPECIAL_BOMB;
                    } else if (cleared >= 4) {
                        createdSpecial = 1;
                        specialKind = SPECIAL_LINE;
                    }

                    // Suppression et comptage des elements collectes
                    for (int i = 0; i < ROWS; i++)
                        for (int j = 0; j < COLS; j++)
                            if (mark[i][j] == 1) {
                                if (board[i][j].type == level.targetType)
                                    collectedTarget++;
                                board[i][j].type = ITEM_EMPTY;
                                board[i][j].special = SPECIAL_NONE;
                            }

                    // Montrer brièvement les cases vidées avant la gravité
                    clearScreen();
                    displayBoard(board, cx, cy, sx, sy);
                    displayInfo(level.level, *lives, moves, timeLeft, level.text, collectedTarget, level.targetCount);
                    Sleep(200);

                    applyGravity(board);

                    if (createdSpecial) {
                        board[cx][cy].type = baseType;
                        board[cx][cy].special = specialKind;
                    }

                    fillEmpty(board);
                } else {
                    // Pas de match => rollback de l'echange
                    Item tmp2 = board[cx][cy];
                    board[cx][cy] = board[sx][sy];
                    board[sx][sy] = tmp2;
                    // le coup n'est pas compte
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
                printf("Bravo %s ! Retour au menu principal.\n", pseudo);
                Sleep(3000);
                return;
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
            displayInfo(level.level, *lives, moves, timeLeft, level.text, collectedTarget, level.targetCount);
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
