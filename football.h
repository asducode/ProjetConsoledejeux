#ifndef FOOTBALL_H_
#define FOOTBALL_H_

#include <stdint.h>
#include "stm32g4_utils.h"

#define SCREEN_WIDTH   320
#define SCREEN_HEIGHT  240
#define GOAL_LENGTH    60  // Longueur du but
#define PLAYER_WIDTH_FOOT   15
#define PLAYER_HEIGHT_FOOT  40
#define BALL_SIZE_FOOT      10
#define PLAYER_SPEED_FOOT   5
#define BALL_SPEED_FOOT     4

typedef enum {
    FOOT_GAME_WAIT_START,
    FOOT_GAME_RUNNING,
    FOOT_GAME_OVER
} gamefoot_state_t;

typedef struct {
    int16_t x, y;
    int16_t prev_x, prev_y;
    int16_t width, height;
    int16_t speed_x, speed_y;
} playerfoot_t;


typedef struct {
    int16_t x, y;
    int16_t speed_x, speed_y;
    int16_t size;
} ballfoot_t;

void FOOTBALL_init(void);
void FOOTBALL_process_main(void);


#endif /* FOOTBALL_H_ */
