#ifndef LEVEL_H
#define LEVEL_H

typedef struct {
    int level;
    int moves;
    int time;
    int targetType;
    int targetCount;
    char text[128];
} Level;

Level getLevel(int lvl);

#endif
