#include <stdio.h>
#include <string.h>
#include "save.h"

int saveGame(char pseudo[], int level, int lives) {
    FILE *f = fopen("saves.txt", "w");
    if (!f) return 0;

    fprintf(f, "%s %d %d\n", pseudo, level, lives);
    fclose(f);

    return 1;
}

int loadGame(char pseudo[], int *level, int *lives) {
    FILE *f = fopen("saves.txt", "r");
    if (!f) return 0;

    char p[64];
    while (fscanf(f, "%s %d %d", p, level, lives) == 3) {
        if (strcmp(p, pseudo) == 0) {
            fclose(f);
            return 1;
        }
    }

    fclose(f);
    return 0;
}
