#include <stdbool.h>
#include <stdint-gcc.h>

#ifndef SNAKE
#define SNAKE

typedef enum {
    NONE = 0,
    UP = 1, DOWN = -1,
    LEFT = 2, RIGHT = -2
} snake_dir_t;

typedef struct snake_segment_t {
    struct snake_segment_t *prev;
    struct snake_segment_t *next;
    int x, prev_x;
    int y, prev_y;
    snake_dir_t dir, prev_dir;
} snake_segment_t;

snake_segment_t *snake_create_segment(snake_segment_t *prev, int x, int y, snake_dir_t dir);

void snake_set_dir(snake_segment_t *snake, snake_dir_t dir);

void snake_grow(snake_segment_t *snake);

void snake_update(snake_segment_t *snake);

void snake_draw(snake_segment_t *snake, void (*draw_ch)(uint16_t ch, int x, int y));

bool snake_check_collision(snake_segment_t *snake, int min_x, int min_y, int max_x, int max_y);

void snake_destroy(snake_segment_t *snake);

#endif