/**
 * @file display.h
 * @author MOUSTAPHA et ATIGOSSOU
 * @brief Gestion de l'affichage pour le jeu de Pong
 * @details Ce fichier contient les fonctions pour gérer l'affichage du jeu Pong
 *          sur l'écran TFT, incluant l'affichage de la balle, des raquettes,
 *          de la ligne centrale et des scores.
 */

#ifndef DISPLAY_H_
#define DISPLAY_H_

#include "pong.h"

/**
 * @brief Initialise l'affichage pour le jeu
 */
void DISPLAY_init(void);

/**
 * @brief Rafraîchit l'affichage de la balle
 * @param ball Pointeur vers la structure de la balle à afficher
 */
void DISPLAY_refresh_ball(ball_t *ball);

/**
 * @brief Rafraîchit l'affichage d'une raquette
 * @param racket Pointeur vers la structure de la raquette à afficher
 */
void DISPLAY_refresh_racket(racket_t *racket);

/**
 * @brief Dessine la ligne centrale du terrain
 */
void DISPLAY_draw_middle_line(void);

/**
 * @brief Affiche les scores des deux joueurs
 * @param score_p1 Score du joueur 1
 * @param score_p2 Score du joueur 2
 */
void DISPLAY_show_scores(uint8_t score_p1, uint8_t score_p2);

#endif /* DISPLAY_H_ */
