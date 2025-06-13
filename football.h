/**
 * @file football.h
 * @brief Gestion du jeu de Football
 * @author MOUSTAPHA et ATIGOSSOU
 *
 * @details Ce fichier contient les définitions et structures nécessaires pour le jeu de Football,
 *          incluant les dimensions du terrain, les joueurs, la balle et les états du jeu.
 */

#ifndef FOOTBALL_H_
#define FOOTBALL_H_

#include <stdint.h>
#include "stm32g4_utils.h"

/**
 * @brief Largeur de l'écran en pixels
 */
#define SCREEN_WIDTH   320

/**
 * @brief Hauteur de l'écran en pixels
 */
#define SCREEN_HEIGHT  240

/**
 * @brief Longueur du but en pixels
 */
#define GOAL_LENGTH    60  // Longueur du but

/**
 * @brief Largeur d'un joueur en pixels
 */
#define PLAYER_WIDTH_FOOT   15

/**
 * @brief Hauteur d'un joueur en pixels
 */
#define PLAYER_HEIGHT_FOOT  40

/**
 * @brief Taille de la balle en pixels
 */
#define BALL_SIZE_FOOT      10

/**
 * @brief Vitesse de déplacement des joueurs
 */
#define PLAYER_SPEED_FOOT   5

/**
 * @brief Vitesse de déplacement de la balle
 */
#define BALL_SPEED_FOOT     4

/**
 * @brief Énumération des états possibles du jeu
 */
typedef enum {
    FOOT_GAME_WAIT_START,
    FOOT_GAME_RUNNING,
    FOOT_GAME_OVER
} gamefoot_state_t;

/**
 * @brief Structure représentant un joueur
 */
typedef struct {
    int16_t x, y;
    int16_t prev_x, prev_y;
    int16_t width, height;
    int16_t speed_x, speed_y;
} playerfoot_t;

/**
 * @brief Structure représentant la balle
 */
typedef struct {
    int16_t x, y;
    int16_t speed_x, speed_y;
    int16_t size;
} ballfoot_t;

/**
 * @brief Initialise le jeu de Football
 */
void FOOTBALL_init(void);

/**
 * @brief Fonction principale de traitement du jeu
 * @details Cette fonction gère la logique du jeu, les collisions et les mises à jour
 */
void FOOTBALL_process_main(void);

#endif /* FOOTBALL_H_ */
