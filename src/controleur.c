#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "modele.h"
#include "vue.h"

// Macro pour la valeur d'incrémentation du delai
#define INC_DELAI 75
// Macro pour le nombre d'iteration de la boucle avant l'avancement de la forme
#define MAX_APPEL 5

// Structure permettant de controler le jeu
typedef struct {
  uint16_t nbAppel, delai, estEnPause, estTermine;
  Modele *modele;
  Vue *vue;
} Controleur;

/**
 * @brief Permet de recommencer le jeu
 * @param c représente le controleur du jeu
 * @return 0 si tous s'est bien passée et 1 si non
 */
int8_t recommenceJeu(Controleur *c) {
  int err;
  err = recommenceModele(c->modele);
  c->estEnPause = 0;
  return err;
}

/**
 * @brief Fait l'action correspondant à l'évènement en paramètre.
 * @param c représente le controleur du jeu.
 * @param evt représente l'évènement dont on va faire l'action.
 * @return 0 si tous s'est bien passée et -1 si non.
 */
int8_t action(Controleur *c, Evenement evt) {
  if (c->estEnPause || c->estTermine) {
    switch (evt) {
      case ENTREE :
        c->estEnPause = 0;
        return 0;
      case TOUCHE_R :
        return recommenceJeu(c);
      default :
        return 0;
    }
  }
  switch (evt) {
    case FHAUT :
      if (c->delai + INC_DELAI <= getDelai(c->modele))
        c->delai += INC_DELAI;
      return 0;
    case FBAS :
      if (c->delai - INC_DELAI >= 0)
        c->delai -= INC_DELAI;
      return 0;
    case FGAUCHE :
      formeDecaleGauche(c->modele);
      return 0;
    case FDROITE :
      formeDecaleDroite(c->modele);
      return 0;
    case ESPACE :
      formeTourne(c->modele);
      return 0;
    case ENTREE :
      c->estEnPause = 1;
      return 0;
    default :
      return 0;
  }
}

/**
 * @brief Permet de jouer au jeu tetris.
 * @param c représente le controleur du jeu.
 */
void jouer(Controleur *c) {
  struct timespec delai;
  Evenement evt;
  int8_t errEtColl;

  // On lit le prochain évènement et le traite
  evt = c->vue->ecoute();
  errEtColl = action(c, evt);

  // Si le jeu n'est pas terminée ou en pause et que on a appelé MAX_APPEL fois la fonction
  c->estTermine = estTermine(c->modele);
  if (!c->estTermine && !c->estEnPause && c->nbAppel >= MAX_APPEL) {
    // On fait avancer la forme
    errEtColl = formeAvance(c->modele);
    // On reinitialise le delai si il y'a eu collision
    if (errEtColl == 1)
      c->delai = getDelai(c->modele);
  }

  // On met à jour la vue
  if (c->nbAppel >= MAX_APPEL) {
    c->vue->metVueAJour(c->vue, c->modele, errEtColl, c->estEnPause, c->estTermine);
    c->nbAppel = 0;
  }

  // Si on ne quitte pas le jeu, on attend le delai puis on continue
  if (evt != ECHAP && errEtColl != -1) {
    delai.tv_sec = 0, delai.tv_nsec = (c->delai / MAX_APPEL) * 1000000;
    nanosleep(&delai, NULL);
    c->nbAppel++, jouer(c);
  }
}

/************************ Programme Principale *************************/

int main(int argc, char **argv) {
  srand(time(NULL));
  Controleur c;
  uint16_t nbLignes, nbColonnes;

  // Vérification des paramètres
  if (argc != 4) {
    fprintf(
        stderr,
        "Erreur lors du parsing des paramètres\nSyntaxe : %s {sdl, ncurses} nbLignes nbColonnes",
        argv[0]);
    return EXIT_FAILURE;
  }

  // Initialisation du nombre de lignes et colonnes
  nbLignes = atoi(argv[2]);
  nbColonnes = atoi(argv[3]);
  if (!(10 <= nbLignes && nbLignes <= 25 && 5 <= nbColonnes && nbColonnes <= 40)) {
    fprintf(stderr, "10 <= nbLignes <= 25 et 5 <= nbColonnes <= 40\nPour une bonne affichage\n");
    return EXIT_FAILURE;
  }

  // Initialisation du modèle du jeu.
  c.modele = initModele(nbLignes, nbColonnes);
  if (!c.modele)
    return EXIT_FAILURE;

  // Initialisation de la vue du jeu.
  c.vue = initVue(argv[1], nbLignes, nbColonnes);
  if (!c.vue) {
    detruitModele(c.modele);
    return EXIT_FAILURE;
  }

  // Initialisation du reste des variables
  c.delai = getDelai(c.modele);
  c.estEnPause = 1;
  c.nbAppel = c.estTermine = 0;

  // On joue au jeu
  jouer(&c);

  // Destruction du jeu
  detruitModele(c.modele);
  // Destruction de la vue
  c.vue->detruitVue(c.vue);
  return EXIT_SUCCESS;
}