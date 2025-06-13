/**
 * @file displaytir.h
 * @author MOUSTAPHA et ATIGOSSOU
 *
 * @brief Gestion de l'affichage pour le jeu de Tir
 * @details Ce fichier contient les fonctions pour gérer l'affichage du jeu de Tir
 *          sur l'écran TFT, incluant l'affichage des joueurs, des projectiles,
 *          des points de vie et du gagnant.
 */

#ifndef DISPLAYTIR_H_
#define DISPLAYTIR_H_

#include "tir.h"

/**
 * @brief Initialise l'affichage pour le jeu de Tir
 */
void DISPLAY_TIR_init(void);

/**
 * @brief Rafraîchit l'affichage des deux joueurs
 * @param p1 Pointeur vers la structure du joueur 1
 * @param p2 Pointeur vers la structure du joueur 2
 */
void DISPLAY_TIR_refresh_players(playertir_t *p1, playertir_t *p2);

/**
 * @brief Rafraîchit l'affichage des projectiles des deux joueurs
 * @param bullets_p1 Tableau des projectiles du joueur 1
 * @param bullets_p2 Tableau des projectiles du joueur 2
 */
void DISPLAY_TIR_refresh_bullets(bullet_t bullets_p1[], bullet_t bullets_p2[]);

/**
 * @brief Affiche les points de vie des deux joueurs
 * @param health_p1 Points de vie du joueur 1
 * @param health_p2 Points de vie du joueur 2
 */
void DISPLAY_TIR_draw_health(uint8_t health_p1, uint8_t health_p2);

/**
 * @brief Affiche le gagnant de la partie
 * @param winner Numéro du joueur gagnant (1 ou 2)
 */
void DISPLAY_TIR_show_winner(uint8_t winner);

/**
 * @brief Efface un projectile de l'écran
 * @param bullet Pointeur vers la structure du projectile à effacer
 */
void DISPLAY_TIR_erase_bullet(bullet_t *bullet);

#endif /* DISPLAYTIR_H_ */
