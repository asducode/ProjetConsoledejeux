/**
 * @file displayfoot.h
 * @author MOUSTAPHA et ATIGOSSOU
 * @brief Gestion de l'affichage pour le jeu de Football
 * @details Ce fichier contient les fonctions pour gérer l'affichage du jeu de Football
 *          sur l'écran TFT, incluant l'affichage du terrain, de la balle,
 *          des joueurs, des scores et du gagnant.
 */

#ifndef DISPLAYFOOT_H_
#define DISPLAYFOOT_H_

#include <stdint.h>
#include "football.h"
//#include "stm32g4_ili9341.h"

/**
 * @brief Initialise l'affichage pour le jeu de Football
 */
void DISPLAYFOOT_init(void);

/**
 * @brief Affiche le terrain de football
 */
void DISPLAYFOOT_show_field(void);

/**
 * @brief Affiche les scores des deux joueurs
 * @param score_p1 Score du joueur 1
 * @param score_p2 Score du joueur 2
 */
void DISPLAYFOOT_draw_scores(uint8_t score_p1, uint8_t score_p2);

/**
 * @brief Rafraîchit l'affichage de la balle
 * @param ball Pointeur vers la structure de la balle à afficher
 */
void DISPLAYFOOT_refresh_ball(ballfoot_t *ball);

/**
 * @brief Rafraîchit l'affichage des deux joueurs
 * @param p1 Pointeur vers la structure du joueur 1
 * @param p2 Pointeur vers la structure du joueur 2
 */
void DISPLAYFOOT_refresh_players(playerfoot_t *p1, playerfoot_t *p2);

/**
 * @brief Affiche le gagnant de la partie
 * @param winner Numéro du joueur gagnant (1 ou 2)
 */
void DISPLAYFOOT_show_winner(uint8_t winner);

#endif /* DISPLAYFOOT_H_ */
