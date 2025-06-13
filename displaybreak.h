/*
 * display.h
 *
 *  Created on: Feb 12, 2024
 *      Author: Nirgal
 */

#ifndef DISPLAY_H_
#define DISPLAY_H_


#include "breakbricks.h"

void DISPLAYBREAK_init(void);
void DISPLAYBREAK_refresh_ball(ballbreak_t *ball);
void DISPLAYBREAK_refresh_life(int8_t life);

void DISPLAYBREAK_refresh_racket(racketbreak_t *racket);
void DISPLAYBREAK_refresh_grid(grid_t * grid);


#endif /* DISPLAY_H_ */
