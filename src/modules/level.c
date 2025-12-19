#include <stdio.h>
#include "level.h"
#include "types.h"

// Fonction utilitaire : transforme un type en symbole à afficher
char getSymbolFromType(int type) {
    switch (type) {
        case ITEM_O: return 'O';
        case ITEM_X: return 'X';
        case ITEM_Q: return '?';
        case ITEM_P: return '%';
        default: return '?';
    }
}

Level getLevel(int lvl) {
    Level L;

    L.level = lvl;
    

    // Définition de l'objectif selon le niveau
    switch (lvl) {
        case 1:
            L.targetType = ITEM_O;  // Collecte des O
            L.targetCount = 20;
            L.time = 90;   // temps par niveau
            L.moves = 40;  // coups par niveau
            break;

        case 2:
            L.targetType = ITEM_X;  // Collecte des X
            L.targetCount = 25;
            L.time = 75;   // temps par niveau
            L.moves = 30;  // coups par niveau
            break;

        case 3:
            L.targetType = ITEM_Q;  // Collecte des ?
            L.targetCount = 30;
            L.time = 60;   // temps par niveau
            L.moves = 20;  // coups par niveau
            break;

        case 4:
            L.targetType = ITEM_P;  // Collecte des %
            L.targetCount = 40;
            L.time = 45;   // temps par niveau
            L.moves = 10;  // coups par niveau
            break;

        default:
            // Pour les niveaux supérieurs : cycle O → X → ? → %
            L.targetType = ITEM_O + (lvl % 4);
            L.targetCount = 40 + lvl * 2;
            L.time = 45;   // temps par niveau
            L.moves = 10;  // coups par niveau
            break;
    }

    // Remplacement de l’ancien texte qui utilisait O+offset (faux)
    // Maintenant : symbole correct selon le type
    sprintf(L.text, "Collecte %d '%c'", L.targetCount, getSymbolFromType(L.targetType));

    return L;
}
