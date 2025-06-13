#include "stm32g4xx_hal.h"
#include "button.h"
#include "snake.h"
#include "football.h"
#include "tir.h"
#include "pong.h"
#include "breakbricks.h"

#include "stm32g4_uart.h"
#include "stm32g4_sys.h"
#include "config.h"
#include "stm32g4_gpio.h"
#include "stm32g4_utils.h"

#include "TFT_ili9341/stm32g4_ili9341.h"
#include "TFT_ili9341/stm32g4_fonts.h"

#include <stdbool.h>
#include <stdio.h>

typedef enum {
    GAME_SNAKE = 0,
    GAME_FOOTBALL,
    GAME_TIR,
    GAME_PONG,
    GAME_BREAKBRICKS,
    GAME_COUNT
} Game;

const char* game_names[GAME_COUNT] = {
    "Snake",
    "Football",
    "Tir",
    "Pong",
    "BreakBricks"
};

// Initialisation du système
void init_system(void)
{
    HAL_Init();
    BSP_systick_init();

    ILI9341_Init();
    ILI9341_setConfig();
    ILI9341_Rotate(ILI9341_Orientation_Landscape_2); // Rotation de 90°
    ILI9341_Fill(ILI9341_COLOR_BLACK);

    BUTTONS_init();
}

// Affichage du menu
void display_menu(Game selected_game) {
    ILI9341_Fill(ILI9341_COLOR_BLACK);
    ILI9341_printf(10, 10, &Font_11x18, ILI9341_COLOR_WHITE, ILI9341_COLOR_BLACK, "=== Choisissez un jeu ===");

    for (int i = 0; i < GAME_COUNT; i++) {
        if (i == selected_game) {
            ILI9341_printf(10, 40 + i * 25, &Font_11x18, ILI9341_COLOR_YELLOW, ILI9341_COLOR_BLACK, "> %s <", game_names[i]);
        } else {
            ILI9341_printf(10, 40 + i * 25, &Font_11x18, ILI9341_COLOR_WHITE, ILI9341_COLOR_BLACK, "  %s", game_names[i]);
        }
    }

    ILI9341_printf(10, 220, &Font_11x18, ILI9341_COLOR_CYAN, ILI9341_COLOR_BLACK, "Haut/Bas = Naviguer");
    ILI9341_printf(10, 240, &Font_11x18, ILI9341_COLOR_CYAN, ILI9341_COLOR_BLACK, "Centre = Sélection");
}

int main(void) {
    init_system();
    		BSP_UART_init(UART2_ID, 9600);
    		// Redirection des printf vers l’UART2
    		BSP_SYS_set_std_usart(UART2_ID, UART2_ID, UART2_ID);
    		BSP_UART_init(UART1_ID, 9600);

    		BSP_GPIO_pin_config(LED_GREEN_GPIO, LED_GREEN_PIN, GPIO_MODE_OUTPUT_PP,
    				                    GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH, GPIO_NO_AF);
    Game selected_game = GAME_SNAKE;
    Game previous_selection = GAME_COUNT;  // Force affichage initial

    display_menu(selected_game);

    while (1) {

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
    	         			break ;
    	         			default:
    	         		   				// Caractère non reconnu - on ignore
    	         			break;
    	         		}
    	             }
        uint8_t button_up = BUTTON_left_read();
        uint8_t button_down = BUTTON_down_read();
        uint8_t button_center = BUTTON_center_read();

        bool refresh_needed = false;

        if (button_up || j1u || j2h) {
            selected_game = (selected_game == 0) ? GAME_COUNT - 1 : selected_game - 1;
            refresh_needed = true;
            HAL_Delay(1);
        }

        if (button_down || j1d || j2b) {
            selected_game = (selected_game + 1) % GAME_COUNT;
            refresh_needed = true;
            HAL_Delay(1);
        }

        if (refresh_needed && selected_game != previous_selection) {
            display_menu(selected_game);
            previous_selection = selected_game;
        }

        if (button_center || j1c || j2m) {
            ILI9341_Fill(ILI9341_COLOR_BLACK);
            ILI9341_printf(10, 100, &Font_11x18, ILI9341_COLOR_GREEN, ILI9341_COLOR_BLACK,
                           "--- Lancement de : %s ---", game_names[selected_game]);
            HAL_Delay(1000);
            break;
        }

        HAL_Delay(10);
    }

    // Lancer le jeu sélectionné
    switch (selected_game) {
        case GAME_SNAKE:
            snake_init();
            while (1) { snake_update(); HAL_Delay(300); }
        case GAME_FOOTBALL:
            FOOTBALL_init();
            while (1) FOOTBALL_process_main();
        case GAME_TIR:
            TIR_init();
            while (1) TIR_process_main();
        case GAME_PONG:
            PONG_init();
            while (1) PONG_process_main();
        case GAME_BREAKBRICKS:
            BREAKBRICKS_init();
            while (1) BREAKBRICKS_process_main();
    }

    return 0;
}
