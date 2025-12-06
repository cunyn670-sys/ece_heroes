#include <stdio.h>
#include <stdlib.h>
#include "main.h"
#include "modules/game.h"
#include "modules/display.h"
#include "modules/input.h"
#include "modules/level.h"
#include "modules/rules.h"
#include "modules/types.h"
#include "modules/rules.h"
#include "modules/save.h"
#include "modules/detect.h"
#include "modules/gravity.h"

void runProgram() {
    int choix = 0;
    char pseudo[64];

    while (choix != 4) {

        printf("=========== ECE HEROES ===========\n\n");
        printf("1 - Nouvelle Partie\n");
        printf("2 - Reprendre une partie\n");
        printf("3 - Lire les regles\n");
        printf("4 - Quitter\n\n");
        printf("Votre choix : ");
        scanf("%d", &choix);
        getchar(); // Clear newline

        switch (choix) {

            case 1:
                startGame();
                break;

            case 2:
                printf("Entrez votre pseudo : ");
                scanf("%s", pseudo);
                startLoadedGame(pseudo);
                break;

            case 3:
                afficherRegles();
                break;

            case 4:
                printf("\nAu revoir !\n");
                break;

            default:
                printf("Choix invalide.\n");
                break;
        }
    }
}

int main() {
    runProgram();
    return 0;
}
