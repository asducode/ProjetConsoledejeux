#include <displaysnake.h>
#include "snake.h"
#include "button.h"
#include <stdlib.h>
#include <stdbool.h>
#include "stm32g4_uart.h"

#define SPEED_FACTOR 1

static snake_t snake1, snake2;
static apple_t apple;
static uint8_t score1 = 0, score2 = 0;

static bool check_collision_with_snake(snake_t* snake, uint8_t x, uint8_t y);
static bool eat_apple(snake_t* snake);
static void spawn_apple(void);
static void reset_snake(snake_t* snake);
static void shrink_snake(snake_t* snake);

static bool check_collision(snake_t* snake, snake_t* opponent);
static void move_snake(snake_t* snake, uint8_t button_up, uint8_t button_down, uint8_t button_left, uint8_t button_right) ;

static void spawn_apple(void) {
    do {
        apple.x = rand() % GRID_WIDTH;
        apple.y = rand() % GRID_HEIGHT;
    } while (check_collision_with_snake(&snake1, apple.x, apple.y) || check_collision_with_snake(&snake2, apple.x, apple.y));

    DISPLAYSNAKE_refresh_apple(&apple);
}

static bool check_collision_with_snake(snake_t* snake, uint8_t x, uint8_t y) {
    for (int i = 0; i < snake->length; ++i)
        if (snake->body[i].x == x && snake->body[i].y == y)
            return true;
    return false;
}

static void move_snake(snake_t* snake, uint8_t button_up, uint8_t button_down, uint8_t button_left, uint8_t button_right) {
    // Mise à jour de la direction en fonction des entrées du joueur
    if (button_left && snake->dir != DIR_RIGHT) {
        snake->dir = DIR_LEFT;
    } else if (button_right && snake->dir != DIR_LEFT) {
        snake->dir = DIR_RIGHT;
    } else if (button_up && snake->dir != DIR_DOWN) {
        snake->dir = DIR_UP;
    } else if (button_down && snake->dir != DIR_UP) {
        snake->dir = DIR_DOWN;
    }

    // Déplacer le corps du serpent
    for (int i = snake->length - 1; i > 0; i--) {
        snake->body[i] = snake->body[i - 1];
    }

    // Déplacer la tête du serpent
    switch (snake->dir) {
        case DIR_UP:
        	snake->body[0].y -= SPEED_FACTOR;
            break;
        case DIR_DOWN:
        	 snake->body[0].y += SPEED_FACTOR;
            break;
        case DIR_LEFT:
        	snake->body[0].x -= SPEED_FACTOR;
            break;
        case DIR_RIGHT:
        	snake->body[0].x += SPEED_FACTOR;
            break;
    }
}




static bool check_collision(snake_t* snake, snake_t* opponent) {
    positionsnake_t head = snake->body[0];

    // Vérifier si la tête du serpent sort de l'écran
    if (head.x < 0 || head.x >= GRID_WIDTH || head.y < 0 || head.y >= GRID_HEIGHT)
        return true;

    // Vérifier si la tête touche son propre corps
    for (int i = 1; i < snake->length; i++) {
        if (snake->body[i].x == head.x && snake->body[i].y == head.y)
            return false;
    }

    // Vérifier si la tête touche le corps de l'autre serpent
    for (int i = 0; i < opponent->length; i++) {
        if (opponent->body[i].x == head.x && opponent->body[i].y == head.y) {
            // Si collision, réduction du score et rétrécissement du serpent
            if (snake == &snake1) {
                score1 = (score1 > 0) ? score1 - 1 : 0;
                shrink_snake(&snake1);
                reset_snake(&snake1);
            } else {
                score2 = (score2 > 0) ? score2 - 1 : 0;
                shrink_snake(&snake2);
                reset_snake(&snake2);
            }
            return true;
        }
    }

    return false;
}

static void shrink_snake(snake_t* snake) {
    if (snake->length > 3) {  // Ne pas aller en dessous de la taille minimale
        snake->length--;
    }
}

static void reset_snake(snake_t* snake) {
    snake->body[0] = (positionsnake_t){rand() % GRID_WIDTH, rand() % GRID_HEIGHT};

    for (int i = 1; i < snake->length; i++) {
        snake->body[i].x = snake->body[0].x - i;
        snake->body[i].y = snake->body[0].y;
    }

    snake->dir = DIR_RIGHT; // Direction par défaut

    DISPLAYSNAKE_refresh_snake(snake, (snake == &snake1) ? 0 : 1);
}


static bool eat_apple(snake_t* snake) {
    if (snake->body[0].x == apple.x && snake->body[0].y == apple.y) {
        if (snake->length < MAX_SNAKE_LENGTH){
            snake->length++;
            snake->body[snake->length] = snake->body[snake->length - 1];
            snake->length++;}
        return true;
    }
    return false;
}

void snake_init(void) {
    snake1.length = 3;
    snake1.body[0] = (positionsnake_t){5, 5};
    snake1.body[1] = (positionsnake_t){4, 5};
    snake1.body[2] = (positionsnake_t){3, 5};
    snake1.dir = DIR_RIGHT;

    snake2.length = 3;
    snake2.body[0] = (positionsnake_t){25, 15};
    snake2.body[1] = (positionsnake_t){26, 15};
    snake2.body[2] = (positionsnake_t){27, 15};
    snake2.dir = DIR_LEFT;

    score1 = 0;
    score2 = 0;

    DISPLAYSNAKE_init();
    DISPLAYSNAKE_refresh_snake(&snake1, 0);
    DISPLAYSNAKE_refresh_snake(&snake2, 1);
    spawn_apple();
    DISPLAYSNAKE_draw_scores(score1, score2);
}
void snake_update(void) {
    // Effacer uniquement les anciennes positions des serpents
    DISPLAYSNAKE_erase_snake(&snake1);
    DISPLAYSNAKE_erase_snake(&snake2);
    uint8_t j1u = 0 , j1d = 0 , j1l = 0 , j1r = 0 , j1c = 0 ; //joueur1
    uint8_t j2h = 0 , j2b = 0 , j2g = 0, j2e=0 , j2m = 0; //joueur 2
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

    // Déplacer les serpents selon les boutons
    move_snake(&snake1, j1u, j1d, j1l, j1r);
    move_snake(&snake2, j2h, j2b, j2g, j2e);

    // Vérification des collisions entre les serpents
    if (check_collision(&snake1, &snake2)) {
        HAL_Delay(500);
    }
    if (check_collision(&snake2, &snake1)) {
        HAL_Delay(500);
    }

    // Vérification si un serpent mange une pomme
    bool apple_eaten = false;

    if (eat_apple(&snake1)) {
        score1++;
        apple_eaten = true;
    }
    if (eat_apple(&snake2)) {
        score2++;
        apple_eaten = true;
    }

    if (apple_eaten) {
        spawn_apple();
    }

    // Mettre à jour l'affichage
    DISPLAYSNAKE_draw_scores(score1, score2);
    DISPLAYSNAKE_refresh_snake(&snake1, 0);
    DISPLAYSNAKE_refresh_snake(&snake2, 1);
    DISPLAYSNAKE_refresh_apple(&apple);
}
