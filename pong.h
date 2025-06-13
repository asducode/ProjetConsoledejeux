/**
 * @file pong.h
 *  @author MOUSTAPHA et ATIGOSSOU
 * @brief Gestion du jeu Pong
 * @details Ce fichier contient les définitions et structures nécessaires pour le jeu Pong,
 *          incluant les dimensions du terrain, les raquettes, la balle et les états du jeu.
 */

#ifndef PONG_H_
#define PONG_H_

#include <stdint.h>
#include "stm32g4_utils.h"
#include "stm32g4xx_hal.h"

/**
 * @brief Largeur de l'écran en pixels
 */
#define SCREEN_WIDTH	320

/**
 * @brief Hauteur de l'écran en pixels
 */
#define SCREEN_HEIGHT	240

/**
 * @brief Marge par défaut en pixels
 */
#define MARGIN_DEFAULT 2

/**
 * @brief Vitesse de déplacement des raquettes
 */
#define RACKET_SPEED 15

/**
 * @brief Période de rafraîchissement du jeu en millisecondes
 */
#define REFRESH_PERIOD_MS  30

/**
 * @brief Largeur d'une raquette en pixels
 */
#define RACKET_WIDTH 4

/**
 * @brief Hauteur d'une raquette en pixels
 */
#define RACKET_HEIGHT 35

/**
 * @brief Énumération des états possibles du jeu
 */
typedef enum {
    GAME_WAIT_START,  /**< En attente du début de la partie */
    GAME_RUNNING,     /**< Partie en cours */
    GAME_OVER        /**< Partie terminée */
} game_state_t;

/**
 * @brief Structure représentant une raquette
 */
typedef struct {
	int16_t x;        /**< Position X */
	int16_t y;        /**< Position Y */
	int16_t left;     /**< Limite gauche */
	int16_t right;    /**< Limite droite */
	int16_t width;    /**< Largeur */
	int16_t height;   /**< Hauteur */
	int16_t speed;    /**< Vitesse de déplacement */
}racket_t;

/**
 * @brief Énumération des options possibles pour la balle
 */
typedef enum {
	BALL_OPTION_NONE,   /**< Aucune option */
	BALL_OPTION_JOKER,  /**< Option joker */
	BALL_OPTION_GLUE,   /**< Option colle */
	BALL_OPTION_NB,     /**< Nombre d'options */
}ball_options_e;

/**
 * @brief Structure représentant la balle
 */
typedef struct {
	int16_t x;          /**< Position X */
	int16_t y;          /**< Position Y */
	int16_t fine_x;     /**< Position X fine */
	int16_t fine_y;     /**< Position Y fine */
	int16_t speed_x;    /**< Vitesse horizontale */
	int16_t speed_y;    /**< Vitesse verticale */
	int16_t size;       /**< Taille de la balle */
	int8_t joker;       /**< État du joker */
	ball_options_e options; /**< Options actives */
}ball_t;

/**
 * @brief Initialise le jeu Pong
 */
void PONG_init(void);

/**
 * @brief Fonction principale de traitement du jeu
 * @details Cette fonction gère la logique du jeu, les collisions et les mises à jour
 */
void PONG_process_main(void);

/**
 * @brief Affiche le gagnant de la partie
 * @param winner Numéro du joueur gagnant (1 ou 2)
 */
void DISPLAY_show_winner(uint8_t winner);

#endif /* PONG_H_ */
