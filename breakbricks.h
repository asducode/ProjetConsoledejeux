/*
 * breakbricks.h
*
 */

#ifndef BREAKBRICKS_H_
#define BREAKBRICKS_H_

#include <stdint.h>
#include "stm32g4_utils.h"
#include "stm32g4xx_hal.h"

#define NB_BRICKS_LATERAL	6
#define NB_BRICKS_VERTICAL	4
#define NB_MAX_BRICKS (NB_BRICKS_VERTICAL*NB_BRICKS_LATERAL)

#define SCREEN_WIDTH	320
#define SCREEN_HEIGHT	240
#define MARGIN_DEFAULT 2
#define BRICK_WIDTH_DEFAULT ((SCREEN_WIDTH-(NB_BRICKS_LATERAL+1)*MARGIN_DEFAULT)/NB_BRICKS_LATERAL)
#define BRICK_HEIGHT_DEFAULT (32)



typedef enum {
	BREAK_BALL_OPTION_NONE ,
	BREAK_BALL_OPTION_JOKER ,
	BREAK_BALL_OPTION_GLUE ,
	BREAK_BALL_OPTION_NB,

}ballbreak_options_e;

typedef struct {
	int16_t x;
	int16_t y;
	int16_t fine_x;
	int16_t fine_y;
	int16_t speed_x;
	int16_t speed_y;
	int16_t size;
	ballbreak_options_e options;
	int8_t joker;
}ballbreak_t;

typedef struct {
	int16_t x;
	int16_t y;
	int16_t left;
	int16_t right;
	int16_t width;
	int16_t height;
	int16_t speed;
}racketbreak_t;

typedef enum {
	BRICKSTYLE_NONE,
	BRICKSTYLE_FULL,
	BRICKSTYLE_ROCK,
	BRICKSTYLE_BOMB,
	BRICKSTYLE_RESIZER,
	BRICKSTYLE_JOKER,
	BRICKSTYLE_GLUE,
	BRICKSTYLE_LIFE,
	BRICKSTYLE_NB,

}brickstyle_e;

typedef struct {
	brickstyle_e style;
	int16_t x1;
	int16_t x2;
	int16_t y1;
	int16_t y2;
}brick_t;

typedef struct {
	brick_t bricks[NB_MAX_BRICKS];
	uint8_t nb_bricks_remaining;
}grid_t;

void BREAKBRICKS_init(void);

void BREAKBRICKS_process_main(void);


#endif /* BREAKBRICKS_H_ */
