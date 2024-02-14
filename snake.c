#include <malloc.h>
#include "snake.h"

snake_segment_t *snake_create_segment(snake_segment_t *prev, int x, int y, snake_dir_t dir) {
    snake_segment_t *seg = (snake_segment_t *) malloc(sizeof(snake_segment_t));
    seg->dir = seg->prev_dir = dir;
    seg->x = seg->prev_x = x;
    seg->y = seg->prev_y = y;
    seg->prev = prev;
    seg->next = NULL;
    return seg;
}

void snake_set_dir(snake_segment_t *snake, snake_dir_t dir) {
    if (dir != NONE && dir != -snake->dir) snake->dir = dir;
}

void snake_grow(snake_segment_t *snake) {
    snake_segment_t *last_segment = snake;

    while (last_segment->next != NULL) {
        last_segment = last_segment->next;
    }

    snake_dir_t seg_dir = last_segment->dir;
    int x_offset = seg_dir == RIGHT ? -1 : seg_dir == LEFT ? 1 : 0;
    int y_offset = seg_dir == UP ? -1 : seg_dir == DOWN ? 1 : 0;
    int new_x = last_segment->x + x_offset;
    int new_y = last_segment->y + y_offset;

    last_segment->next = snake_create_segment(last_segment, new_x, new_y, last_segment->dir);
}

void snake_update(snake_segment_t *snake) {
    snake_segment_t *segment = snake;

    while (segment != NULL) {
        segment->prev_dir = segment->dir;
        segment->prev_x = segment->x;
        segment->prev_y = segment->y;

        if (segment->prev == NULL) {
            snake_dir_t dir = segment->dir;
            int dx = dir == LEFT ? -1 : dir == RIGHT ? 1 : 0;
            int dy = dir == UP ? -1 : dir == DOWN ? 1 : 0;

            segment->x += dx;
            segment->y += dy;
        } else {
            segment->x = segment->prev->prev_x;
            segment->y = segment->prev->prev_y;
            segment->dir = segment->prev->prev_dir;
        }

        segment = segment->next;
    }
}

char snake_segment_char(bool is_head, snake_dir_t dir) {
    switch (dir) {
        case UP:
            return is_head ? '^' : 'o';
        case LEFT:
            return is_head ? '<' : 'o';
        case RIGHT:
            return is_head ? '>' : 'o';
        case DOWN:
            return is_head ? u'v' : 'o';
        default:
            return 0;
    }
}

void snake_draw(snake_segment_t *snake, void (*draw_ch)(uint16_t ch, int x, int y)) {
    snake_segment_t *segment = snake;

    while (segment != NULL) {
        char c = snake_segment_char(segment->prev == NULL, segment->dir);
        draw_ch(c, segment->x, segment->y);
        segment = segment->next;
    }
}

bool snake_check_collision(snake_segment_t *snake, int min_x, int min_y, int max_x, int max_y) {
    int hx = snake->x, hy = snake->y;
    snake_segment_t *segment = snake->next;

    while (segment != NULL) {
        if (segment->x == hx && segment->y == hy)
            // Collided with self
            return true;
        segment = segment->next;
    }

    if (hx <= min_x || hx >= max_x || hy <= min_y || hy >= max_y)
        // Collided with bounds
        return true;

    return false;
}

void snake_destroy(snake_segment_t *snake) {
    snake_segment_t *segment = snake;

    do {
        snake_segment_t *next = segment->next;
        free(segment);
        segment = next;
    } while (segment != NULL);
}