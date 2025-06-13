#include <displayfoot.h>
#include "football.h"
#include "button.h"
#include <stdlib.h>
#include "stm32g4_uart.h"
#include "TFT_ili9341/stm32g4_ili9341.h"
#include "TFT_ili9341/stm32g4_fonts.h"

static playerfoot_t p1_foot, p2_foot;
static ballfoot_t ballfoot;
static uint8_t score_p1_foot = 0, score_p2_foot = 0;
static gamefoot_state_t game_state_foot = FOOT_GAME_WAIT_START;

static void FOOTBALL_update_positions(void);
static void FOOTBALL_check_collisions(void);
static void FOOTBALL_reset_ball(uint8_t player_scored);
static void FOOTBALL_reset_positions(void);
static void FOOTBALL_check_goal(void);

void FOOTBALL_init(void) {
    BUTTONS_init();
    DISPLAYFOOT_init();
    score_p1_foot = 0;
    score_p2_foot = 0;
    game_state_foot = FOOT_GAME_WAIT_START;
    FOOTBALL_reset_positions();
    DISPLAYFOOT_show_field();
    DISPLAYFOOT_draw_scores(score_p1_foot, score_p2_foot);
}

void FOOTBALL_process_main(void) {
    if (game_state_foot == FOOT_GAME_WAIT_START && BUTTON_center_read()) {
        score_p1_foot = 0;
        score_p2_foot = 0;
        game_state_foot = FOOT_GAME_RUNNING;
        FOOTBALL_reset_positions();
    }

    if (game_state_foot != FOOT_GAME_RUNNING) return;

    FOOTBALL_update_positions();
    FOOTBALL_check_collisions();
    FOOTBALL_check_goal();
    DISPLAYFOOT_refresh_ball(&ballfoot);
    DISPLAYFOOT_refresh_players(&p1_foot, &p2_foot);
    DISPLAYFOOT_draw_scores(score_p1_foot, score_p2_foot);
}

static void FOOTBALL_update_positions(void) {
    bool j1u = 0, j1d = 0, j1l = 0, j1r = 0;
    bool j2h = 0, j2b = 0, j2g = 0, j2e = 0;

    // Lecture des commandes UART
    if (BSP_UART_data_ready(UART1_ID)) {
        uint8_t received_char = BSP_UART_get_next_byte(UART1_ID);
        switch (received_char) {
            case 'u': j1u = 1; break;
            case 'd': j1d = 1; break;
            case 'l': j1l = 1; break;
            case 'r': j1r = 1; break;
        }
    }

    if (BSP_UART_data_ready(UART2_ID)) {
        uint8_t received_char = BSP_UART_get_next_byte(UART2_ID);
        switch (received_char) {
            case 'h': j2h = 1; break;
            case 'b': j2b = 1; break;
            case 'g': j2g = 1; break;
            case 'e': j2e = 1; break;
        }
    }

    // Mise à jour des positions des joueurs avec contraintes
    p1_foot.x += (j1l ? -PLAYER_SPEED_FOOT : j1r ? PLAYER_SPEED_FOOT : 0);
    p1_foot.y += (j1u ? -PLAYER_SPEED_FOOT : j1d ? PLAYER_SPEED_FOOT : 0);

    p2_foot.x += (j2g ? -PLAYER_SPEED_FOOT : j2e ? PLAYER_SPEED_FOOT : 0);
    p2_foot.y += (j2h ? -PLAYER_SPEED_FOOT : j2b ? PLAYER_SPEED_FOOT : 0);
}

static void FOOTBALL_check_collisions(void) {
    // Collision avec les joueurs
    if ((ballfoot.x + BALL_SIZE_FOOT >= p1_foot.x &&
         ballfoot.x <= p1_foot.x + PLAYER_WIDTH_FOOT &&
         ballfoot.y + BALL_SIZE_FOOT >= p1_foot.y &&
         ballfoot.y <= p1_foot.y + PLAYER_HEIGHT_FOOT) ||
        (ballfoot.x + BALL_SIZE_FOOT >= p2_foot.x &&
         ballfoot.x <= p2_foot.x + PLAYER_WIDTH_FOOT &&
         ballfoot.y + BALL_SIZE_FOOT >= p2_foot.y &&
         ballfoot.y <= p2_foot.y + PLAYER_HEIGHT_FOOT)) {

        ballfoot.speed_x = -ballfoot.speed_x;
        ballfoot.speed_y = -ballfoot.speed_y;
    }
}

static void FOOTBALL_check_goal(void) {
    if (ballfoot.x <= 0) {
        score_p2_foot++;
        FOOTBALL_reset_ball(2);
    } else if (ballfoot.x >= SCREEN_WIDTH - BALL_SIZE_FOOT) {
        score_p1_foot++;
        FOOTBALL_reset_ball(1);
    }
}

static void FOOTBALL_reset_ball(uint8_t player_scored) {
    ballfoot.x = SCREEN_WIDTH / 2;
    ballfoot.y = SCREEN_HEIGHT / 2;
    ballfoot.speed_x = (rand() % 2 ? 1 : -1) * BALL_SPEED_FOOT;
    ballfoot.speed_y = (rand() % 2 ? 1 : -1) * BALL_SPEED_FOOT;
}

static void FOOTBALL_reset_positions(void) {
    p1_foot.x = GOAL_LENGTH;
    p1_foot.y = SCREEN_HEIGHT / 2;
    p2_foot.x = SCREEN_WIDTH - GOAL_LENGTH - PLAYER_WIDTH_FOOT;
    p2_foot.y = SCREEN_HEIGHT / 2;
    ballfoot.x = SCREEN_WIDTH / 2;
    ballfoot.y = SCREEN_HEIGHT / 2;
    ballfoot.speed_x = BALL_SPEED_FOOT;
    ballfoot.speed_y = BALL_SPEED_FOOT;
}
