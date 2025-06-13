/* breakbricks.c */

#include "config.h"
#include "breakbricks.h"
#include "button.h"
#include "displaybreak.h"
#include "stdbool.h"
#include "stm32g4_systick.h"
#include "stm32g4_utils.h"
#include "stm32g4xx_hal.h"
#include "stm32g4_uart.h"

extern UART_HandleTypeDef huart1;

#define BLINK_DELAY		100	//ms

#define DEFAULT_RACKET_SPEED 10
#define REFRESH_PERIOD_MS  30

static bool flag_refresh = false;
static ballbreak_t ball;
static racketbreak_t racket;
static grid_t grid;
static int8_t life;

static void BREAKBRICKS_process_ms(void);
static void BREAKBRICKS_racket_update(bool button_left, bool button_right);
static void BREAKBRICKS_ball_update(bool button_up);
static void BREAKBRICKS_check_collision(void);
static void BREAKBRICKS_breaks(uint8_t b);









void BREAKBRICKS_init(void)
{
    BUTTONS_init();
    DISPLAYBREAK_init();

    ball.size = 10;
    ball.x = SCREEN_WIDTH / 2;
    ball.y = 40;
    ball.fine_x = ball.x << 3;
    ball.fine_y = ball.y << 3;
    ball.speed_x = 0;
    ball.speed_y = 0; // Balle initialement immobile
    ball.joker = 0;
    ball.options = BREAK_BALL_OPTION_GLUE;

    racket.x = SCREEN_WIDTH / 2;
    racket.y = 10;
    racket.width = 50;
    racket.height = 10;
    racket.speed = 0;

    grid.nb_bricks_remaining = 0;
    for (int y = 0; y < NB_BRICKS_VERTICAL; y++) {
        for (int x = 0; x < NB_BRICKS_LATERAL; x++) {
            int b = y * NB_BRICKS_LATERAL + x;
            grid.bricks[b].style = BRICKSTYLE_FULL;
            grid.bricks[b].x1 = (int16_t)(MARGIN_DEFAULT + x * BRICK_WIDTH_DEFAULT);
            grid.bricks[b].x2 = (int16_t)(-MARGIN_DEFAULT + (x + 1) * BRICK_WIDTH_DEFAULT);
            grid.bricks[b].y1 = (int16_t)(SCREEN_HEIGHT + MARGIN_DEFAULT - (y + 1) * BRICK_HEIGHT_DEFAULT);
            grid.bricks[b].y2 = (int16_t)(SCREEN_HEIGHT - MARGIN_DEFAULT - y * BRICK_HEIGHT_DEFAULT);
            grid.nb_bricks_remaining++;
        }
    }

    life = 3;
    DISPLAYBREAK_refresh_ball(&ball);
    BSP_systick_add_callback_function(&BREAKBRICKS_process_ms);
}

bool char_received(uart_id_t uart_id)
{
	if( BSP_UART_data_ready(uart_id) )	/* Si un caract�re est re�u sur l'UART 2*/
	{
		/* On "utilise" le caract�re pour vider le buffer de r�ception */
		BSP_UART_get_next_byte(uart_id);
		return true;
	}
	else
		return false;
}


void BREAKBRICKS_process_main(void)
{

    // Lit les boutons physiques
    uint8_t button_left = BUTTON_left_read();
    uint8_t button_right = BUTTON_right_read();
    uint8_t button_center = BUTTON_center_read();
    uint8_t button_up = BUTTON_up_read();

    bool j1u = 0 , j1d = 0 , j1l = 0 , j1r = 0 , j1c = 0 ;
    bool j2h = 0 , j2b = 0 , j2g = 0, j2e=0 , j2m = 0;

    if(BSP_UART_data_ready(UART1_ID))
           {
           	uint8_t received_char = 0;
           	received_char = BSP_UART_get_next_byte(UART1_ID);
       		// Traite les commandes UART
       		switch(received_char)
       		{
       			case 'u':
       				j1u = 1; // touche du haut pour le joueur 1
       				break;
       			case 'd' :
       				j1d = 1 ; // touche du bas pour le joueur 1
       				break ;
       			case 'l':
       			j1l = 1;	// touche de gauche
       				break;
       			case 'r':
       			j1r = 1;	//touche de droite
       				break;
       			case 'c' :
       				j1c = 1 ;
       			default:
       				// Caractère non reconnu - on ignore
       				break;
       		}
           }
        if(BSP_UART_data_ready(UART2_ID))
             {
             	uint8_t received_char = 0;
             	received_char = BSP_UART_get_next_byte(UART2_ID);
         		// Traite les commandes UART
         		switch(received_char)
         		{ // touches joueurs 2
         		case 'h':
         			j2h = 1; //haut
         			break;
         		case 'b' :
         			j2b= 1 ; //bas
         			break;
         		case 'g':
         		   	j2g= 1; //gauche
         		   	break;
         		case 'e':
         		   	j2e= 1; //droite
         		   	break;
         		case 'm' :
         			j2m = 1 ;
         			default:
         		   				// Caractère non reconnu - on ignore
         			break;
         		}
             }

    // Combine boutons physiques et UART
    button_left = button_left || j1l || j2g;
    button_right = button_right || j1r || j2e ;
    button_up = button_up || j1u || j2h;
    button_center = button_center || j1c || j2m ;
    /* Le reste de votre code existant... */
    if (button_left){
        HAL_GPIO_WritePin(LED_GREEN_GPIO, LED_GREEN_PIN, GPIO_PIN_RESET);
    }
    else if (button_center){
        HAL_GPIO_WritePin(LED_GREEN_GPIO, LED_GREEN_PIN, GPIO_PIN_SET);
    }


    if (flag_refresh) {
        flag_refresh = false;

        if (ball.speed_x == 0 && ball.speed_y == 0)
        {
            ball.x = racket.x;
            ball.y = (int16_t)(racket.y + racket.height / 2 + ball.size / 2 + 1);
            ball.fine_x = ball.x << 3;
            ball.fine_y = ball.y << 3;
            if (button_up) {
                ball.speed_y = -120;
            }
        }

        BREAKBRICKS_ball_update(button_up);
        BREAKBRICKS_racket_update(button_left, button_center);
        BREAKBRICKS_check_collision();
        DISPLAYBREAK_refresh_ball(&ball);
        DISPLAYBREAK_refresh_racket(&racket);
        DISPLAYBREAK_refresh_grid(&grid);
        DISPLAYBREAK_refresh_life(life);


    }

    if (life == 0 || grid.nb_bricks_remaining == 0)
    {
        while (1);
    }
}


