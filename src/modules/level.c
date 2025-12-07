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
    L.time = 90;   // temps par niveau
    L.moves = 40;  // coups par niveau

    // Définition de l'objectif selon le niveau
    switch (lvl) {
        case 1:
            L.targetType = ITEM_O;  // Collecte des O
            L.targetCount = 10;
            break;

        case 2:
            L.targetType = ITEM_X;  // Collecte des X
            L.targetCount = 12;
            break;

        case 3:
            L.targetType = ITEM_Q;  // Collecte des ?
            L.targetCount = 15;
            break;

        case 4:
            L.targetType = ITEM_P;  // Collecte des %
            L.targetCount = 18;
            break;

        default:
            // Pour les niveaux supérieurs : cycle O → X → ? → %
            L.targetType = ITEM_O + (lvl % 4);
            L.targetCount = 10 + lvl * 2;
            break;
    }

    // Remplacement de l’ancien texte qui utilisait O+offset (faux)
    // Maintenant : symbole correct selon le type
    sprintf(L.text, "Collecte %d '%c'", L.targetCount, getSymbolFromType(L.targetType));

    return L;
}
