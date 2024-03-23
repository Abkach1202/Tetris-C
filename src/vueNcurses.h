#ifndef VUENCURSES_H
#define VUENCURSES_H

#include <ncurses.h>
#include "vue.h"

// Structure de la vue Ncurses
typedef struct {
  WINDOW *boxJeu, *boxTerrain, *boxSuivante, *boxScore, *boxMessage;
  WINDOW **terrain, **suivante;
} VueNcurses;

/**
 * @brief Crée une box en dessinant un rectangle qui commence à (x, y) de hauteur h et de largeur w.
 * @param x représente l'abscisse à laquelle il commence à dessiner.
 * @param y représente l'ordonnées à laquelle il commence à dessiner.
 * @param w représente la largeur du rectangle à dessiner.
 * @param h représente la hauteur du rectangle à dessiner.
 * @return un pointeur vers la fenêtre créée pour dessiner le rectangle.
 */
WINDOW *creeBox(uint16_t x, uint16_t y, uint16_t w, uint16_t h);

/**
 * @brief Crée un ensemble de case pour le jeu. Une case représente une fenêtre de dimensions
 * HAUT_CASE et LARG_CASE avec un fond de couleur.
 * @param x représente l'abscisse à laquelle il commence à créer les cases.
 * @param y représente l'ordonnées à laquelle il commence à créer les cases.
 * @param w représente le nombre de case sur une ligne.
 * @param h représente le nombre de case sur une colonne.
 * @return un pointeur vers l'ensemble des cases créées .
 */
WINDOW **creeLesCases(uint16_t x, uint16_t y, uint16_t w, uint16_t h);

/**
 * @brief Crée et initialiser la vue Ncurses du jeu Tetris.
 * @param nbLignes représente le nombre de lignes du terrain du jeu.
 * @param nbColonnes représente le nombre de colonnes du terrain du jeu.
 * @return un pointeur vers la vue ou NULL si il y'a eu erreur
 */
Vue *initVueNcurses(uint16_t nbLignes, uint16_t nbColonnes);

/**
 * @brief Détruit et libère l'espace occupée par la vue Ncurses du jeu.
 * @param vue représente la vue Ncurses à détruire.
 */
void detruitVueNcurses(Vue *vue);

/**
 * @brief Dessine la case (x, y) du terrain d'affichage du jeu en mettant la couleur spécifiée comme
 * couleur de fond.
 * @param vue représente la vue Ncurses du jeu.
 * @param x représente le numéro de colonne de la case.
 * @param y représente le numéro de ligne de la case.
 * @param couleur représente la couleur utilisée pour dessiner la case.
 */
void dessineCaseNcurses(Vue *vue, uint16_t x, uint16_t y, Couleur couleur);

/**
 * @brief Dessine la case (x, y) du terrain d'affichage de la forme suivante en mettant la couleur
 * spécifiée comme couleur de fond.
 * @param vue représente la vue Ncurses du jeu.
 * @param x représente le numéro de colonne de la case.
 * @param y représente le numéro de ligne de la case.
 * @param couleur représente la couleur utilisée pour dessiner la case.
 */
void dessineCaseSuivanteNcurses(Vue *vue, uint16_t x, uint16_t y, Couleur couleur);

/**
 * @brief Dessine la forme courante sur le terrain d'affichage du jeu avec la couleur spécifiée.
 * @param vue représente la vue Ncurses du jeu.
 * @param coords représente les coordonnées de la forme sur le terrain.
 * @param couleur représente la couleur utilisée pour dessiner la forme.
 */
void dessineFormeNcurses(Vue *vue, Couple *coords, Couleur couleur);

/**
 * @brief Dessine la forme suivante sur le terrain d'affichage de la forme suivante avec la couleur
 * spécifiée.
 * @param vue représente la vue Ncurses du jeu.
 * @param coords représente les coordonnées relatives de la forme suivante.
 * @param couleur représente la couleur utilisée pour dessiner la forme.
 */
void dessineFormeSuivanteNcurses(Vue *vue, Couple *coords, Couleur couleur);

/**
 * @brief Dessine toutes les cases du terrain d'affichage du jeu avec leurs couleurs.
 * @param vue représente la vue Ncurses du jeu.
 * @param terrain représente le terrain du jeu à dessiner.
 */
void dessineTerrainNcurses(Vue *vue, Couleur *terrain);

/**
 * @brief nettoie le terrain d'affichage de la forme suivante en mettant la couleur NOIR dans toutes
 * les cases.
 * @param vue représente la vue Ncurses du jeu.
 */
void nettoieTerrainSuivanteNcurses(Vue *vue);

/**
 * @brief Met à jour le score sur la vue du jeu.
 * @param vue représente la vue Ncurses du jeu.
 * @param score représente le score du jeu.
 */
void afficheScoreNcurses(Vue *vue, uint16_t score);

/**
 * @brief Permet d'afficher un message dans la box des messages.
 * @param vue représente la vue Ncurses du jeu.
 * @param s représente le message à afficher.
 */
void afficheMessageNcurses(Vue *vue, char *s);

/**
 * @brief Fonction permettant d'ecouter l'évènement lancé par l'utilisateur
 * @return l'évènement correspondant au clic de l'utilisateur.
 */
Evenement ecouteNcurses();

/**
 * @brief rafraichi la vue Ncurses du jeu.
 * @param vue représente la vue Ncurses du jeu.
 */
void rafraichiVueNcurses(Vue *vue);

/**
 * @brief Met à jour la vue Ncurses du jeu.
 * @param vue représente la vue Ncurses du jeu.
 * @param modele représente le modèledu jeu.
 * @param errEtColl représente un entier qui dit si il y'a eu collision ou erreur
 * @param fini représente un booléen qui dit si le jeu est terminé.
 * @param pause représente un booléen qui dit si le jeu est en pause.
 * @return 0 si tout s'est bien passée et 1 si non.
 */
uint8_t metVueAJourNcurses(Vue *vue,
                           Modele *modele,
                           int8_t errEtColl,
                           uint16_t pause,
                           uint16_t fini);

#endif