#ifndef DISPLAYTIR_H_
#define DISPLAYTIR_H_

#include "tir.h"

void DISPLAY_TIR_init(void);
void DISPLAY_TIR_refresh_players(playertir_t *p1, playertir_t *p2);
void DISPLAY_TIR_refresh_bullets(bullet_t bullets_p1[], bullet_t bullets_p2[]);
void DISPLAY_TIR_draw_health(uint8_t health_p1, uint8_t health_p2);
void DISPLAY_TIR_show_winner(uint8_t winner);
void DISPLAY_TIR_erase_bullet(bullet_t *bullet);
#endif /* DISPLAYTIR_H_ */
