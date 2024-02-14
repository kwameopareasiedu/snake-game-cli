#ifndef FOOD
#define FOOD

#include <stdint-gcc.h>

typedef struct {
    int x;
    int y;
    int reward;
} food_t;

food_t *food_init(int min_x, int min_y, int max_x, int max_y);

void food_reposition(food_t *food, int min_x, int min_y, int max_x, int max_y);

void food_draw(food_t *food, void (*draw_ch)(uint16_t ch, int x, int y));

void food_destroy(food_t *food);

#endif