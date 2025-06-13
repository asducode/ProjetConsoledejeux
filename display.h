/*
 * display.h
 *
 *  Created on: Feb 12, 2024
 *      Author: Nirgal
 */

#ifndef DISPLAY_H_
#define DISPLAY_H_


#include "pong.h"

void DISPLAY_init(void);
void DISPLAY_refresh_ball(ball_t *ball);
void DISPLAY_refresh_racket(racket_t *racket);
void DISPLAY_draw_middle_line(void);
void DISPLAY_show_scores(uint8_t score_p1, uint8_t score_p2);


#endif /* DISPLAY_H_ */
