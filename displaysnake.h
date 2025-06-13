/**
 * @file displaysnake.h
 * @author MOUSTAPHA et ATIGOSSOU
 *
 * @brief Gestion de l'affichage pour le jeu Snake
 * @details Ce fichier contient les fonctions pour gérer l'affichage du jeu Snake
 *          sur l'écran TFT, incluant l'affichage des serpents, des pommes,
 *          des scores et la gestion de la grille de jeu.
 */

#ifndef DISPLAYSNAKE_H
#define DISPLAYSNAKE_H

#include <stdint.h>
#include "snake.h"

/**
 * @brief Largeur de l'écran en pixels
 */
#define SCREEN_WIDTH  240

/**
 * @brief Hauteur de l'écran en pixels
 */
#define SCREEN_HEIGHT 320

/**
 * @brief Taille d'une cellule de la grille en pixels
 */
#define CELL_SIZE 10

/**
 * @brief Largeur de la grille en nombre de cellules
 */
#define GRID_WIDTH  (SCREEN_WIDTH / CELL_SIZE)

/**
 * @brief Hauteur de la grille en nombre de cellules
 */
#define GRID_HEIGHT (SCREEN_HEIGHT / CELL_SIZE)

/**
 * @brief Initialise l'affichage pour le jeu Snake
 */
void DISPLAYSNAKE_init(void);

/**
 * @brief Efface tout l'écran
 */
void DISPLAYSNAKE_clear(void);

/**
 * @brief Affiche les scores des deux joueurs
 * @param score1 Score du joueur 1
 * @param score2 Score du joueur 2
 */
void DISPLAYSNAKE_draw_scores(uint8_t score1, uint8_t score2);

/**
 * @brief Rafraîchit l'affichage d'un serpent
 * @param snake Pointeur vers la structure du serpent à afficher
 * @param player_id Identifiant du joueur (1 ou 2)
 */
void DISPLAYSNAKE_refresh_snake(snake_t* snake, int player_id);

/**
 * @brief Rafraîchit l'affichage d'une pomme
 * @param apple Pointeur vers la structure de la pomme à afficher
 */
void DISPLAYSNAKE_refresh_apple(apple_t* apple);

/**
 * @brief Efface un serpent de l'écran
 * @param snake Pointeur vers la structure du serpent à effacer
 */
void DISPLAYSNAKE_erase_snake(snake_t* snake);

#endif
