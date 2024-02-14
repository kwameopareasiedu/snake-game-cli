#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include "snake.h"
#include "food.h"

#define DEBUG_MODE true

snake_dir_t get_key_dir(int key);

void draw_ch(uint16_t ch, int x, int y);

void draw_bounds(int min_x, int min_y, int max_x, int max_y);

void draw_stats(int pos_x, int pos_y, int score);

void draw_credits(int pos_x, int pos_y);

void draw_debug_info(int pos_x, int pos_y, snake_segment_t *snake, food_t *food, int iter_count);

int main() {
    srand(time(NULL));

    WINDOW *win = initscr();
    cbreak();
    nodelay(win, true);
    keypad(win, true);
    curs_set(0);

    int score = 0;
    int iter_count = 0;
    int min_bounds_x = 0, max_bounds_x = 50;
    int min_bounds_y = 0, max_bounds_y = 25;
    snake_segment_t *snake = snake_create_segment(NULL, 10, 10, RIGHT);
    food_t *food = food_init(min_bounds_x, min_bounds_y, max_bounds_x, max_bounds_y);

    while (true) {
        iter_count++;
        int pressed_key = wgetch(win);
        snake_dir_t dir = get_key_dir(pressed_key);


        snake_set_dir(snake, dir);
        snake_update(snake);

        if (snake_check_collision(snake, min_bounds_x, min_bounds_y, max_bounds_x, max_bounds_y))
            break;

        if (snake->x == food->x && snake->y == food->y) {
            food_reposition(food, min_bounds_x, min_bounds_y, max_bounds_x, max_bounds_y);
            snake_grow(snake);
            score += food->reward;
        }

        erase();
        draw_bounds(min_bounds_x, min_bounds_y, max_bounds_x, max_bounds_y);
        snake_draw(snake, draw_ch);
        food_draw(food, draw_ch);
        draw_stats(max_bounds_x + 4, 0, score);
        draw_credits(max_bounds_x + 4, max_bounds_y);

        if (DEBUG_MODE) draw_debug_info(0, max_bounds_y + 4, snake, food, iter_count);

        usleep(75000);
    }

    endwin();
    snake_destroy(snake);
    food_destroy(food);

    printf("Game over!\nScore is %d\n", score);
    return 0;
}

snake_dir_t get_key_dir(int key) {
    switch (key) {
        case KEY_UP:
            return UP;
        case KEY_LEFT:
            return LEFT;
        case KEY_RIGHT:
            return RIGHT;
        case KEY_DOWN:
            return DOWN;
        default:
            return NONE;
    }
}

void draw_ch(uint16_t ch, int x, int y) {
    mvaddch(y, x, ch);
}

void draw_bounds(int min_x, int min_y, int max_x, int max_y) {
    for (int x = min_x; x <= max_x; x++) {
        mvaddch(0, x, '#');
        mvaddch(max_y, x, '#');
    }

    for (int y = min_y; y <= max_y; y++) {
        mvaddch(y, 0, '#');
        mvaddch(y, max_x, '#');
    }
}

void draw_stats(int pos_x, int pos_y, int score) {
    char score_str[16];
    sprintf(score_str, "Score: %d", score);
    mvaddstr(pos_y, pos_x, score_str);
}

void draw_credits(int pos_x, int pos_y) {
    mvaddstr(pos_y - 2, pos_x, "Author: Kwame Opare Asiedu (https://github.com/kwameopareasiedu)");
    mvaddstr(pos_y - 1, pos_x, "Tools:  C, Cmake, Ncurses");
}

void draw_debug_info(int pos_x, int pos_y, snake_segment_t *snake, food_t *food, int iter_count) {
    char snake_pos_str[32], food_pos_str[32], iter_count_str[16];
    sprintf(snake_pos_str, "Snake pos: (%d,%d)", snake->x, snake->y);
    sprintf(food_pos_str, "Food pos: (%d,%d)", food->x, food->y);
    sprintf(iter_count_str, "Iterations: %d", iter_count);

    mvaddstr(pos_y, pos_x, "Debug");
    mvaddstr(pos_y + 1, pos_x, "*****");
    mvaddstr(pos_y + 2, pos_x, snake_pos_str);
    mvaddstr(pos_y + 3, pos_x, food_pos_str);
    mvaddstr(pos_y + 4, pos_x, iter_count_str);
}