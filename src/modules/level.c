#include <stdio.h>
#include "level.h"
#include "types.h"

Level getLevel(int lvl) {
    Level L;

    L.level = lvl;
    L.time = 90;
    L.targetType = ITEM_O;
    L.targetCount = 10;
    L.moves = 40;

    sprintf(L.text, "Collecte %d 'O'", L.targetCount);

    return L;
}
