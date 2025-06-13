#ifndef DISPLAYSNAKE_H
#define DISPLAYSNAKE_H

#include <stdint.h>
#include "snake.h"

#define SCREEN_WIDTH  240
#define SCREEN_HEIGHT 320
#define CELL_SIZE 10
#define GRID_WIDTH  (SCREEN_WIDTH / CELL_SIZE)
#define GRID_HEIGHT (SCREEN_HEIGHT / CELL_SIZE)

void DISPLAYSNAKE_init(void);
void DISPLAYSNAKE_clear(void);
void DISPLAYSNAKE_draw_scores(uint8_t score1, uint8_t score2);
void DISPLAYSNAKE_refresh_snake(snake_t* snake, int player_id);
void DISPLAYSNAKE_refresh_apple(apple_t* apple);
void DISPLAYSNAKE_erase_snake(snake_t* snake);

#endif
