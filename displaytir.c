#include <displaytir.h>

#include "TFT_ili9341/stm32g4_ili9341.h"
#include "TFT_ili9341/stm32g4_fonts.h"

void DISPLAY_TIR_init(void) {
    ILI9341_Init();
    ILI9341_Rotate(ILI9341_Orientation_Landscape_2);  // Ajouté pour s'assurer de la bonne orientation
    ILI9341_Fill(ILI9341_COLOR_BLACK);
}

void DISPLAY_TIR_refresh_players(playertir_t *p1, playertir_t *p2) {
    // Effacer les anciennes positions
    ILI9341_DrawFilledRectangle(p1->prev_x,
                                SCREEN_HEIGHT - p1->prev_y - PLAYER_HEIGHT_TIR,
                                p1->prev_x + PLAYER_WIDTH_TIR,
                                SCREEN_HEIGHT - p1->prev_y,
                                ILI9341_COLOR_BLACK);

    ILI9341_DrawFilledRectangle(p2->prev_x,
                                SCREEN_HEIGHT - p2->prev_y - PLAYER_HEIGHT_TIR,
                                p2->prev_x + PLAYER_WIDTH_TIR,
                                SCREEN_HEIGHT - p2->prev_y,
                                ILI9341_COLOR_BLACK);

    // Afficher les nouvelles positions
    ILI9341_DrawFilledRectangle(p1->x,
                                SCREEN_HEIGHT - p1->y - PLAYER_HEIGHT_TIR,
                                p1->x + PLAYER_WIDTH_TIR,
                                SCREEN_HEIGHT - p1->y,
                                ILI9341_COLOR_RED);

    ILI9341_DrawFilledRectangle(p2->x,
                                SCREEN_HEIGHT - p2->y - PLAYER_HEIGHT_TIR,
                                p2->x + PLAYER_WIDTH_TIR,
                                SCREEN_HEIGHT - p2->y,
                                ILI9341_COLOR_BLUE);

    // Mise à jour des anciennes positions
    p1->prev_x = p1->x;
    p1->prev_y = p1->y;
    p2->prev_x = p2->x;
    p2->prev_y = p2->y;
}

void DISPLAY_TIR_refresh_bullets(bullet_t bullets_p1[], bullet_t bullets_p2[]) {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bullets_p1[i].active) {
            ILI9341_DrawFilledRectangle(bullets_p1[i].prev_x,
                                        SCREEN_HEIGHT - bullets_p1[i].prev_y - 5,
                                        bullets_p1[i].prev_x + 5,
                                        SCREEN_HEIGHT - bullets_p1[i].prev_y,
                                        ILI9341_COLOR_BLACK);

            ILI9341_DrawFilledRectangle(bullets_p1[i].x,
                                        SCREEN_HEIGHT - bullets_p1[i].y - 5,
                                        bullets_p1[i].x + 5,
                                        SCREEN_HEIGHT - bullets_p1[i].y,
                                        ILI9341_COLOR_YELLOW);
        }

        if (bullets_p2[i].active) {
            ILI9341_DrawFilledRectangle(bullets_p2[i].prev_x,
                                        SCREEN_HEIGHT - bullets_p2[i].prev_y - 5,
                                        bullets_p2[i].prev_x + 5,
                                        SCREEN_HEIGHT - bullets_p2[i].prev_y,
                                        ILI9341_COLOR_BLACK);

            ILI9341_DrawFilledRectangle(bullets_p2[i].x,
                                        SCREEN_HEIGHT - bullets_p2[i].y - 5,
                                        bullets_p2[i].x + 5,
                                        SCREEN_HEIGHT - bullets_p2[i].y,
                                        ILI9341_COLOR_GREEN);
        }
    }
}

void DISPLAY_TIR_draw_health(uint8_t health_p1, uint8_t health_p2) {
    // Effacer ancienne zone HP
    ILI9341_DrawFilledRectangle(10, SCREEN_HEIGHT - 25, 80, SCREEN_HEIGHT - 5, ILI9341_COLOR_BLACK);
    ILI9341_DrawFilledRectangle(SCREEN_WIDTH - 90, SCREEN_HEIGHT - 25, SCREEN_WIDTH - 10, SCREEN_HEIGHT - 5, ILI9341_COLOR_BLACK);

    // Afficher le HP du joueur 1
    ILI9341_printf(10, SCREEN_HEIGHT - 25, &Font_11x18, ILI9341_COLOR_RED, ILI9341_COLOR_BLACK, "P1 HP: %u", health_p1);

    // HP joueur 2
    ILI9341_printf(SCREEN_WIDTH - 90, SCREEN_HEIGHT - 25, &Font_11x18, ILI9341_COLOR_BLUE, ILI9341_COLOR_BLACK, "P2 HP: %u", health_p2);
}

void DISPLAY_TIR_show_winner(uint8_t winner) {
    ILI9341_Fill(ILI9341_COLOR_BLACK);
    ILI9341_printf(SCREEN_WIDTH / 2 - 60, SCREEN_HEIGHT / 2 - 10, &Font_11x18, ILI9341_COLOR_YELLOW, ILI9341_COLOR_BLACK, "Player %u Wins!", winner);
}

void DISPLAY_TIR_erase_bullet(bullet_t *bullet) {
    ILI9341_DrawFilledRectangle(bullet->x,
                                SCREEN_HEIGHT - bullet->y - 5,
                                bullet->x + 5,
                                SCREEN_HEIGHT - bullet->y,
                                ILI9341_COLOR_BLACK);
}