static void BREAKBRICKS_process_ms(void)
{
    static int t = 0;
    if (t < REFRESH_PERIOD_MS - 1) {
        t++;
    } else {
        t = 0;
        flag_refresh = true;
    }
}

static void BREAKBRICKS_racket_update(bool button_left, bool button_right)
{
    if (button_left)
        racket.speed = -DEFAULT_RACKET_SPEED;
    else if (button_right)
        racket.speed = DEFAULT_RACKET_SPEED;
    else
        racket.speed = 0;

    racket.x += racket.speed;

    if (racket.x < racket.width / 2)
        racket.x = racket.width / 2;
    if (racket.x > SCREEN_WIDTH - racket.width / 2)
        racket.x = SCREEN_WIDTH - racket.width / 2;
}

static void BREAKBRICKS_ball_update(bool button_up)
{
    ball.fine_x += ball.speed_x;
    ball.fine_y += ball.speed_y;
    ball.x = ball.fine_x >> 3;
    ball.y = ball.fine_y >> 3;

    if (ball.x < ball.size / 2)
        ball.x = ball.size / 2;
    if (ball.x > SCREEN_WIDTH - ball.size / 2)
        ball.x = SCREEN_WIDTH - ball.size / 2;
    if (ball.y < ball.size / 2)
        ball.y = ball.size / 2;
    if (ball.y > SCREEN_HEIGHT - ball.size / 2)
        ball.y = SCREEN_HEIGHT - ball.size / 2;
}

static void BREAKBRICKS_check_collision(void)
{
    if (ball.y + ball.size / 2 >= SCREEN_HEIGHT)
        ball.speed_y = -ball.speed_y;

    if ((ball.x - ball.size / 2 <= 0) || (ball.x + ball.size / 2 >= SCREEN_WIDTH - 1))
        ball.speed_x = -ball.speed_x;

    if (ball.y - ball.size / 2 <= 0)
    {
        ball.speed_x = 0;
        ball.speed_y = 0;
        life--;
    }

    if (ball.y - ball.size / 2 < racket.y + racket.height / 2 + 1 &&
        ball.x + ball.size / 2 >= racket.x - racket.width / 2 - 1 &&
        ball.x - ball.size / 2 <= racket.x + racket.width / 2 + 1)
    {
        ball.y = (int16_t)(racket.y + racket.height / 2 + ball.size / 2 + 1);
        ball.speed_y = -ball.speed_y;
        int16_t percentage = (int16_t)((ball.x - racket.x) * 100 / racket.width);
        ball.speed_x = percentage;
    }

    for (int b = 0; b < NB_MAX_BRICKS; b++) {
        if (grid.bricks[b].style != BRICKSTYLE_NONE) {
            if (ball.x >= grid.bricks[b].x1 - ball.size / 2 &&
                ball.x <= grid.bricks[b].x2 + ball.size / 2 &&
                ball.y >= grid.bricks[b].y1 - ball.size / 2 &&
                ball.y <= grid.bricks[b].y2 + ball.size / 2)
            {
                BREAKBRICKS_breaks(b);
                if (ball.x >= grid.bricks[b].x2 || ball.x <= grid.bricks[b].x1)
                    ball.speed_x = -ball.speed_x;
                if (ball.y >= grid.bricks[b].y2 || ball.y <= grid.bricks[b].y1)
                    ball.speed_y = -ball.speed_y;
            }
        }
    }
}

static void BREAKBRICKS_breaks(uint8_t b)
{
    if (grid.bricks[b].style != BRICKSTYLE_NONE)
    {
        grid.bricks[b].style = BRICKSTYLE_NONE;
        grid.nb_bricks_remaining--;
    }
}

/*
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART2) // V�rifie que c'est bien UART2
    {
        switch (btn[0]) {
            case 'u': button_up = true; break;
            case 'l': button_left = true; break;
            case 'r': button_center = true; break;
        }

        // Red�marre la r�ception pour les prochaines donn�es
        HAL_UART_Receive_IT(UART1_ID, (uint8_t *)btn, 1);
    }
}
*/
