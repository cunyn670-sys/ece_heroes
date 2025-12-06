#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include "rules.h"

void afficherRegles() {
    system("cls");

    printf("=========== REGLES DU JEU ===========\n\n");

    printf("1. Le tableau est compose de 15x15 items :\n");
    printf("   O (vert), X (rouge), ? (bleu), %% (jaune).\n\n");

    printf("2. Le but du jeu :\n");
    printf("   - Completer l'objectif du niveau (ex: collecter 10 O)\n");
    printf("   - Avant la fin du temps et sans depasser le nombre de coups.\n\n");

    printf("3. Comment jouer :\n");
    printf("   - Utiliser ZQSD pour deplacer le curseur.\n");
    printf("   - Appuyer sur ESPACE pour selectionner un item.\n");
    printf("   - Deplacer le curseur vers un voisin (Haut/Bas/Gauche/Droite)\n");
    printf("     puis appuyer sur ESPACE pour permuter.\n\n");

    printf("4. Combinaisons possibles :\n");
    printf("   - Aligner 4 items identiques horizontalement.\n");
    printf("   - D'autres combinaisons seront ajoutees.\n\n");

    printf("5. Gravite :\n");
    printf("   - Quand des items sont supprimes, ceux du dessus tombent.\n");
    printf("   - Les cases vides sont remplacees par de nouveaux items aleatoires.\n\n");

    printf("6. Sauvegardes :\n");
    printf("   - Si vous entrez un pseudo, le niveau atteint est sauvegarde.\n");
    printf("   - Vous pouvez reprendre plus tard avec l'option 'Reprendre une partie'.\n\n");

    printf("======================================\n");
    printf("Appuyez sur n'importe quelle touche pour revenir au menu...");
    
    _getch();
    system("cls");
}
