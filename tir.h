#ifndef TIR_H_
#define TIR_H_

#include <stdint.h>

#define SCREEN_WIDTH   320
#define SCREEN_HEIGHT  240
#define PLAYER_WIDTH_TIR   15
#define PLAYER_HEIGHT_TIR  25
#define BULLET_SPEED   8
#define PLAYER_SPEED   5
#define MAX_BULLETS    5

typedef enum {
    TIR_GAME_WAIT_START,
    TIR_GAME_RUNNING,
    TIR_GAME_OVER
} gametir_state_t;

typedef struct {
    int16_t x, y;
    int16_t prev_x, prev_y; // Ajout des anciennes positions
    uint8_t health;
} playertir_t;

typedef struct {
    int16_t x, y;
    int16_t prev_x, prev_y; // Ajout des anciennes positions
    int16_t speed_x;
    uint8_t active;
} bullet_t;

void TIR_init(void);
void TIR_process_main(void);
void TIR_shoot(playertir_t *player);
void TIR_check_collisions(void);

#endif /* TIR_H_ */
