/**
 * @file displaybreak.h
 * @brief Gestion de l'affichage pour le jeu Break Bricks
 * @details Ce fichier contient les fonctions pour gérer l'affichage du jeu Break Bricks
 *          sur l'écran TFT, incluant l'affichage de la balle, de la raquette,
 *          de la grille de briques et des vies restantes.
 */

#ifndef DISPLAY_H_
#define DISPLAY_H_

#include "breakbricks.h"

/**
 * @brief Initialise l'affichage pour le jeu Break Bricks
 */
void DISPLAYBREAK_init(void);

/**
 * @brief Rafraîchit l'affichage de la balle
 * @param ball Pointeur vers la structure de la balle à afficher
 */
void DISPLAYBREAK_refresh_ball(ballbreak_t *ball);

/**
 * @brief Rafraîchit l'affichage du nombre de vies restantes
 * @param life Nombre de vies restantes
 */
void DISPLAYBREAK_refresh_life(int8_t life);

/**
 * @brief Rafraîchit l'affichage de la raquette
 * @param racket Pointeur vers la structure de la raquette à afficher
 */
void DISPLAYBREAK_refresh_racket(racketbreak_t *racket);

/**
 * @brief Rafraîchit l'affichage de la grille de briques
 * @param grid Pointeur vers la structure de la grille à afficher
 */
void DISPLAYBREAK_refresh_grid(grid_t * grid);

#endif /* DISPLAY_H_ */
