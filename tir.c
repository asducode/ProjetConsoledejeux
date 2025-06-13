#include <displaytir.h>
#include "tir.h"
#include "button.h"
#include <stdlib.h>
#include "stm32g4_uart.h"

static playertir_t p1, p2;
static bullet_t bullets_p1[MAX_BULLETS];
static bullet_t bullets_p2[MAX_BULLETS];
static gametir_state_t game_state = TIR_GAME_WAIT_START;

void TIR_update_positions(void);
void TIR_check_collisions(void);

void TIR_init(void) {
    BUTTONS_init();
    DISPLAY_TIR_init();

    p1.x = 30;
    p1.y = SCREEN_HEIGHT / 2;
    p1.prev_x = p1.x;
    p1.prev_y = p1.y;
    p1.health = 5;

    p2.x = SCREEN_WIDTH - 50;
    p2.y = SCREEN_HEIGHT / 2;
    p2.prev_x = p2.x;
    p2.prev_y = p2.y;
    p2.health = 5;

    for (int i = 0; i < MAX_BULLETS; i++) {
        bullets_p1[i].active = 0;
        bullets_p2[i].active = 0;
    }

    DISPLAY_TIR_refresh_players(&p1, &p2);
    DISPLAY_TIR_draw_health(p1.health, p2.health);

    game_state = TIR_GAME_WAIT_START;
}

void TIR_process_main(void) {
    if (game_state == TIR_GAME_WAIT_START ) {
        game_state = TIR_GAME_RUNNING;
    }

    if (game_state != TIR_GAME_RUNNING) return;

    TIR_update_positions();
    TIR_check_collisions();

    DISPLAY_TIR_refresh_players(&p1, &p2);
    DISPLAY_TIR_refresh_bullets(bullets_p1, bullets_p2);
    DISPLAY_TIR_draw_health(p1.health, p2.health);
}

void TIR_update_positions(void) {

	 bool j1u = 0 , j1d = 0 , j1l = 0 , j1r = 0 , j1c = 0 ; //joueur1
	 bool j2h = 0 , j2b = 0 , j2g = 0, j2e=0 , j2m = 0; //joueur 2
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
	   				break;
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
	     			j2m = 1;
					break;
	     		case 'z' :
	     				HAL_NVIC_SystemReset();
	     			break;
	     		default:
	     		   				// Caractère non reconnu - on ignore
	     			break;
	     		}
	         }

	// Joueur 1
	    if (j1d) p1.y -= PLAYER_SPEED;
	    if (j1u) p1.y += PLAYER_SPEED;

    if (j1l) p1.x -= PLAYER_SPEED;
    if (j1r) p1.x += PLAYER_SPEED;
    if (j1c) TIR_shoot(&p1);

    // Joueur 2 (boutons personnalisés)
    if (j2b) p2.y -= PLAYER_SPEED;
    if (j2h) p2.y += PLAYER_SPEED;
    if (j2g) p2.x -= PLAYER_SPEED;
    if (j2e) p2.x += PLAYER_SPEED;
    if (j2m) TIR_shoot(&p2);

    // Limites écran
    p1.x = (p1.x < 0) ? 0 : (p1.x > SCREEN_WIDTH - PLAYER_WIDTH_TIR) ? SCREEN_WIDTH - PLAYER_WIDTH_TIR : p1.x;
    p1.y = (p1.y < 0) ? 0 : (p1.y > SCREEN_HEIGHT - PLAYER_HEIGHT_TIR) ? SCREEN_HEIGHT - PLAYER_HEIGHT_TIR : p1.y;

    p2.x = (p2.x < 0) ? 0 : (p2.x > SCREEN_WIDTH - PLAYER_WIDTH_TIR) ? SCREEN_WIDTH - PLAYER_WIDTH_TIR : p2.x;
    p2.y = (p2.y < 0) ? 0 : (p2.y > SCREEN_HEIGHT - PLAYER_HEIGHT_TIR) ? SCREEN_HEIGHT - PLAYER_HEIGHT_TIR : p2.y;
}

void TIR_shoot(playertir_t *player) {
    bullet_t *bullets = (player == &p1) ? bullets_p1 : bullets_p2;
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (!bullets[i].active) {
            bullets[i].active = 1;
            bullets[i].x = player->x + ((player == &p1) ? PLAYER_WIDTH_TIR : -5);
            bullets[i].y = player->y + PLAYER_HEIGHT_TIR / 2;
            bullets[i].prev_x = bullets[i].x;
            bullets[i].prev_y = bullets[i].y;
            bullets[i].speed_x = (player == &p1) ? BULLET_SPEED : -BULLET_SPEED;
            break;
        }
    }
}

void TIR_check_collisions(void) {
    for (int i = 0; i < MAX_BULLETS; i++) {
        // Balle P1
        if (bullets_p1[i].active) {
            DISPLAY_TIR_erase_bullet(&bullets_p1[i]);
            bullets_p1[i].prev_x = bullets_p1[i].x;
            bullets_p1[i].prev_y = bullets_p1[i].y;
            bullets_p1[i].x += bullets_p1[i].speed_x;

            if (bullets_p1[i].x < 0 || bullets_p1[i].x > SCREEN_WIDTH) {
                bullets_p1[i].active = 0;
                continue;
            }

            if (bullets_p1[i].x >= p2.x && bullets_p1[i].x <= p2.x + PLAYER_WIDTH_TIR &&
                bullets_p1[i].y >= p2.y && bullets_p1[i].y <= p2.y + PLAYER_HEIGHT_TIR) {
                bullets_p1[i].active = 0;
                p2.health--;
                p2.x += 5;
                continue;
            }
        }

        // Balle P2
        if (bullets_p2[i].active) {
            DISPLAY_TIR_erase_bullet(&bullets_p2[i]);
            bullets_p2[i].prev_x = bullets_p2[i].x;
            bullets_p2[i].prev_y = bullets_p2[i].y;
            bullets_p2[i].x += bullets_p2[i].speed_x;

            if (bullets_p2[i].x < 0 || bullets_p2[i].x > SCREEN_WIDTH) {
                bullets_p2[i].active = 0;
                continue;
            }

            if (bullets_p2[i].x >= p1.x && bullets_p2[i].x <= p1.x + PLAYER_WIDTH_TIR &&
                bullets_p2[i].y >= p1.y && bullets_p2[i].y <= p1.y + PLAYER_HEIGHT_TIR) {
                bullets_p2[i].active = 0;
                p1.health--;
                p1.x -= 5;
                continue;
            }
        }
    }

    if (p1.health <= 0 || p2.health <= 0) {
        game_state = TIR_GAME_OVER;
        DISPLAY_TIR_show_winner(p1.health > 0 ? 1 : 2);
    }
}
