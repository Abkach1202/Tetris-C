#ifndef FORME_H
#define FORME_H

#include "modele.h"

// Structure d'une forme du jeu Tetris
struct forme {
  uint16_t x0, y0;
  Couleur couleur;
  Modele *modele;
  Couple *forme;
};

/**
 * @brief Crée et initialise une forme.
 * @param modele représente le modèle du jeu Tetris dans lequel sera la forme.
 * @return la nouvelle forme créee (que l'on doit liberer) ou NULL si il y'a erreur.
 */
Forme *initForme(Modele *modele);

/**
 * @brief Détruit et libère l'espace occupée par une forme.
 * @param forme représente la forme à détruire.
 */
void detruitForme(Forme *forme);

/**
 * @brief Permet d'avoir la couleur de la forme spécifiée.
 * @return la couleur de la forme.
 */
Couleur getCouleur(Forme *forme);

/**
 * @brief Permet d'avoir les coordonnées relatives des différentes cases de la forme spécifiée.
 * @param forme représente la forme dont on veut les coordonnées.
 * @param coords représente un pointeur vers un espace où stocker les coordonnées.
 */
void getCoordonees(Forme *forme, Couple *coords);

/**
 * @brief Permet d'avoir les coordonnées des différentes cases de la forme spécifiée dans le terrain
 * du modele.
 * @param forme représente la forme dont on veut les coordonnées.
 * @param coords représente un pointeur vers un espace où stocker les coordonnées.
 */
void getCoordoneesTerrain(Forme *forme, Couple *coords);

/**
 * @brief Parcours la forme et vérifie si elle est en collision (Si elle atteint la derinière ligne
 * ou si la case en dessous d'elle est occupé).
 * @param forme représente la forme dont on va vérifier la collision.
 * @return 1 si elle est en collision et 0 si non.
 */
uint8_t estEnCollision(Forme *forme);

/**
 * @brief Vérifie si les coordonnées de la forme sont toutes valides c'est à dire si elles
 * debordent pas le terrain et qu'elles ne sont pas déjà occupées.
 * @param forme représente la forme dont on va vérifier les coordonnées.
 * @return 1 si elles sont valides et 0 si non.
 */
uint8_t coordonneesValides(Forme *forme);

/**
 * @brief Fait avancer la forme d'une case vers le bas.
 * @param forme représente la forme dont on va faire avancer. (Paramètre modifié)
 */
void avance(Forme *forme);

/**
 * @brief Décale la forme d'une case vers la gauche.
 * @param forme représente la forme dont on va decaler vers la gauche. (Paramètre modifié)
 */
void decaleGauche(Forme *forme);

/**
 * @brief Décale la forme d'une case vers la droite.
 * @param forme représente la forme dont on va decaler vers la droite. (Paramètre modifié)
 */
void decaleDroite(Forme *forme);

/**
 * @brief Tourne la forme d'un angle de 90° dans le sens des aiguilles d'une montre.
 * @param forme représente la forme dont on decale vers la droite. (Paramètre modifié)
 */
void tourne(Forme *forme);

#endif