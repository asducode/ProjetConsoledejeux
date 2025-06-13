/**
 * @file tir.h
 * @author MOUSTAPHA et ATIGOSSOU
 * @brief Gestion du jeu de Tir
 * @details Ce fichier contient les définitions et structures nécessaires pour le jeu de Tir,
 *          incluant les dimensions du terrain, les joueurs, les projectiles et les états du jeu.
 */

#ifndef TIR_H_
#define TIR_H_

#include <stdint.h>

/**
 * @brief Largeur de l'écran en pixels
 */
#define SCREEN_WIDTH   320

/**
 * @brief Hauteur de l'écran en pixels
 */
#define SCREEN_HEIGHT  240

/**
 * @brief Largeur d'un joueur en pixels
 */
#define PLAYER_WIDTH_TIR   15

/**
 * @brief Hauteur d'un joueur en pixels
 */
#define PLAYER_HEIGHT_TIR  25

/**
 * @brief Vitesse des projectiles
 */
#define BULLET_SPEED   8

/**
 * @brief Vitesse de déplacement des joueurs
 */
#define PLAYER_SPEED   5

/**
 * @brief Nombre maximum de projectiles par joueur
 */
#define MAX_BULLETS    5

/**
 * @brief Énumération des états possibles du jeu
 */
typedef enum {
    TIR_GAME_WAIT_START,  /**< En attente du début de la partie */
    TIR_GAME_RUNNING,     /**< Partie en cours */
    TIR_GAME_OVER        /**< Partie terminée */
} gametir_state_t;

/**
 * @brief Structure représentant un joueur
 */
typedef struct {
    int16_t x, y;          /**< Position actuelle */
    int16_t prev_x, prev_y;/**< Position précédente */
    uint8_t health;        /**< Points de vie */
} playertir_t;

/**
 * @brief Structure représentant un projectile
 */
typedef struct {
    int16_t x, y;          /**< Position actuelle */
    int16_t prev_x, prev_y;/**< Position précédente */
    int16_t speed_x;       /**< Vitesse horizontale */
    uint8_t active;        /**< État d'activation du projectile */
} bullet_t;

/**
 * @brief Initialise le jeu de Tir
 */
void TIR_init(void);

/**
 * @brief Fonction principale de traitement du jeu
 * @details Cette fonction gère la logique du jeu, les déplacements et les collisions
 */
void TIR_process_main(void);

/**
 * @brief Fait tirer un joueur
 * @param player Pointeur vers la structure du joueur qui tire
 */
void TIR_shoot(playertir_t *player);

/**
 * @brief Vérifie les collisions entre les projectiles et les joueurs
 */
void TIR_check_collisions(void);

#endif /* TIR_H_ */
