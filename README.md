# ECE Heroes

Petit match-3 console sous Windows (WinAPI pour l'affichage couleur et la capture des touches). Le plateau est généré sans alignements initiaux de 3, les déplacements se font case par case, et des bonus spéciaux apparaissent sur les combinaisons de 4+.

## Construire et lancer
```bash
pwd
cd src
gcc -o ece_heroes.exe main.c modules/*.c
./ece_heroes.exe
```

## Commandes en jeu
- `z / s / q / d` : déplacer le curseur.
- `espace` : sélectionner puis permuter avec une case adjacente.
- `echap` : abandonner la partie en cours.

## Règles principales
- Objectif : collecter un nombre d'items cibles avant la fin des coups/du temps pour passer au niveau suivant.
- Génération : le plateau initial ne contient pas de séries de 3 et ne valide pas déjà l'objectif.
- Spéciaux :
  - Match de 4 → item spécial ligne (efface la ligne complète quand détruit).
  - Match de 5+ → bombe (efface un carré 3x3 autour de sa position).
  - Les effets se déclenchent lors d'une élimination et se propagent avant gravité.
- Niveau 4 terminé → retour au menu principal.

## Sauvegarde
- `saves.txt` stocke pseudo, niveau et vies. Charger via le menu « Reprendre une partie ».

## Dépendances
- Windows (utilise `<windows.h>` et `_getch`).
- GCC (MinGW) ou tout compilateur compatible C sous Windows.

## Structure rapide
- `src/main.c` : menu principal.
- `src/modules/game.c` : boucle de jeu, génération, règles.
- `src/modules/display.c` : rendu console et couleurs.
- `src/modules/detect.c` : détection de matches.
- `src/modules/gravity.c` : gravité et remplissage.
- `src/modules/save.c` : persistance simple.
