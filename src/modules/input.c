#include <conio.h>
#include "input.h"

int readKey() {
    if (_kbhit()) return _getch();
    return -1;
}

int isArrowKey(int key) {
    return (key == 224); // touches spéciales
}

void getArrowDirection(int key, int *dx, int *dy) {
    int k = _getch();
    *dx = *dy = 0;

    if (k == 72) *dx = -1; // up
    if (k == 80) *dx = 1;  // down
    if (k == 75) *dy = -1; // left
    if (k == 77) *dy = 1;  // right
}
