#ifndef DISPLAYFOOT_H_
#define DISPLAYFOOT_H_

#include <stdint.h>
#include "football.h"
//#include "stm32g4_ili9341.h"

void DISPLAYFOOT_init(void);
void DISPLAYFOOT_show_field(void);
void DISPLAYFOOT_draw_scores(uint8_t score_p1, uint8_t score_p2);
void DISPLAYFOOT_refresh_ball(ballfoot_t *ball);
void DISPLAYFOOT_refresh_players(playerfoot_t *p1, playerfoot_t *p2);
void DISPLAYFOOT_show_winner(uint8_t winner);

#endif /* DISPLAYFOOT_H_ */
