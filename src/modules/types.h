#ifndef TYPES_H
#define TYPES_H
#define ROWS 20
#define COLS 10


typedef enum {
    ITEM_EMPTY = -1,
    ITEM_O = 0,
    ITEM_X = 1,
    ITEM_Q = 2,  // '?'
    ITEM_P = 3,  // '%'

    ITEM_BOMB = 10,
    ITEM_DISCO = 11,
    ITEM_ARROW_H = 12,
    ITEM_ARROW_V = 13

} ItemType;

typedef enum {
    SPECIAL_NONE = 0,
    SPECIAL_LINE = 1, // supprime une ligne
    SPECIAL_BOMB = 2  // supprime un carré 3x3
} SpecialType;

typedef struct {
    int type;
    int special; // SpecialType
} Item;

#endif
