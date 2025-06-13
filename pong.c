/**
 * @file pong.c
 * @brief Implémentation du jeu Pong
 * @details Ce fichier contient la logique du jeu Pong, un jeu classique de tennis de table
 * où deux joueurs contrôlent des raquettes pour renvoyer une balle.
 */

#include "config.h"
#include "pong.h"
#include "button.h"
#include "display.h"
#include "stdbool.h"
#include "stm32g4_systick.h"
#include "stm32g4_utils.h"
#include "TFT_ili9341/stm32g4_ili9341.h"
#include "TFT_ili9341/stm32g4_fonts.h"
#include <stdlib.h>
#include <time.h>
#include "stm32g4xx_hal.h"
#include "stm32g4_uart.h"


static bool flag_refresh = false;

static ball_t ball;
static racket_t r1;
static racket_t r2;
static uint8_t pong_score_player1 = 0;
static uint8_t pong_score_player2 = 0;

static game_state_t game_state = GAME_WAIT_START;

static void PONG_process_ms(void);
static void pong_reset_ball(void);
static void PONG_display_scores(void);
static void PONG_racket_update(bool button_up, bool button_down, racket_t* racket);
static void PONG_racket_update_edges(racket_t* r);
static void PONG_ball_update(void);
static void PONG_check_collision(racket_t* r1, racket_t* r2);

void DISPLAY_show_winner(uint8_t winner);

/**
 * @brief Initialise le jeu Pong
 * @details Configure les paramètres initiaux du jeu, la position des raquettes et de la balle
 */
void PONG_init(void) {
    BUTTONS_init();
    DISPLAY_init();

    pong_score_player1 = 0;
    pong_score_player2 = 0;
    game_state = GAME_WAIT_START;

    ball.size = 10;
    ball.options = BALL_OPTION_GLUE;

    r1.x = 5 + RACKET_WIDTH / 2;
    r1.y = SCREEN_HEIGHT / 2;
    r1.width = RACKET_WIDTH;
    r1.height = RACKET_HEIGHT;
    r1.speed = 0;

    r2.x = SCREEN_WIDTH - 5 - RACKET_WIDTH / 2;
    r2.y = SCREEN_HEIGHT / 2;
    r2.width = RACKET_WIDTH;
    r2.height = RACKET_HEIGHT;
    r2.speed = 0;

    DISPLAY_draw_middle_line();
    DISPLAY_show_scores(pong_score_player1, pong_score_player2);

    srand((unsigned int)time(NULL));

    BSP_systick_add_callback_function(&PONG_process_ms);
}

/**
 * @brief Gère la logique du jeu Pong
 * @details Met à jour la position des raquettes et de la balle, gère les collisions et le score
 */
void PONG_process_main(void) {
   // uint8_t uart_left = 0, uart_right = 0, uart_up = 0, uart_center = 0 , uart_down = 0;
    bool j1u = 0 , j1d = 0 , j1l = 0 , j1r = 0 , j1c = 0 , j1reset = 0;
    bool j2h = 0 , j2b = 0 , j2g = 0, j2e=0 , j2m = 0 , j2reset = 0;
    if (!flag_refresh) return;
    flag_refresh = false;
/*
    uint8_t button_left = BUTTON_left_read();
    uint8_t button_right = BUTTON_center_read();
*/
    uint8_t button_center = BUTTON_right_read();

 //   uint8_t button_up = BUTTON_up_read();
  ///	uint8_t button_down = BUTTON_down_read();

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
   			case 'f' :
   				HAL_NVIC_SystemReset();
   				break;
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
     			break;
     		case 'z' :
   				HAL_NVIC_SystemReset();
     			break;
     			default:
     		   				// Caractère non reconnu - on ignore
     			break;
     		}
         }
/*
    button_right = button_right || uart_center ;
    button_left = button_left || uart_left ;
    uint8_t button_down = uart_down ;
    uint8_t button_up = uart_up ;
*/
    button_center =  j1c || j2m || button_center;
    if (game_state == GAME_WAIT_START && button_center) { // Si un des boutons centraux est pressé
        pong_score_player1 = 0;
        pong_score_player2 = 0;
        PONG_display_scores();

        r1.y = SCREEN_HEIGHT / 2;
        r2.y = SCREEN_HEIGHT / 2;

        pong_reset_ball();
        DISPLAY_refresh_ball(&ball);
        game_state = GAME_RUNNING;
    }

    if (game_state != GAME_RUNNING) return;

    if(j1reset || j2reset) game_state = GAME_WAIT_START;
    PONG_racket_update(j1l, j1r, &r1);
    PONG_racket_update(j2g, j2e, &r2);
    PONG_ball_update();
    PONG_check_collision(&r1, &r2);

    DISPLAY_refresh_ball(&ball);
    DISPLAY_refresh_racket(&r1);
    DISPLAY_refresh_racket(&r2);
    PONG_display_scores();
}

static void PONG_process_ms(void) {
    static int t = 0;
    if (++t >= REFRESH_PERIOD_MS) {
        t = 0;
        flag_refresh = true;
    }
}

static void PONG_racket_update_edges(racket_t* r) {
    r->left = r->x - r->width / 2;
    r->right = r->x + r->width / 2;
}

static void PONG_racket_update(bool button_up, bool button_down, racket_t* racket) {

    if (button_up)
        racket->speed = -RACKET_SPEED;
    else if (button_down)
        racket->speed = RACKET_SPEED;
    else
        racket->speed = 0;

    racket->y += racket->speed;

    if (racket->y < racket->height / 2)
        racket->y = racket->height / 2;
    if (racket->y > SCREEN_HEIGHT - racket->height / 2)
        racket->y = SCREEN_HEIGHT - racket->height / 2;

    PONG_racket_update_edges(racket);
}

