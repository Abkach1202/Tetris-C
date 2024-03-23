#ifndef VUESDL_H
#define VUESDL_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "vue.h"

// Structure de la vue SDL
typedef struct {
  Couple oTerrain, oSuivante;
  SDL_Window *fenetre;
  SDL_Renderer *renderer;
  TTF_Font *police;
} VueSDL;

/**
 * @brief Crée et initialiser la vue SDL du jeu Tetris.
 * @param nbLignes représente le nombre de lignes du terrain du jeu.
 * @param nbColonnes représente le nombre de colonnes du terrain du jeu.
 * @return un pointeur vers la vue ou NULL si il y'a eu erreur
 */
Vue *initVueSDL(uint16_t nbLignes, uint16_t nbColonnes);

/**
 * @brief Détruit et libère l'espace occupée par la vue SDL du jeu.
 * @param vue représente la vue SDL à détruire.
 */
void detruitVueSDL(Vue *vue);

/**
 * @brief Permet d'avoir la couleur correspondante à l'une des énumérations de couleurs.
 * @param couleur représente la couleur de fond du rectangle à dessiner.
 * @return La couleur SDL_Color correspondante à celle spécifiée.
 */
SDL_Color getSDLColor(Couleur couleur);

/**
 * @brief Dessine un rectangle avec couleur de fond spécifiée et bordures.
 * @param rect représente les dimensions du rectangle à dessiner.
 * @param couleur représente la couleur de fond du rectangle à dessiner.
 * @param epais représente l'épaisseur des bordures du rectangle à dessiner.
 */
uint8_t dessineRectBordures(VueSDL *data, SDL_Rect *rect, Couleur couleur, float epais);

/**
 * @brief Permet d'ecrire le texte en paramètre dans le rectangle spécifiée.
 * @param rect représente les dimensions du rectangle à dessiner.
 * @param s représente le string contenant le texte.
 * @param epais représente l'épaisseur des bordures du rectangle à dessiner.
 */
uint8_t ecritTexte(VueSDL *data, SDL_Rect *rect, char *s);


/**
 * @brief Dessine la case (x, y) du terrain d'affichage du jeu en mettant la couleur spécifiée comme
 * couleur de fond.
 * @param vue représente la vue SDL du jeu.
 * @param x représente le numéro de colonne de la case.
 * @param y représente le numéro de ligne de la case.
 * @param couleur représente la couleur utilisée pour dessiner la case.
 */
uint8_t dessineCaseSDL(Vue *vue, uint16_t x, uint16_t y, Couleur couleur);

/**
 * @brief Dessine la case (x, y) du terrain d'affichage de la forme suivante en mettant la couleur
 * spécifiée comme couleur de fond.
 * @param vue représente la vue SDL du jeu.
 * @param x représente le numéro de colonne de la case.
 * @param y représente le numéro de ligne de la case.
 * @param couleur représente la couleur utilisée pour dessiner la case.
 */
uint8_t dessineCaseSuivanteSDL(Vue *vue, uint16_t x, uint16_t y, Couleur couleur);

/**
 * @brief Dessine la forme courante sur le terrain d'affichage du jeu avec la couleur spécifiée.
 * @param vue représente la vue SDL du jeu.
 * @param coords représente les coordonnées de la forme sur le terrain.
 * @param couleur représente la couleur utilisée pour dessiner la forme.
 */
uint8_t dessineFormeSDL(Vue *vue, Couple *coords, Couleur couleur);

/**
 * @brief Dessine la forme suivante sur le terrain d'affichage de la forme suivante avec la couleur
 * spécifiée.
 * @param vue représente la vue SDL du jeu.
 * @param coords représente les coordonnées relatives de la forme suivante.
 * @param couleur représente la couleur utilisée pour dessiner la forme.
 */
uint8_t dessineFormeSuivanteSDL(Vue *vue, Couple *coords, Couleur couleur);

/**
 * @brief Dessine toutes les cases du terrain d'affichage du jeu avec leurs couleurs.
 * @param vue représente la vue SDL du jeu.
 * @param terrain représente le terrain du jeu à dessiner.
 */
uint8_t dessineTerrainSDL(Vue *vue, Couleur *terrain);

/**
 * @brief nettoie le terrain d'affichage de la forme suivante en mettant la couleur NOIR dans toutes
 * les cases.
 * @param vue représente la vue SDL du jeu.
 */
uint8_t nettoieTerrainSuivanteSDL(Vue *vue);

/**
 * @brief Met à jour le score sur la vue du jeu.
 * @param vue représente la vue SDL du jeu.
 * @param score représente le score du jeu.
 */
uint8_t afficheScoreSDL(Vue *vue, uint16_t score);

/**
 * @brief Permet d'afficher un message dans la box des messages.
 * @param vue représente la vue SDL du jeu.
 * @param s représente le message à afficher.
 */
uint8_t afficheMessageSDL(Vue *vue);

/**
 * @brief Fonction permettant d'ecouter l'évènement lancé par l'utilisateur
 * @return l'évènement correspondant au clic de l'utilisateur.
 */
Evenement ecouteSDL();

/**
 * @brief Met à jour la vue Ncurses du jeu.
 * @param vue représente la vue SDL du jeu.
 * @param modele représente le modèle du jeu.
 * @param errEtColl représente un entier qui dit si il y'a eu collision ou erreur
 * @param fini représente un booléen qui dit si le jeu est terminé.
 * @param pause représente un booléen qui dit si le jeu est en pause.
 * @return 0 si tout s'est bien passée et 1 si non.
 */
uint8_t metVueAJourSDL(Vue *vue, Modele *modele, int8_t errEtColl, uint16_t fini, uint16_t pause);

#endif