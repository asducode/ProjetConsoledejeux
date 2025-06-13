/* display.c */

#include "stm32g4xx_hal.h"
#include "breakbricks.h"
#include "displaybreak.h"
#include "TFT_ili9341/stm32g4_ili9341.h"
#include "TFT_ili9341/stm32g4_fonts.h"
#include "stm32g4_systick.h"

extern const uint16_t ball_color[BREAK_BALL_OPTION_NB] = {
	ILI9341_COLOR_RED, 		//BALL_OPTION_NONE
	ILI9341_COLOR_BLUE, 	//BALL_OPTION_GLUE
	ILI9341_COLOR_ORANGE	//BALL_OPTION_JOKER
};

static char style2char(uint8_t style)
{
    switch (style)
    {
        case BRICKSTYLE_BOMB:     return 'B';
        case BRICKSTYLE_JOKER:    return 'J';
        case BRICKSTYLE_RESIZER:  return 'R';
        case BRICKSTYLE_GLUE:     return 'G';
        case BRICKSTYLE_ROCK:     return 'X';
        case BRICKSTYLE_LIFE:     return 'L';
        default:                  return ' ';
    }
}

void DISPLAYBREAK_init(void) {
	ILI9341_Init();
	ILI9341_Fill(ILI9341_COLOR_WHITE);
	ILI9341_Rotate(ILI9341_Orientation_Landscape_2);
}

void DISPLAYBREAK_refresh_ball(ballbreak_t *ball)
{
	static ballbreak_t displayed_ball;
	static ballbreak_options_e displayed_ball_valid = false;

	if (displayed_ball.x != ball->x || displayed_ball.y != ball->y || displayed_ball.size != ball->size || displayed_ball.options != ball->options)
	{
		if(displayed_ball_valid)
			ILI9341_DrawCircle(displayed_ball.x, ILI9341_WIDTH - displayed_ball.y, displayed_ball.size/2, ILI9341_COLOR_WHITE);

		ILI9341_DrawCircle(ball->x, ILI9341_WIDTH - ball->y, ball->size/2, ball_color[ball->options]);
		displayed_ball = *ball;
		displayed_ball_valid = true;
	}
}

void DISPLAYBREAK_refresh_racket(racketbreak_t *racket)
{
    const uint16_t racket_color = ILI9341_COLOR_BROWN;
    static uint16_t x1d, x2d, y1d, y2d;
    static bool initialized = false;
    uint16_t x1, x2;

    x1 = racket->x - racket->width / 2;
    x2 = racket->x + racket->width / 2;

    if (!initialized)
    {
        x1d = x1;
        x2d = x2;
        y1d = (uint16_t)(ILI9341_WIDTH - racket->y - racket->height / 2);
        y2d = (uint16_t)(ILI9341_WIDTH - racket->y + racket->height / 2);
        ILI9341_DrawFilledRectangle(x1d, y1d, x2d, y2d, racket_color);
        initialized = true;
    }
    else
    {
        if (x1 < x1d)
            ILI9341_DrawFilledRectangle(x1, y1d, x1d, y2d, racket_color);
        else if (x1 > x1d)
            ILI9341_DrawFilledRectangle(x1d, y1d, x1, y2d, ILI9341_COLOR_WHITE);

        if (x2 > x2d)
            ILI9341_DrawFilledRectangle(x2d, y1d, x2, y2d, racket_color);
        else if (x2 < x2d)
            ILI9341_DrawFilledRectangle(x2, y1d, x2d, y2d, ILI9341_COLOR_WHITE);

        x1d = x1;
        x2d = x2;
    }
}

void DISPLAYBREAK_refresh_grid(grid_t *grid)
{
    static brickstyle_e displayed_bricks[NB_MAX_BRICKS] = {BRICKSTYLE_NONE};

    for (int b = 0; b < NB_MAX_BRICKS; b++)
    {
        brick_t *brick = &grid->bricks[b];

        if (brick->style != displayed_bricks[b])
        {
            uint16_t color = ILI9341_COLOR_WHITE;

            switch (brick->style)
            {
                case BRICKSTYLE_FULL:     color = ILI9341_COLOR_YELLOW; break;
                case BRICKSTYLE_ROCK:     color = ILI9341_COLOR_GRAY;   break;
                case BRICKSTYLE_BOMB:     color = ILI9341_COLOR_BLACK;  break;
                case BRICKSTYLE_RESIZER:  color = ILI9341_COLOR_GREEN;  break;
                case BRICKSTYLE_JOKER:    color = ILI9341_COLOR_ORANGE; break;
                case BRICKSTYLE_GLUE:     color = ILI9341_COLOR_BLUE;   break;
                case BRICKSTYLE_LIFE:     color = ILI9341_COLOR_RED;    break;
                case BRICKSTYLE_NONE:
                default:                  color = ILI9341_COLOR_WHITE;  break;
            }

            uint16_t x1 = brick->x1;
            uint16_t x2 = brick->x2;
            uint16_t y1 = ILI9341_WIDTH - brick->y2;
            uint16_t y2 = ILI9341_WIDTH - brick->y1;

            ILI9341_DrawFilledRectangle(x1, y1, x2, y2, color);

            ILI9341_DrawRectangle(x1, y1, x2, y2,
                (brick->style != BRICKSTYLE_NONE) ? ILI9341_COLOR_BLACK : ILI9341_COLOR_WHITE);

            if (brick->style > BRICKSTYLE_FULL)
            {
                char c = style2char(brick->style);
                ILI9341_Putc(
                    c,
                    x1 + (x2 - x1) / 2 - Font_11x18.FontWidth / 2,
                    y1 + (y2 - y1) / 2 - Font_11x18.FontHeight / 2,
                    &Font_11x18,
                    ILI9341_COLOR_WHITE,
                    color
                );
            }

            displayed_bricks[b] = brick->style;
        }
    }
}

void DISPLAYBREAK_refresh_life(int8_t life)
{
    char buffer[10];
    sprintf(buffer, "Vies: %d", life);
    ILI9341_DrawFilledRectangle(5, 5, 100, 25, ILI9341_COLOR_WHITE); // efface la zone
    ILI9341_Puts(5, 5, buffer, &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
}


