#include <displayfoot.h>
#include "football.h"
#include "TFT_ili9341/stm32g4_ili9341.h"
#include "TFT_ili9341/stm32g4_fonts.h"

void DISPLAYFOOT_init(void) {
    ILI9341_Init();
    DISPLAYFOOT_show_field();
}

void DISPLAYFOOT_show_field(void) {
    ILI9341_Fill(ILI9341_COLOR_GREEN);

    // Ligne centrale
    ILI9341_DrawLine(SCREEN_WIDTH / 2, 0, SCREEN_WIDTH / 2, SCREEN_HEIGHT, ILI9341_COLOR_WHITE);

    // Cercle central
    ILI9341_DrawCircle(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 30, ILI9341_COLOR_WHITE);

    // Buts
    ILI9341_DrawRectangle(0, SCREEN_HEIGHT / 2 - GOAL_LENGTH / 2, 5, SCREEN_HEIGHT / 2 + GOAL_LENGTH / 2, ILI9341_COLOR_RED);
    ILI9341_DrawRectangle(SCREEN_WIDTH - 5, SCREEN_HEIGHT / 2 - GOAL_LENGTH / 2, SCREEN_WIDTH, SCREEN_HEIGHT / 2 + GOAL_LENGTH / 2, ILI9341_COLOR_BLUE);
}

void DISPLAYFOOT_draw_scores(uint8_t score_p1, uint8_t score_p2) {
    // Effacer l'ancienne zone des scores
    ILI9341_DrawFilledRectangle(5, SCREEN_HEIGHT - 30, 60, SCREEN_HEIGHT - 10, ILI9341_COLOR_WHITE);
    ILI9341_DrawFilledRectangle(SCREEN_WIDTH - 70, SCREEN_HEIGHT - 30, SCREEN_WIDTH - 10, SCREEN_HEIGHT - 10, ILI9341_COLOR_WHITE);

    // Afficher le score du joueur 1 (en bas à gauche)
    ILI9341_printf(5, SCREEN_HEIGHT - 30, &Font_11x18, ILI9341_COLOR_RED, ILI9341_COLOR_WHITE, " %u", score_p1);

    // Afficher le score du joueur 2 (en bas à droite)
    ILI9341_printf(SCREEN_WIDTH - 70, SCREEN_HEIGHT - 30, &Font_11x18, ILI9341_COLOR_BLUE, ILI9341_COLOR_WHITE, " %u", score_p2);
}




void DISPLAYFOOT_refresh_ball(ballfoot_t *ball) {
    // Effacer l’ancienne position de la balle
    ILI9341_DrawFilledCircle(ball->x, ball->y, BALL_SIZE_FOOT / 2, ILI9341_COLOR_GREEN);

    // Redessiner la balle à sa nouvelle position
    ILI9341_DrawFilledCircle(ball->x, ball->y, BALL_SIZE_FOOT / 2, ILI9341_COLOR_YELLOW);
}



void DISPLAYFOOT_refresh_players(playerfoot_t *p1, playerfoot_t *p2) {
    // Effacement des anciennes positions
    ILI9341_DrawFilledRectangle(p1->prev_x, p1->prev_y,
                                p1->prev_x + PLAYER_WIDTH_FOOT, p1->prev_y + PLAYER_HEIGHT_FOOT, ILI9341_COLOR_GREEN);
    ILI9341_DrawFilledRectangle(p2->prev_x, p2->prev_y,
                                p2->prev_x + PLAYER_WIDTH_FOOT, p2->prev_y + PLAYER_HEIGHT_FOOT, ILI9341_COLOR_GREEN);

    // Dessiner les nouvelles positions
    ILI9341_DrawFilledRectangle(p1->x, p1->y, p1->x + PLAYER_WIDTH_FOOT, p1->y + PLAYER_HEIGHT_FOOT, ILI9341_COLOR_RED);
    ILI9341_DrawFilledRectangle(p2->x, p2->y, p2->x + PLAYER_WIDTH_FOOT, p2->y + PLAYER_HEIGHT_FOOT, ILI9341_COLOR_BLUE);

    // Mise à jour des positions précédentes
    p1->prev_x = p1->x;
    p1->prev_y = p1->y;
    p2->prev_x = p2->x;
    p2->prev_y = p2->y;
}



void DISPLAYFOOT_show_winner(uint8_t winner) {
    ILI9341_Fill(ILI9341_COLOR_BLACK);
    ILI9341_printf(SCREEN_WIDTH / 2 - 60, SCREEN_HEIGHT / 2 - 10, &Font_11x18, ILI9341_COLOR_YELLOW, ILI9341_COLOR_BLACK, "Player %u Wins!", winner);
}