static void PONG_ball_update(void) {
    ball.fine_x += ball.speed_x;
    ball.fine_y += ball.speed_y;

    ball.x = ball.fine_x >> 3;
    ball.y = ball.fine_y >> 3;

    if (ball.y <= ball.size / 2) {
        ball.y = ball.size / 2;
        ball.speed_y = -ball.speed_y;
        ball.fine_y = ball.y << 3;
    }

    if (ball.y >= SCREEN_HEIGHT - ball.size / 2) {
        ball.y = SCREEN_HEIGHT - ball.size / 2;
        ball.speed_y = -ball.speed_y;
        ball.fine_y = ball.y << 3;
    }
}

static void PONG_check_collision(racket_t* r1, racket_t* r2) {
    if (ball.x - ball.size / 2 <= r1->right &&
        ball.x >= r1->left &&
        ball.y >= r1->y - r1->height / 2 &&
        ball.y <= r1->y + r1->height / 2) {

        ball.x = r1->right + ball.size / 2;
        ball.fine_x = ball.x << 3;
        ball.speed_x = -ball.speed_x;
    }

    if (ball.x + ball.size / 2 >= r2->left &&
        ball.x <= r2->right &&
        ball.y >= r2->y - r2->height / 2 &&
        ball.y <= r2->y + r2->height / 2) {

        ball.x = r2->left - ball.size / 2;
        ball.fine_x = ball.x << 3;
        ball.speed_x = -ball.speed_x;
    }

    if (ball.x < 0) {
        pong_score_player2++;
        pong_reset_ball();
    } else if (ball.x > SCREEN_WIDTH) {
        pong_score_player1++;
        pong_reset_ball();
    }

    DISPLAY_show_scores(pong_score_player1, pong_score_player2);

    if (pong_score_player1 >= 7 || pong_score_player2 >= 7) {
        game_state = GAME_OVER;
        DISPLAY_show_winner(pong_score_player1 >= 7 ? 1 : 2);
    }
}

static void pong_reset_ball(void) {
    ball.x = SCREEN_WIDTH / 2;
    ball.y = SCREEN_HEIGHT / 2;
    ball.fine_x = ball.x << 3;
    ball.fine_y = ball.y << 3;
    ball.speed_x = (rand() % 2 ? 1 : -1) * 20;
    ball.speed_y = (rand() % 2 ? 1 : -1) * 20;
}

static void PONG_display_scores(void) {
    static uint8_t last_score_p1 = 255;
    static uint8_t last_score_p2 = 255;

    if (pong_score_player1 != last_score_p1) {
        ILI9341_DrawFilledRectangle(10, 10, 10 + 30 - 1, 10 + 20 - 1, ILI9341_COLOR_WHITE);
        ILI9341_printf(10, 10, &Font_11x18, ILI9341_COLOR_RED, ILI9341_COLOR_WHITE, "%u", pong_score_player1);
        last_score_p1 = pong_score_player1;
    }

    if (pong_score_player2 != last_score_p2) {
        ILI9341_DrawFilledRectangle(SCREEN_WIDTH - 40, 10, SCREEN_WIDTH - 11, 10 + 20 - 1, ILI9341_COLOR_WHITE);
        ILI9341_printf(SCREEN_WIDTH - 40, 10, &Font_11x18, ILI9341_COLOR_BLUE, ILI9341_COLOR_WHITE, "%u", pong_score_player2);
        last_score_p2 = pong_score_player2;
    }
}

void DISPLAY_show_winner(uint8_t winner) {
    ILI9341_DrawFilledRectangle(40, SCREEN_HEIGHT / 2 - 20,
                                 SCREEN_WIDTH - 40, SCREEN_HEIGHT / 2 + 20,
                                 ILI9341_COLOR_BLACK);
    ILI9341_printf(SCREEN_WIDTH / 2 - 60, SCREEN_HEIGHT / 2 - 10,
                   &Font_11x18, ILI9341_COLOR_YELLOW, ILI9341_COLOR_BLACK,
                   "Player %u Wins!", winner);
}

/**
 * @brief Gère les collisions de la balle avec les raquettes
 * @param ballX Position X de la balle
 * @param ballY Position Y de la balle
 * @return 1 si une collision est détectée, 0 sinon
 */
int checkPaddleCollision(int ballX, int ballY) {
    // Implementation of checkPaddleCollision function
    return 0; // Placeholder return, actual implementation needed
}

/**
 * @brief Gère les collisions de la balle avec les murs
 * @param ballX Position X de la balle
 * @param ballY Position Y de la balle
 * @return 1 si une collision est détectée, 0 sinon
 */
int checkWallCollision(int ballX, int ballY) {
    // Implementation of checkWallCollision function
    return 0; // Placeholder return, actual implementation needed
}

/**
 * @brief Met à jour la position des raquettes
 * @param player1Direction Direction de déplacement de la raquette du joueur 1
 * @param player2Direction Direction de déplacement de la raquette du joueur 2
 */
void updatePaddles(int player1Direction, int player2Direction) {
    // Implementation of updatePaddles function
}

/**
 * @brief Vérifie si un point a été marqué
 * @param ballX Position X de la balle
 * @return 1 si un point est marqué, 0 sinon
 */
int checkScore(int ballX) {
    // Implementation of checkScore function
    return 0; // Placeholder return, actual implementation needed
}

/**
 * @brief Vérifie si la partie est terminée
 * @return 1 si la partie est terminée, 0 sinon
 */
int checkGameOver(void) {
    // Implementation of checkGameOver function
    return 0; // Placeholder return, actual implementation needed
}
