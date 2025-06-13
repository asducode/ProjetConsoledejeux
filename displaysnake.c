#include <displaysnake.h>
#include "TFT_ili9341/stm32g4_ili9341.h"
#include "TFT_ili9341/stm32g4_fonts.h"

void DISPLAYSNAKE_init(void) {
    ILI9341_Init();
    ILI9341_Rotate(ILI9341_Orientation_Landscape_2);
    DISPLAYSNAKE_clear();
}

void DISPLAYSNAKE_clear(void) {
    ILI9341_Fill(ILI9341_COLOR_WHITE);
}

void DISPLAYSNAKE_draw_scores(uint8_t score1, uint8_t score2) {
    ILI9341_printf(10, 10, &Font_11x18, ILI9341_COLOR_BLUE, ILI9341_COLOR_WHITE, "P1: %d", score1);
    ILI9341_printf(SCREEN_WIDTH - 80, 10, &Font_11x18, ILI9341_COLOR_RED, ILI9341_COLOR_WHITE, "P2: %d", score2);
}

void DISPLAYSNAKE_refresh_snake(snake_t* snake, int player_id) {
    uint16_t color = (player_id == 0) ? ILI9341_COLOR_BLUE : ILI9341_COLOR_RED;
    for (int i = 0; i < snake->length; ++i) {
        ILI9341_DrawFilledRectangle(
            snake->body[i].x * CELL_SIZE,
            snake->body[i].y * CELL_SIZE,
            (snake->body[i].x + 1) * CELL_SIZE - 1,
            (snake->body[i].y + 1) * CELL_SIZE - 1,
            color);
    }
}

 void DISPLAYSNAKE_erase_snake(snake_t* snake) {
    ILI9341_DrawFilledRectangle(
        snake->body[snake->length - 1].x * CELL_SIZE,
        snake->body[snake->length - 1].y * CELL_SIZE,
        (snake->body[snake->length - 1].x + 1) * CELL_SIZE - 1,
        (snake->body[snake->length - 1].y + 1) * CELL_SIZE - 1,
        ILI9341_COLOR_WHITE);
}



void DISPLAYSNAKE_refresh_apple(apple_t* apple) {
    ILI9341_DrawFilledRectangle(
        apple->x * CELL_SIZE,
        apple->y * CELL_SIZE,
        (apple->x + 1) * CELL_SIZE - 1,
        (apple->y + 1) * CELL_SIZE - 1,
        ILI9341_COLOR_GREEN);
}
