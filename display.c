

#include "stm32g4xx_hal.h"
#include "pong.h"
#include "display.h"
#include "TFT_ili9341/stm32g4_ili9341.h"
#include "TFT_ili9341/stm32g4_fonts.h"
#include "stm32g4_systick.h"

extern const uint16_t ball_colors[BALL_OPTION_NB] = {
    ILI9341_COLOR_RED,     // BALL_OPTION_NONE
    ILI9341_COLOR_BLUE,    // BALL_OPTION_GLUE
    ILI9341_COLOR_ORANGE   // BALL_OPTION_JOKER
};

void DISPLAY_init(void) {
    ILI9341_Init();
    ILI9341_Fill(ILI9341_COLOR_WHITE);
    ILI9341_Rotate(ILI9341_Orientation_Landscape_2);
}

void DISPLAY_refresh_ball(ball_t *ball) {
    static ball_t displayed_ball;
    static ball_options_e displayed_ball_valid = false;

    if (displayed_ball.x != ball->x || displayed_ball.y != ball->y ||
        displayed_ball.size != ball->size || displayed_ball.options != ball->options) {

        if (displayed_ball_valid) {
            ILI9341_DrawCircle(displayed_ball.x, ILI9341_WIDTH - displayed_ball.y, displayed_ball.size / 2, ILI9341_COLOR_WHITE);
        }

        ILI9341_DrawCircle(ball->x, ILI9341_WIDTH - ball->y, ball->size / 2, ball_colors[ball->options]);
        displayed_ball = *ball;
        displayed_ball_valid = true;
    }
}

void DISPLAY_refresh_racket(racket_t* racket) {
    static racket_t displayed_r1 = {0};
    static racket_t displayed_r2 = {0};
    static bool valid_r1 = false;
    static bool valid_r2 = false;

    racket_t* cached;
    bool* valid;

    if (racket->x < SCREEN_WIDTH / 2) {
        cached = &displayed_r1;
        valid = &valid_r1;
    } else {
        cached = &displayed_r2;
        valid = &valid_r2;
    }

    if (!*valid || racket->x != cached->x || racket->y != cached->y || racket->height != cached->height) {
        if (*valid) {
            ILI9341_DrawFilledRectangle(
                cached->left,
                ILI9341_WIDTH - (cached->y + cached->height / 2),
                cached->left + cached->width - 1,
                ILI9341_WIDTH - (cached->y - cached->height / 2) - 1,
                ILI9341_COLOR_WHITE
            );
        }

        ILI9341_DrawFilledRectangle(
            racket->left,
            ILI9341_WIDTH - (racket->y + racket->height / 2),
            racket->left + racket->width - 1,
            ILI9341_WIDTH - (racket->y - racket->height / 2) - 1,
            ILI9341_COLOR_BLACK
        );

        *cached = *racket;
        *valid = true;
    }
}

void DISPLAY_draw_middle_line(void) {
    for (int y = 0; y < SCREEN_HEIGHT; y += 10) {
        ILI9341_DrawLine(SCREEN_WIDTH / 2, y, SCREEN_WIDTH / 2, y + 5, ILI9341_COLOR_BLACK);
    }
}

void DISPLAY_show_scores(uint8_t score_p1, uint8_t score_p2) {
    // Joueur 1 (gauche)
    ILI9341_DrawFilledRectangle(10, 10, 10 + 30 - 1, 10 + 20 - 1, ILI9341_COLOR_WHITE);
    ILI9341_printf(10, 10, &Font_11x18, ILI9341_COLOR_RED, ILI9341_COLOR_WHITE, "%u", score_p1);

    // Joueur 2 (droite)
    ILI9341_DrawFilledRectangle(SCREEN_WIDTH - 40, 10, SCREEN_WIDTH - 11, 10 + 20 - 1, ILI9341_COLOR_WHITE);
    ILI9341_printf(SCREEN_WIDTH - 40, 10, &Font_11x18, ILI9341_COLOR_BLUE, ILI9341_COLOR_WHITE, "%u", score_p2);
}
