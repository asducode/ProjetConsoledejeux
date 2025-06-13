/**
 * @file snake.h
 *  @author MOUSTAPHA et ATIGOSSOU
 * @brief Gestion du jeu Snake
 * @details Ce fichier contient les définitions et structures nécessaires pour le jeu Snake,
 *          incluant les structures pour les serpents, les pommes et les directions.
 */

#ifndef SNAKE_H
#define SNAKE_H

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Longueur maximale d'un serpent
 */
#define MAX_SNAKE_LENGTH 1000

/**
 * @brief Énumération des directions possibles pour un serpent
 */
typedef enum { 
    DIR_UP,    /**< Direction vers le haut */
    DIR_DOWN,  /**< Direction vers le bas */
    DIR_LEFT,  /**< Direction vers la gauche */
    DIR_RIGHT  /**< Direction vers la droite */
} directionsnake_t;

/**
 * @brief Structure représentant une position dans la grille
 */
typedef struct {
    uint8_t x, y;  /**< Coordonnées X et Y */
} positionsnake_t;

/**
 * @brief Structure représentant un serpent
 */
typedef struct {
    positionsnake_t body[MAX_SNAKE_LENGTH];  /**< Tableau des positions du corps */
    uint8_t length;                          /**< Longueur actuelle du serpent */
    directionsnake_t dir;                    /**< Direction actuelle du serpent */
} snake_t;

/**
 * @brief Structure représentant une pomme
 */
typedef struct {
    uint8_t x, y;  /**< Position de la pomme */
} apple_t;

/**
 * @brief Initialise le jeu Snake
 */
void snake_init(void);

/**
 * @brief Met à jour l'état du jeu
 * @details Cette fonction gère la logique du jeu, les déplacements des serpents,
 *          les collisions et la génération des pommes
 */
void snake_update(void);

#endif
