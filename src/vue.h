#ifndef VUE_H
#define VUE_H

#include "modele.h"

// Macros pour les differents messages dans le jeu
#define MSG_JEU \
  "FLECHE pour diriger\n\nESPACE pour tourner\n\nENTREE pour pauser\n\nECHAP  pour sortir"

#define MSG_PAUSE "ENTREE pour jouer\n\nR pour recommencer\n\nECHAP pour quitter"
#define MSG_FIN "ECHAP pour quitter le jeu\n\nR pour recommencer le jeu"

// Énumération des évènements interprétés
typedef enum evenement {
  ECHAP = 0,
  ESPACE,
  ENTREE,
  FGAUCHE,
  FHAUT,
  FDROITE,
  FBAS,
  TOUCHE_R,
  RIEN
} Evenement;

// Structure de la vue du jeu
typedef struct vue {
  void *data;
  uint16_t nbLignes, nbColonnes;
  Evenement (*ecoute)();
  uint8_t (*metVueAJour)(struct vue *, Modele *, int8_t, uint16_t, uint16_t);
  void (*detruitVue)(struct vue *);
} Vue;

/**
 * @brief Crée et initialise la vue SDL ou Ncurses selon ce que l'utilisateur a choisi.
 * @param vtype représente le choix de l'utilisateur. Sa valeur est soit "sdl", soit "ncurses".
 * @param nbLignes représente le nombre de lignes du terrain du jeu.
 * @param nbColonnes représente le nombre de colonnes du terrain du jeu.
 * @return un pointeur vers la vue ou NULL si il y'a eu erreur
 */
Vue *initVue(char *vtype, uint16_t nbLignes, uint16_t nbColonnes);

#endif