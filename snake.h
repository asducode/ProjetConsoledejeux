#ifndef SNAKE_H
#define SNAKE_H

#include <stdint.h>
#include <stdbool.h>

#define MAX_SNAKE_LENGTH 1000

typedef enum { DIR_UP, DIR_DOWN, DIR_LEFT, DIR_RIGHT } directionsnake_t;

typedef struct {
    uint8_t x, y;
} positionsnake_t;

typedef struct {
    positionsnake_t body[MAX_SNAKE_LENGTH];
    uint8_t length;
    directionsnake_t dir;
} snake_t;

typedef struct {
    uint8_t x, y;
} apple_t;

void snake_init(void);
void snake_update(void);

#endif
