/* PONG.h */

#ifndef PONG_H_
#define PONG_H_

#include <stdint.h>
#include "stm32g4_utils.h"
#include "stm32g4xx_hal.h"

#define SCREEN_WIDTH	320
#define SCREEN_HEIGHT	240
#define MARGIN_DEFAULT 2
#define RACKET_SPEED 15
#define REFRESH_PERIOD_MS  30
#define RACKET_WIDTH 4
#define RACKET_HEIGHT 35


typedef enum {
    GAME_WAIT_START,
    GAME_RUNNING,
    GAME_OVER
} game_state_t;

typedef struct {
	int16_t x;
	int16_t y;
	int16_t left;
	int16_t right;
	int16_t width;
	int16_t height;
	int16_t speed;
}racket_t;

typedef enum {
	BALL_OPTION_NONE ,
	BALL_OPTION_JOKER ,
	BALL_OPTION_GLUE ,
	BALL_OPTION_NB,

}ball_options_e;

typedef struct {
	int16_t x;
	int16_t y;
	int16_t fine_x;
	int16_t fine_y;
	int16_t speed_x;
	int16_t speed_y;
	int16_t size;
	int8_t joker;
	ball_options_e options;
}ball_t;

void PONG_init(void);

void PONG_process_main(void);

void DISPLAY_show_winner(uint8_t winner);


#endif /* PONG_H_ */
