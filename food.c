#include <stdlib.h>
#include "food.h"

food_t *food_init(int min_x, int min_y, int max_x, int max_y) {
    food_t *food = (food_t *) malloc(sizeof(food_t));
    food_reposition(food, min_x, min_y, max_x, max_y);
    food->reward = 10;
    return food;
}

void food_reposition(food_t *food, int min_x, int min_y, int max_x, int max_y) {
    food->x = min_x + 1 + rand() % (max_x - min_x - 2);
    food->y = min_y + 1 + rand() % (max_y - min_y - 2);
}

void food_draw(food_t *food, void (*draw_ch)(uint16_t ch, int x, int y)) {
    draw_ch('*', food->x, food->y);
}

void food_destroy(food_t *food) {
    free(food);
}